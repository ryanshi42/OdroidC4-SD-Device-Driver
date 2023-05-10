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
    uintptr_t data_region;
    size_t data_region_size;
    size_t data_buf_size;
    size_t num_data_bufs;
    blk_data_buf_t data_bufs[MAX_NUM_BLK_DATA_BUFS];
    size_t num_unused_slots; /* This is the number of unused slots in
    `data_bufs` and its value is simply (MAX_NUM_BLK_DATA_BUFS -
    ring_buf->num_data_bufs). We precompute the number of unused slots to avoid
    recomputing it every time we need to check if the ring buffer is full or
    not. */
    size_t head_idx; /* We dequeue from the head. This is also known as the
    "read index". */
    size_t tail_idx; /* We enqueue onto the tail. This is also known as the
    "write index". */
};

/* Possible return values from the following functions. */
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
    ERR_BLK_DATA_RING_BUF_EMPTY = -9,
    ERR_NULL_RET_VAL_PTR_PASSED_TO_BLK_DATA_RING_BUF_FN = -10, /* A NULL
    `ret_val` pointer was passed into a `blk_data_ring_buf` function. */
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
 * Returns the number of buffers this ring buffer currently holds.
 * @param ring_buf
 * @param ret_val
 * @return
 */
blk_data_ring_buf_result_t blk_data_ring_buf_size(
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

