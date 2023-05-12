#pragma once

/*
 * A Queue of Shared Data Buffers. The Queue is implemented as a Ring Buffer.
 */

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include "blk_shared_data_buf.h"
#include "fence.h"

#define MAX_NUM_BLK_SHARED_DATA_BUFS (512)

typedef struct blk_shared_data_queue blk_shared_data_queue_t;
struct blk_shared_data_queue {
    uintptr_t data_region;
    size_t data_region_size;
    size_t data_buf_size;
    size_t num_data_bufs;
    blk_shared_data_buf_t data_bufs[MAX_NUM_BLK_SHARED_DATA_BUFS];
    size_t num_unused_slots; /* This is the number of unused slots in
    `data_bufs` and its value is simply (MAX_NUM_BLK_SHARED_DATA_BUFS -
    queue->num_data_bufs). We precompute the number of unused slots to avoid
    recomputing it every time we need to check if the ring buffer is full or
    not. */
    size_t head_idx; /* We dequeue from the head. This is also known as the
    "read index". */
    size_t tail_idx; /* We enqueue onto the tail. This is also known as the
    "write index". */
};

/* Possible return values from the following functions. */
enum blk_shared_data_queue_result {
    OK_BLK_SHARED_DATA_QUEUE = 0,
    ERR_INCREASE_MAX_NUM_BLK_SHARED_DATA_BUFS = -1,
    ERR_NULL_BLK_SHARED_DATA_QUEUE = -2,
    ERR_BLK_SHARED_DATA_BUF_SIZE_TOO_LARGE = -3,
    ERR_BLK_SHARED_DATA_BUF_SIZE_MISALIGNED = -4,
    ERR_INVALID_BLK_SHARED_DATA_REGION = -5,
    ERR_NULL_BLK_SHARED_DATA_BUF_VAL = -6,
    ERR_BLK_SHARED_DATA_QUEUE_FULL = -7,
    ERR_BLK_SHARED_DATA_QUEUE_EMPTY = -8,
    ERR_NULL_RET_VAL_PTR_PASSED_TO_BLK_SHARED_DATA_QUEUE_FN = -9, /* A NULL
    `ret_val` pointer was passed into a `blk_shared_data_queue` function. */
};
typedef enum blk_shared_data_queue_result blk_shared_data_queue_result_t;

/**
 * Initializes a Shared Data Ring Buffer.
 * @param queue
 * @param shared_data_region_vaddr Virtual address of shared data region.
 * @param shared_data_region_size Size of shared data region in bytes.
 * @param shared_data_buf_size Size of each shared data buffer in bytes.
 * @return 0 on success, -1 on failure.
 */
blk_shared_data_queue_result_t blk_shared_data_queue_init(
        blk_shared_data_queue_t *queue,
        uintptr_t shared_data_region_vaddr,
        size_t shared_data_region_size,
        size_t shared_data_buf_size
);

/**
 * Returns the maximum number of buffers this ring buffer holds.
 * @param queue
 * @param ret_val The number of buffers this ring buffer holds.
 * @return
 */
blk_shared_data_queue_result_t blk_shared_data_queue_capacity(
        blk_shared_data_queue_t *queue,
        size_t *ret_val
);

/**
 * Returns the number of buffers this ring buffer currently holds.
 * @param queue
 * @param ret_val
 * @return
 */
blk_shared_data_queue_result_t blk_shared_data_queue_size(
        blk_shared_data_queue_t *queue,
        size_t *ret_val
);

/**
 * Returns True if the ring buffer is empty, False otherwise.
 * @param queue
 * @param ret_val
 * @return
 */
blk_shared_data_queue_result_t blk_shared_data_queue_is_empty(
        blk_shared_data_queue_t *queue,
        bool *ret_val
);

/**
 * Returns True if the ring buffer is full, False otherwise.
 * @param queue
 * @param ret_val
 * @return
 */
blk_shared_data_queue_result_t blk_shared_data_queue_is_full(
        blk_shared_data_queue_t *queue,
        bool *ret_val
);

/**
 * Enqueues the `blk_shared_data_buf` onto the ring buffer.
 * @param queue
 * @param val
 * @return
 */
blk_shared_data_queue_result_t blk_shared_data_queue_enqueue(
        blk_shared_data_queue_t *queue,
        blk_shared_data_buf_t *val
);

/**
 * Dequeues the `blk_shared_data_buf` from the ring buffer.
 * @param queue
 * @param ret_val The dequeued `blk_shared_data_buf` is saved to `ret_val`.
 * @return
 */
blk_shared_data_queue_result_t blk_shared_data_queue_dequeue(
        blk_shared_data_queue_t *queue,
        blk_shared_data_buf_t *ret_val
);

/**
 * Copies the contents of `src_queue` into `dst_queue`.
 * @param dst_queue
 * @param src_queue
 */
blk_shared_data_queue_result_t blk_shared_data_queue_copy(
        blk_shared_data_queue_t *dst_queue,
        blk_shared_data_queue_t *src_queue
);
