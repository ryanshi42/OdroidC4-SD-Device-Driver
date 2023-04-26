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
    ring_buf->data_region = (uint8_t *) shared_data_region_vaddr;
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
    /* We precompute the number of empty slots to avoid re-computing this value
     * every time we need to check if the ring buffer is full or not for
     * performance reasons. */
    ring_buf->num_empty_slots = (MAX_NUM_BLK_DATA_BUFS - ring_buf->num_data_bufs);
    /* Initialise the head and tail index to empty. */
    ring_buf->head_idx = 0;
    ring_buf->tail_idx = 0;
    /* TODO: Enqueue all shared buffers onto `data_bufs`. */
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
    *ret_val = (ring_buf->head_idx == (ring_buf->tail_idx + ring_buf->num_empty_slots) % MAX_NUM_BLK_DATA_BUFS);
    return OK_BLK_DATA_RING_BUF;
}
