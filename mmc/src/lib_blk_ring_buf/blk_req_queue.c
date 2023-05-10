#include "blk_req_queue.h"


blk_req_queue_result_t blk_req_queue_init(
        blk_req_queue_t *queue,
        size_t req_queue_region_size
) {
    if (queue == NULL) {
        return ERR_NULL_BLK_REQ_QUEUE;
    }
    if (req_queue_region_size < sizeof(*queue)) {
        return ERR_BLK_REQ_QUEUE_REGION_TOO_SMALL;
    }
    size_t const num_unused_bytes = (req_queue_region_size - sizeof(*queue));
    if ((num_unused_bytes / sizeof(blk_req_buf_t)) > 0) {
        return ERR_BLK_REQ_QUEUE_REGION_TOO_LARGE;
    }
    /* Clear the `queue` data structure, which initialises `head_idx` and
     * `tail_idx` to zero. */
    memset(queue, 0, sizeof(*queue));
    return OK_BLK_REQ_QUEUE;
}

blk_req_queue_result_t blk_req_queue_capacity(
        blk_req_queue_t *queue,
        size_t *ret_val
) {
    if (queue == NULL) {
        return ERR_NULL_BLK_REQ_QUEUE;
    }
    if (ret_val == NULL) {
        return ERR_NULL_RET_VAL_PTR_PASSED_TO_BLK_REQ_QUEUE_FN;
    }
    *ret_val = MAX_NUM_BLK_REQ_BUFS - 1;
    return OK_BLK_REQ_QUEUE;
}

blk_req_queue_result_t blk_req_queue_size(
        blk_req_queue_t *queue,
        size_t *ret_val
) {
    if (queue == NULL) {
        return ERR_NULL_BLK_REQ_QUEUE;
    }
    if (ret_val == NULL) {
        return ERR_NULL_RET_VAL_PTR_PASSED_TO_BLK_REQ_QUEUE_FN;
    }
    size_t const head = queue->head_idx;
    size_t const tail = queue->tail_idx;
    if (tail >= head) {
        *ret_val = (tail - head);
    } else {
        *ret_val = ((tail + MAX_NUM_BLK_REQ_BUFS) - head);
    }
    return OK_BLK_REQ_QUEUE;
}

blk_req_queue_result_t blk_req_queue_is_empty(
        blk_req_queue_t *queue,
        bool *ret_val
) {
    if (queue == NULL) {
        return ERR_NULL_BLK_REQ_QUEUE;
    }
    if (ret_val == NULL) {
        return ERR_NULL_RET_VAL_PTR_PASSED_TO_BLK_REQ_QUEUE_FN;
    }
    *ret_val = (queue->head_idx == queue->tail_idx);
    return OK_BLK_REQ_QUEUE;
}

blk_req_queue_result_t blk_req_queue_is_full(
        blk_req_queue_t *queue,
        bool *ret_val
) {
    if (queue == NULL) {
        return ERR_NULL_BLK_REQ_QUEUE;
    }
    if (ret_val == NULL) {
        return ERR_NULL_RET_VAL_PTR_PASSED_TO_BLK_REQ_QUEUE_FN;
    }
    size_t const head = queue->head_idx;
    size_t const tail = queue->tail_idx;
    size_t const unused_slots = 1;
    *ret_val = (head == ((tail + unused_slots) % MAX_NUM_BLK_REQ_BUFS));
    return OK_BLK_REQ_QUEUE;
}

blk_req_queue_result_t blk_req_queue_enqueue(
        blk_req_queue_t *queue,
        blk_req_buf_t *val
) {
    if (queue == NULL) {
        return ERR_NULL_BLK_REQ_QUEUE;
    }
    if (val == NULL) {
        return ERR_NULL_BLK_REQ_BUF;
    }
    bool is_full = false;
    blk_req_queue_result_t const res = blk_req_queue_is_full(queue, &is_full);
    if (res != OK_BLK_REQ_QUEUE) {
        return res;
    }
    if (is_full) {
        return ERR_BLK_REQ_QUEUE_FULL;
    }
    /* Copy everything in `val` into the next available slot in the request ring
     * buffer. */
    memcpy(&queue->req_bufs[queue->tail_idx], val, sizeof(*val));
    /* We place a memory barrier here to ensure that the instructions that
     * update the tail index are guaranteed to occur after the buffer has been
     * enqueued onto `queue->req_bufs`. */
    THREAD_MEMORY_RELEASE();
    /* Update the tail index. */
    queue->tail_idx = (queue->tail_idx + 1) % MAX_NUM_BLK_REQ_BUFS;
    return OK_BLK_REQ_QUEUE;
}

blk_req_queue_result_t blk_req_queue_dequeue(
        blk_req_queue_t *queue,
        blk_req_buf_t *ret_val
) {
    if (queue == NULL) {
        return ERR_NULL_BLK_REQ_QUEUE;
    }
    if (ret_val == NULL) {
        return ERR_NULL_RET_VAL_PTR_PASSED_TO_BLK_REQ_QUEUE_FN;
    }
    /* Return an error if the ring buffer is empty and there is nothing to
     * dequeue. */
    bool is_empty = false;
    blk_req_queue_result_t const res = blk_req_queue_is_empty(queue, &is_empty);
    if (res != OK_BLK_REQ_QUEUE) {
        return res;
    }
    if (is_empty) {
        return ERR_BLK_REQ_QUEUE_EMPTY;
    }
    /* Copy everything from the first slot into `ret_val`. */
    memcpy(ret_val, &queue->req_bufs[queue->head_idx], sizeof(*ret_val));
    /* We place a memory barrier here to ensure that the instructions that
     * update the head index are guaranteed to occur after the buffer has been
     * dequeued from `queue->req_bufs`. */
    THREAD_MEMORY_RELEASE();
    /* Update the head index. */
    queue->head_idx = (queue->head_idx + 1) % MAX_NUM_BLK_REQ_BUFS;
    return OK_BLK_REQ_QUEUE;
}



