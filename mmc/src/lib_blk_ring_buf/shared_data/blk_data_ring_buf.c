#include "blk_data_ring_buf.h"

int blk_data_ring_buf_init(
        blk_data_ring_buf_t *ring_buf,
        uintptr_t shared_data_region_vaddr,
        size_t shared_data_region_size,
        size_t shared_data_buf_size
) {
    if (ring_buf == NULL ||
        shared_data_buf_size > shared_data_region_size ||
        shared_data_region_size % shared_data_buf_size != 0 ||
        shared_data_region_vaddr == 0) {
        return -1;
    }
    /* Empty the `ring_buf` data structure. */
    memset(ring_buf, 0, sizeof(*ring_buf));
    ring_buf->data_region = (uint8_t *) shared_data_region_vaddr;
    ring_buf->data_region_size = shared_data_region_size;
    ring_buf->data_buf_size = shared_data_buf_size;
    ring_buf->num_data_bufs = (shared_data_region_size / shared_data_buf_size);



    return 0;
}

int blk_data_ring_buf_capacity(
        blk_data_ring_buf_t *ring_buf,
        size_t *ret_val
) {
    if (ring_buf == NULL) {
        return -1;
    }
    *ret_val = ring_buf->num_data_bufs;
    return 0;
}
