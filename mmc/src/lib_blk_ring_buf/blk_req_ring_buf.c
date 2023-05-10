#include "blk_req_ring_buf.h"


blk_req_ring_buf_result_t blk_req_ring_buf_init(
        blk_req_ring_buf_t *ring_buf,
        size_t req_ring_buf_region_size
) {
    if (ring_buf == NULL) {
        return ERR_NULL_BLK_REQ_RING_BUF;
    }
    if (req_ring_buf_region_size < sizeof(*ring_buf)) {
        return ERR_BLK_REQ_RING_BUF_REGION_TOO_SMALL;
    }
    size_t const num_unused_bytes = (req_ring_buf_region_size - sizeof(*ring_buf));
    if ((num_unused_bytes / sizeof(blk_req_buf_t)) > 0) {
        return ERR_BLK_REQ_RING_BUF_REGION_TOO_LARGE;
    }
    /* Clear the `ring_buf` data structure, which initialises `head_idx` and
     * `tail_idx` to zero. */
    memset(ring_buf, 0, sizeof(*ring_buf));
    return OK_BLK_REQ_RING_BUF;
}

blk_req_ring_buf_result_t blk_req_ring_buf_capacity(
        blk_req_ring_buf_t *ring_buf,
        size_t *ret_val
) {
    if (ring_buf == NULL) {
        return ERR_NULL_BLK_REQ_RING_BUF;
    }
    if (ret_val == NULL) {
        return ERR_NULL_RET_VAL_PTR_PASSED_TO_BLK_REQ_RING_BUF_FN;
    }
    *ret_val = MAX_NUM_BLK_REQ_BUFS - 1;
    return OK_BLK_REQ_RING_BUF;
}

blk_req_ring_buf_result_t blk_req_ring_buf_size(
        blk_req_ring_buf_t *ring_buf,
        size_t *ret_val
) {
    if (ring_buf == NULL) {
        return ERR_NULL_BLK_REQ_RING_BUF;
    }
    if (ret_val == NULL) {
        return ERR_NULL_RET_VAL_PTR_PASSED_TO_BLK_REQ_RING_BUF_FN;
    }
    size_t const head = ring_buf->head_idx;
    size_t const tail = ring_buf->tail_idx;
    if (tail >= head) {
        *ret_val = (tail - head);
    } else {
        *ret_val = ((tail + MAX_NUM_BLK_REQ_BUFS) - head);
    }
    return OK_BLK_REQ_RING_BUF;
}

blk_req_ring_buf_result_t blk_req_ring_buf_is_empty(
        blk_req_ring_buf_t *ring_buf,
        bool *ret_val
) {
    if (ring_buf == NULL) {
        return ERR_NULL_BLK_REQ_RING_BUF;
    }
    if (ret_val == NULL) {
        return ERR_NULL_RET_VAL_PTR_PASSED_TO_BLK_REQ_RING_BUF_FN;
    }
    *ret_val = (ring_buf->head_idx == ring_buf->tail_idx);
    return OK_BLK_REQ_RING_BUF;
}

blk_req_ring_buf_result_t blk_req_ring_buf_is_full(
        blk_req_ring_buf_t *ring_buf,
        bool *ret_val
) {
    if (ring_buf == NULL) {
        return ERR_NULL_BLK_REQ_RING_BUF;
    }
    if (ret_val == NULL) {
        return ERR_NULL_RET_VAL_PTR_PASSED_TO_BLK_REQ_RING_BUF_FN;
    }
    size_t const head = ring_buf->head_idx;
    size_t const tail = ring_buf->tail_idx;
    size_t const unused_slots = 1;
    *ret_val = (head == ((tail + unused_slots) % MAX_NUM_BLK_REQ_BUFS));
    return OK_BLK_REQ_RING_BUF;
}

blk_req_ring_buf_result_t blk_req_ring_buf_enqueue(
        blk_req_ring_buf_t *ring_buf,
        blk_req_buf_t *val
) {
    if (ring_buf == NULL) {
        return ERR_NULL_BLK_REQ_RING_BUF;
    }
    if (val == NULL) {
        return ERR_NULL_BLK_REQ_BUF;
    }
    bool is_full = false;
    blk_req_ring_buf_result_t const res = blk_req_ring_buf_is_full(ring_buf, &is_full);
    if (res != OK_BLK_REQ_RING_BUF) {
        return res;
    }
    if (is_full) {
        return ERR_BLK_REQ_RING_BUF_FULL;
    }
    /* Copy everything in `val` into the next available slot in the request ring
     * buffer. */
    memcpy(&ring_buf->req_bufs[ring_buf->tail_idx], val, sizeof(*val));
    /* We place a memory barrier here to ensure that the instructions that
     * update the tail index are guaranteed to occur after the buffer has been
     * enqueued onto `ring_buf->req_bufs`. */
    THREAD_MEMORY_RELEASE();
    /* Update the tail index. */
    ring_buf->tail_idx = (ring_buf->tail_idx + 1) % MAX_NUM_BLK_REQ_BUFS;
    return OK_BLK_REQ_RING_BUF;
}

blk_req_ring_buf_result_t blk_req_ring_buf_dequeue(
        blk_req_ring_buf_t *ring_buf,
        blk_req_buf_t *ret_val
) {
    if (ring_buf == NULL) {
        return ERR_NULL_BLK_REQ_RING_BUF;
    }
    if (ret_val == NULL) {
        return ERR_NULL_RET_VAL_PTR_PASSED_TO_BLK_REQ_RING_BUF_FN;
    }
    /* Return an error if the ring buffer is empty and there is nothing to
     * dequeue. */
    bool is_empty = false;
    blk_req_ring_buf_result_t const res = blk_req_ring_buf_is_empty(ring_buf, &is_empty);
    if (res != OK_BLK_REQ_RING_BUF) {
        return res;
    }
    if (is_empty) {
        return ERR_BLK_REQ_RING_BUF_EMPTY;
    }
    /* Copy everything from the first slot into `ret_val`. */
    memcpy(ret_val, &ring_buf->req_bufs[ring_buf->head_idx], sizeof(*ret_val));
    /* We place a memory barrier here to ensure that the instructions that
     * update the head index are guaranteed to occur after the buffer has been
     * dequeued from `ring_buf->req_bufs`. */
    THREAD_MEMORY_RELEASE();
    /* Update the head index. */
    ring_buf->head_idx = (ring_buf->head_idx + 1) % MAX_NUM_BLK_REQ_BUFS;
    return OK_BLK_REQ_RING_BUF;
}



