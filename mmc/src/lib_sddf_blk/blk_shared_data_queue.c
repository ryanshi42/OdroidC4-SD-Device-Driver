#include "blk_shared_data_queue.h"

blk_shared_data_queue_result_t blk_shared_data_queue_init(
        blk_shared_data_queue_t *queue,
        uintptr_t shared_data_region_vaddr,
        size_t shared_data_region_size,
        size_t shared_data_buf_size
) {
    if (queue == NULL) {
        return ERR_NULL_BLK_SHARED_DATA_QUEUE;
    }
    if (shared_data_buf_size > shared_data_region_size) {
        return ERR_BLK_SHARED_DATA_BUF_SIZE_TOO_LARGE;
    }
    if (shared_data_region_size % shared_data_buf_size != 0) {
        return ERR_BLK_SHARED_DATA_BUF_SIZE_MISALIGNED;
    }
    if (shared_data_region_vaddr == 0) {
        return ERR_INVALID_BLK_SHARED_DATA_REGION;
    }
    /* Clear the `queue` data structure. */
    memset(queue, 0, sizeof(*queue));
    queue->data_region = shared_data_region_vaddr;
    queue->data_region_size = shared_data_region_size;
    queue->data_buf_size = shared_data_buf_size;
    queue->num_data_bufs = (shared_data_region_size / shared_data_buf_size);
    /* We need at least one slot in `num_data_bufs` to be empty to ensure the
     * head and tail of our ring buffer do not point to the same element in the
     * ring buffer when our ring buffer is full. This breaks the ambiguity that
     * arises from the head and tail pointer pointing to the same element, which
     * occurs when the ring buffer is empty. */
    if (MAX_NUM_BLK_SHARED_DATA_BUFS < queue->num_data_bufs + 1) {
        return ERR_INCREASE_MAX_NUM_BLK_SHARED_DATA_BUFS;
    }
    /* We precompute the number of unused slots to avoid recomputing this value
     * every time we need to check if the ring buffer is full or not for
     * performance reasons. */
    queue->num_unused_slots = (MAX_NUM_BLK_SHARED_DATA_BUFS - queue->num_data_bufs);
    /* Initialise the head and tail index to empty. */
    queue->head_idx = 0;
    queue->tail_idx = 0;
    /* Enqueue all shared data buffers onto `data_bufs`. */
    for (size_t i = 0; i < queue->num_data_bufs; i++) {
        blk_shared_data_buf_t data_buf = {
                .buf_vaddr = queue->data_region + (i * queue->data_buf_size),
                .buf_size = queue->data_buf_size,
        };
        blk_shared_data_queue_result_t res = blk_shared_data_queue_enqueue(
                queue,
                &data_buf
        );
        if (res != OK_BLK_SHARED_DATA_QUEUE) {
            return res;
        }
    }
    return OK_BLK_SHARED_DATA_QUEUE;
}

blk_shared_data_queue_result_t blk_shared_data_queue_capacity(
        blk_shared_data_queue_t *queue,
        size_t *ret_val
) {
    if (queue == NULL) {
        return ERR_NULL_BLK_SHARED_DATA_QUEUE;
    }
    if (ret_val == NULL) {
        return ERR_NULL_RET_VAL_PTR_PASSED_TO_BLK_SHARED_DATA_QUEUE_FN;
    }
    *ret_val = queue->num_data_bufs;
    return OK_BLK_SHARED_DATA_QUEUE;
}

blk_shared_data_queue_result_t blk_shared_data_queue_size(
        blk_shared_data_queue_t *queue,
        size_t *ret_val
) {
    if (queue == NULL) {
        return ERR_NULL_BLK_SHARED_DATA_QUEUE;
    }
    if (ret_val == NULL) {
        return ERR_NULL_RET_VAL_PTR_PASSED_TO_BLK_SHARED_DATA_QUEUE_FN;
    }
    size_t const head = queue->head_idx;
    size_t const tail = queue->tail_idx;
    if (tail >= head) {
        *ret_val = (tail - head);
    } else {
        *ret_val = ((tail + MAX_NUM_BLK_SHARED_DATA_BUFS) - head);
    }
    return OK_BLK_SHARED_DATA_QUEUE;
}

blk_shared_data_queue_result_t blk_shared_data_queue_is_empty(
        blk_shared_data_queue_t *queue,
        bool *ret_val
) {
    if (queue == NULL) {
        return ERR_NULL_BLK_SHARED_DATA_QUEUE;
    }
    if (ret_val == NULL) {
        return ERR_NULL_RET_VAL_PTR_PASSED_TO_BLK_SHARED_DATA_QUEUE_FN;
    }
    *ret_val = (queue->head_idx == queue->tail_idx);
    return OK_BLK_SHARED_DATA_QUEUE;
}

blk_shared_data_queue_result_t blk_shared_data_queue_is_full(
        blk_shared_data_queue_t *queue,
        bool *ret_val
) {
    if (queue == NULL) {
        return ERR_NULL_BLK_SHARED_DATA_QUEUE;
    }
    if (ret_val == NULL) {
        return ERR_NULL_RET_VAL_PTR_PASSED_TO_BLK_SHARED_DATA_QUEUE_FN;
    }
    size_t const head = queue->head_idx;
    size_t const tail = queue->tail_idx;
    size_t const unused_slots = queue->num_unused_slots;
    *ret_val = (head == ((tail + unused_slots) % MAX_NUM_BLK_SHARED_DATA_BUFS));
    return OK_BLK_SHARED_DATA_QUEUE;
}

blk_shared_data_queue_result_t blk_shared_data_queue_enqueue(
        blk_shared_data_queue_t *queue,
        blk_shared_data_buf_t *val
) {
    if (queue == NULL) {
        return ERR_NULL_BLK_SHARED_DATA_QUEUE;
    }
    if (val == NULL) {
        return ERR_NULL_BLK_SHARED_DATA_BUF_VAL;
    }
    /* Return an error if the ring buffer is full and there is no space to
     * enqueue. */
    bool is_full = false;
    blk_shared_data_queue_result_t const res = blk_shared_data_queue_is_full(queue, &is_full);
    if (res != OK_BLK_SHARED_DATA_QUEUE) {
        return res;
    }
    if (is_full) {
        return ERR_BLK_SHARED_DATA_QUEUE_FULL;
    }
    /* Copy everything from `val` into `queue->data_bufs[queue->tail_idx]`. */
    /* TODO: Encapsulate into a function call. */
    memcpy(&queue->data_bufs[queue->tail_idx], val, sizeof(*val));
    /* We place a memory barrier here to ensure that the instructions that
     * update the tail index are guaranteed to occur after the buffer has been
     * enqueued onto `queue->data_bufs`. */
    THREAD_MEMORY_RELEASE();
    /* Update the tail index. */
    queue->tail_idx = (queue->tail_idx + 1) % MAX_NUM_BLK_SHARED_DATA_BUFS;
    return OK_BLK_SHARED_DATA_QUEUE;
}

blk_shared_data_queue_result_t blk_shared_data_queue_dequeue(
        blk_shared_data_queue_t *queue,
        blk_shared_data_buf_t *ret_val
) {
    if (queue == NULL) {
        return ERR_NULL_BLK_SHARED_DATA_QUEUE;
    }
    if (ret_val == NULL) {
        return ERR_NULL_RET_VAL_PTR_PASSED_TO_BLK_SHARED_DATA_QUEUE_FN;
    }
    /* Return an error if the ring buffer is empty and there is nothing to
     * dequeue. */
    bool is_empty = false;
    blk_shared_data_queue_result_t const res = blk_shared_data_queue_is_empty(queue, &is_empty);
    if (res != OK_BLK_SHARED_DATA_QUEUE) {
        return res;
    }
    if (is_empty) {
        return ERR_BLK_SHARED_DATA_QUEUE_EMPTY;
    }
    /* Copy everything from `queue->data_bufs[queue->head_idx]` into `ret_val`. */
    /* TODO: Encapsulate into a function call. */
    memcpy(ret_val, &queue->data_bufs[queue->head_idx], sizeof(*ret_val));
    /* We place a memory barrier here to ensure that the instructions that
     * update the head index are guaranteed to occur after the buffer has been
     * dequeued from `queue->data_bufs`. */
    THREAD_MEMORY_RELEASE();
    /* Update the head index. */
    queue->head_idx = (queue->head_idx + 1) % MAX_NUM_BLK_SHARED_DATA_BUFS;
    return OK_BLK_SHARED_DATA_QUEUE;
}

blk_shared_data_queue_result_t blk_shared_data_queue_copy(
        blk_shared_data_queue_t *dst_queue,
        blk_shared_data_queue_t *src_queue
) {
    if (dst_queue == NULL || src_queue == NULL) {
        return ERR_NULL_BLK_SHARED_DATA_QUEUE;
    }
    memcpy(dst_queue, src_queue, sizeof(*dst_queue));
    return OK_BLK_SHARED_DATA_QUEUE;
}

