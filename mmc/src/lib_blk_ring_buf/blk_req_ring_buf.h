#pragma once

/* Request Ring Buffer. */

#include <string.h>
#include <stddef.h>
#include <stdbool.h>
#include "fence.h"
#include "blk_req_buf.h"

#define MAX_NUM_BLK_REQ_BUFS (512)

typedef struct blk_req_ring_buf blk_req_ring_buf_t;
struct blk_req_ring_buf {
    size_t head_idx; /* We dequeue from the head. This is also known as the
    "read index". */
    size_t tail_idx; /* We enqueue onto the tail. This is also known as the
    "write index". */
    blk_req_buf_t req_bufs[MAX_NUM_BLK_REQ_BUFS];
};

enum blk_req_ring_buf_result {
    OK_BLK_REQ_RING_BUF = 0,
    ERR_NULL_BLK_REQ_RING_BUF = -1, /* A NULL `ring_buf` pointer was passed in. */
    ERR_BLK_REQ_RING_BUF_REGION_TOO_SMALL = -2, /* The shared memory region
    reserved for this ring buffer is too small. To remedy this, you can either
    increase the shared memory region reserved for the `blk_req_ring_buf` data
    structure or reduce the size of `MAX_NUM_BLK_REQ_BUFS`. */
    ERR_BLK_REQ_RING_BUF_REGION_TOO_LARGE = -3, /* The shared memory region
    reserved for this ring buffer is wastefully large. To remedy this, you can
    either reduce the shared memory region reserved for the `blk_req_ring_buf`
    or increase the size of `MAX_NUM_BLK_REQ_BUFS`. */
    ERR_NULL_RET_VAL_PTR_PASSED_TO_BLK_REQ_RING_BUF_FN = -4, /* A NULL `ret_val` pointer
    was passed into a `blk_req_ring_buf` function. */
    ERR_NULL_BLK_REQ_BUF = -5, /* A NULL `req_buf` pointer was passed into
    blk_req_ring_buf_enqueue */
    ERR_BLK_REQ_RING_BUF_FULL = -6, /* The ring buffer is full. */
    ERR_BLK_REQ_RING_BUF_EMPTY = -7, /* The ring buffer is empty. */
};
typedef enum blk_req_ring_buf_result blk_req_ring_buf_result_t;

/**
 * Initializes a Request Ring Buffer.
 * @param ring_buf
 * @param req_ring_buf_region_size Size of the shared memory region reserved for
 * this Request Ring Buffer in bytes.
 * @return
 */
blk_req_ring_buf_result_t blk_req_ring_buf_init(
        blk_req_ring_buf_t *ring_buf,
        size_t req_ring_buf_region_size
);

/**
 * Returns the number of Requests that can be enqueued onto the Request Ring
 * Buffer.
 * @param ring_buf
 * @param ret_val
 * @return
 */
blk_req_ring_buf_result_t blk_req_ring_buf_capacity(
        blk_req_ring_buf_t *ring_buf,
        size_t *ret_val
);

/**
 * Returns the number of Requests that are currently enqueued onto the Request
 * Ring Buffer.
 * @param ring_buf
 * @param ret_val
 * @return
 */
blk_req_ring_buf_result_t blk_req_ring_buf_size(
        blk_req_ring_buf_t *ring_buf,
        size_t *ret_val
);

/**
 * Enqueues a Request onto the Request Ring Buffer.
 * @param ring_buf
 * @param req The Request to enqueue.
 * @return
 */
blk_req_ring_buf_result_t blk_req_ring_buf_enqueue(
        blk_req_ring_buf_t *ring_buf,
        blk_req_buf_t *req
);

/**
 * Dequeues a Request from the Request Ring Buffer.
 * @param ring_buf
 * @param ret_val The dequeued Request.
 * @return
 */
blk_req_ring_buf_result_t blk_req_ring_buf_dequeue(
        blk_req_ring_buf_t *ring_buf,
        blk_req_buf_t *ret_val
);


/**
 * Returns True if the ring buffer is empty, False otherwise.
 * @param ring_buf
 * @param ret_val
 * @return
 */
blk_req_ring_buf_result_t blk_req_ring_buf_is_empty(
        blk_req_ring_buf_t *ring_buf,
        bool *ret_val
);

/**
 * Returns True if the ring buffer is full, False otherwise.
 * @param ring_buf
 * @param ret_val
 * @return
 */
blk_req_ring_buf_result_t blk_req_ring_buf_is_full(
        blk_req_ring_buf_t *ring_buf,
        bool *ret_val
);

/**
 * Enqueues the `blk_req_buf` onto the ring buffer.
 * @param ring_buf
 * @param val
 * @return
 */
blk_req_ring_buf_result_t blk_req_ring_buf_enqueue(
        blk_req_ring_buf_t *ring_buf,
        blk_req_buf_t *val
);

/**
 * Dequeues the `blk_req_buf` from the ring buffer.
 * @param ring_buf
 * @param ret_val
 * @return
 */
blk_req_ring_buf_result_t blk_req_ring_buf_dequeue(
        blk_req_ring_buf_t *ring_buf,
        blk_req_buf_t *ret_val
);


