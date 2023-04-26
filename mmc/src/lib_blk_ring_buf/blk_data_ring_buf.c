#include "blk_data_ring_buf.h"

blk_data_ring_buf_result_t blk_data_ring_buf_init(
        blk_data_ring_buf_t *ring_buf,
        uintptr_t shared_data_region_vaddr,
        size_t shared_data_region_size,
        size_t shared_data_buf_size
) {
    if (ring_buf == NULL) {
        return ERR_NULL_BLK_DATA_RING_BUF;
    }
    if (shared_data_buf_size > shared_data_region_size) {
        return ERR_BLK_DATA_BUF_SIZE_TOO_LARGE;
    }
    if (shared_data_region_size % shared_data_buf_size != 0) {
        return ERR_BLK_DATA_BUF_SIZE_MISALIGNED;
    }
    if (shared_data_region_vaddr == 0) {
        return ERR_INVALID_BLK_DATA_REGION;
    }
    /* Clear the `ring_buf` data structure. */
    memset(ring_buf, 0, sizeof(*ring_buf));
    ring_buf->data_region = shared_data_region_vaddr;
    ring_buf->data_region_size = shared_data_region_size;
    ring_buf->data_buf_size = shared_data_buf_size;
    ring_buf->num_data_bufs = (shared_data_region_size / shared_data_buf_size);
    /* We need at least one slot in `num_data_bufs` to be empty to ensure the
     * head and tail of our ring buffer do not point to the same element in the
     * ring buffer when our ring buffer is full. This breaks the ambiguity that
     * arises from the head and tail pointer pointing to the same element when
     * the ring buffer is empty. */
    if (MAX_NUM_BLK_DATA_BUFS < ring_buf->num_data_bufs + 1) {
        return ERR_INCREASE_MAX_NUM_BLK_DATA_BUFS;
    }
    /* We precompute the number of unused slots to avoid recomputing this value
     * every time we need to check if the ring buffer is full or not for
     * performance reasons. */
    ring_buf->num_unused_slots = (MAX_NUM_BLK_DATA_BUFS - ring_buf->num_data_bufs);
    /* Initialise the head and tail index to empty. */
    ring_buf->head_idx = 0;
    ring_buf->tail_idx = 0;
    /* Enqueue all shared data buffers onto `data_bufs`. */
    for (size_t i = 0; i < ring_buf->num_data_bufs; i++) {
        blk_data_buf_t data_buf = {
                .buf = ring_buf->data_region + (i * ring_buf->data_buf_size),
                .buf_size = ring_buf->data_buf_size,
        };
        blk_data_ring_buf_result_t res = blk_data_ring_buf_enqueue(
                ring_buf,
                &data_buf
        );
        if (res != OK_BLK_DATA_RING_BUF) {
            return res;
        }
    }
    return OK_BLK_DATA_RING_BUF;
}

blk_data_ring_buf_result_t blk_data_ring_buf_capacity(
        blk_data_ring_buf_t *ring_buf,
        size_t *ret_val
) {
    if (ring_buf == NULL) {
        return ERR_NULL_BLK_DATA_RING_BUF;
    }
    *ret_val = ring_buf->num_data_bufs;
    return OK_BLK_DATA_RING_BUF;
}

blk_data_ring_buf_result_t blk_data_ring_buf_is_empty(
        blk_data_ring_buf_t *ring_buf,
        bool *ret_val
) {
    if (ring_buf == NULL) {
        return ERR_NULL_BLK_DATA_RING_BUF;
    }
    *ret_val = (ring_buf->head_idx == ring_buf->tail_idx);
    return OK_BLK_DATA_RING_BUF;
}

blk_data_ring_buf_result_t blk_data_ring_buf_is_full(
        blk_data_ring_buf_t *ring_buf,
        bool *ret_val
) {
    if (ring_buf == NULL) {
        return ERR_NULL_BLK_DATA_RING_BUF;
    }
    size_t const head = ring_buf->head_idx;
    size_t const tail = ring_buf->tail_idx;
    size_t const unused_slots = ring_buf->num_unused_slots;
    *ret_val = (head == ((tail + unused_slots) % MAX_NUM_BLK_DATA_BUFS));
    return OK_BLK_DATA_RING_BUF;
}

blk_data_ring_buf_result_t blk_data_ring_buf_enqueue(
        blk_data_ring_buf_t *ring_buf,
        blk_data_buf_t *val
) {
    if (ring_buf == NULL) {
        return ERR_NULL_BLK_DATA_RING_BUF;
    }
    if (val == NULL) {
        return ERR_NULL_BLK_DATA_BUF_VAL;
    }
    /* Return an error if the ring buffer is full and there is no space to
     * enqueue. */
    bool is_full = false;
    blk_data_ring_buf_result_t res = blk_data_ring_buf_is_full(ring_buf, &is_full);
    if (res != OK_BLK_DATA_RING_BUF) {
        return res;
    }
    if (is_full) {
        return ERR_BLK_DATA_RING_BUF_FULL;
    }
    /* Copy everything from `val` into `ring_buf->data_bufs[ring_buf->tail_idx]`. */
    memcpy(&ring_buf->data_bufs[ring_buf->tail_idx], val, sizeof(*val));
    /* We place a memory barrier here to ensure that the instructions that
     * update the tail index are guaranteed to occur after the buffer has been
     * enqueued onto `ring_buf->data_bufs`. */
    THREAD_MEMORY_RELEASE();
    /* Update the tail index. */
    ring_buf->tail_idx = (ring_buf->tail_idx + 1) % MAX_NUM_BLK_DATA_BUFS;
    return OK_BLK_DATA_RING_BUF;
}

blk_data_ring_buf_result_t blk_data_ring_buf_dequeue(
        blk_data_ring_buf_t *ring_buf,
        blk_data_buf_t *ret_val
) {
    if (ring_buf == NULL) {
        return ERR_NULL_BLK_DATA_RING_BUF;
    }
    if (ret_val == NULL) {
        return ERR_NULL_BLK_DATA_BUF_RET_VAL;
    }
    /* Return an error if the ring buffer is empty and there is nothing to
     * dequeue. */
    bool is_empty = false;
    blk_data_ring_buf_result_t res = blk_data_ring_buf_is_empty(ring_buf, &is_empty);
    if (res != OK_BLK_DATA_RING_BUF) {
        return res;
    }
    if (is_empty) {
        return ERR_BLK_DATA_RING_BUF_EMPTY;
    }
    /* Copy everything from `ring_buf->data_bufs[ring_buf->head_idx]` into `ret_val`. */
    memcpy(ret_val, &ring_buf->data_bufs[ring_buf->head_idx], sizeof(*ret_val));
    /* We place a memory barrier here to ensure that the instructions that
     * update the head index are guaranteed to occur after the buffer has been
     * dequeued from `ring_buf->data_bufs`. */
    THREAD_MEMORY_RELEASE();
    /* Update the head index. */
    ring_buf->head_idx = (ring_buf->head_idx + 1) % MAX_NUM_BLK_DATA_BUFS;
    return OK_BLK_DATA_RING_BUF;
}



