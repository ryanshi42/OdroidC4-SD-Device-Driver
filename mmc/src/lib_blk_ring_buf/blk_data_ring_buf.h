#pragma once

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include "blk_data_buf.h"
#include "fence.h"

#define MAX_NUM_BLK_DATA_BUFS (512)

typedef struct blk_data_ring_buf blk_data_ring_buf_t;
struct blk_data_ring_buf {
    uint8_t *data_region;
    size_t data_region_size;
    size_t data_buf_size;
    size_t num_data_bufs;
    blk_data_buf_t data_bufs[MAX_NUM_BLK_DATA_BUFS];
    size_t num_empty_slots;
    size_t head_idx;
    size_t tail_idx;
};

/* Possible responses from the following functions. */
enum blk_data_ring_buf_result {
    OK_BLK_DATA_RING_BUF = 0,
    ERR_INCREASE_MAX_NUM_BLK_DATA_BUFS = -1,
    ERR_NULL_BLK_DATA_RING_BUF = -2,
    ERR_BLK_DATA_BUF_SIZE_TOO_LARGE = -3,
    ERR_BLK_DATA_BUF_SIZE_MISALIGNED = -4,
    ERR_INVALID_BLK_DATA_REGION = -5,
    ERR_NULL_BLK_DATA_BUF_VAL = -6,
    ERR_NULL_BLK_DATA_BUF_RET_VAL = -7,
    ERR_BLK_DATA_RING_BUF_FULL = -8,
};
typedef enum blk_data_ring_buf_result blk_data_ring_buf_result_t;

/**
 * Initializes a Shared Data Ring Buffer.
 * @param ring_buf
 * @param shared_data_region_vaddr Virtual address of shared data region.
 * @param shared_data_region_size Size of shared data region in bytes.
 * @param shared_data_buf_size Size of each shared data buffer in bytes.
 * @return 0 on success, -1 on failure.
 */
blk_data_ring_buf_result_t blk_data_ring_buf_init(
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
blk_data_ring_buf_result_t blk_data_ring_buf_capacity(
        blk_data_ring_buf_t *ring_buf,
        size_t *ret_val
);

/**
 * Returns True if the ring buffer is empty, False otherwise.
 * @param ring_buf
 * @param ret_val
 * @return
 */
blk_data_ring_buf_result_t blk_data_ring_buf_is_empty(
        blk_data_ring_buf_t *ring_buf,
        bool *ret_val
);

/**
 * Returns True if the ring buffer is full, False otherwise.
 * @param ring_buf
 * @param ret_val
 * @return
 */
blk_data_ring_buf_result_t blk_data_ring_buf_is_full(
        blk_data_ring_buf_t *ring_buf,
        bool *ret_val
);

/**
 * Enqueues the `blk_data_buf` onto the ring buffer.
 * @param ring_buf
 * @param val
 * @return
 */
blk_data_ring_buf_result_t blk_data_ring_buf_enqueue(
        blk_data_ring_buf_t *ring_buf,
        blk_data_buf_t *val
);

/**
 * Dequeues the `blk_data_buf` from the ring buffer.
 * @param ring_buf
 * @param ret_val The dequeued `blk_data_buf` is saved to `ret_val`.
 * @return
 */
blk_data_ring_buf_result_t blk_data_ring_buf_dequeue(
        blk_data_ring_buf_t *ring_buf,
        blk_data_buf_t *ret_val
);

