#pragma once

#include <stddef.h>
#include <stdint.h>
#include <string.h>

typedef struct blk_data_ring_buf blk_data_ring_buf_t;
struct blk_data_ring_buf {
    size_t head;
    size_t tail;
    uint8_t *data_region;
    size_t data_region_size;
    size_t data_buf_size;
    size_t num_data_bufs;
};

/**
 * Initializes a Shared Data Ring Buffer.
 * @param ring_buf
 * @param shared_data_region_vaddr Virtual address of shared data region.
 * @param shared_data_region_size Size of shared data region in bytes.
 * @param shared_data_buf_size Size of each shared data buffer in bytes.
 * @return 0 on success, -1 on failure.
 */
int blk_data_ring_buf_init(
        blk_data_ring_buf_t *ring_buf,
        uintptr_t shared_data_region_vaddr,
        size_t shared_data_region_size,
        size_t shared_data_buf_size
);

/**
 * Returns the maximum number of buffers this ring buffer holds.
 * @param ring_buf
 * @param ret_val The number of buffers this ring buffer holds.
 * @return
 */
int blk_data_ring_buf_capacity(
        blk_data_ring_buf_t *ring_buf,
        size_t *ret_val
);