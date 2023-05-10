#pragma once

/*
 * A queue of Requests. The queue is implemented as a ring buffer.
 */

#include <string.h>
#include <stddef.h>
#include <stdbool.h>

#include "fence.h"
#include "blk_req.h"

#define MAX_NUM_BLK_REQUESTS (512)

typedef struct blk_req_queue blk_req_queue_t;
struct blk_req_queue {
    size_t head_idx; /* We dequeue from the head. This is also known as the
    "read index". */
    size_t tail_idx; /* We enqueue onto the tail. This is also known as the
    "write index". */
    blk_req_t req_bufs[MAX_NUM_BLK_REQUESTS];
};

enum blk_req_queue_result {
    OK_BLK_REQ_QUEUE = 0,
    ERR_NULL_BLK_REQ_QUEUE = -1, /* A NULL `queue` pointer was passed in. */
    ERR_BLK_REQ_QUEUE_REGION_TOO_SMALL = -2, /* The shared memory region
    reserved for this ring buffer is too small. To remedy this, you can either
    increase the shared memory region reserved for the `blk_req_queue` data
    structure or reduce the size of `MAX_NUM_BLK_REQUESTS`. */
    ERR_BLK_REQ_QUEUE_REGION_TOO_LARGE = -3, /* The shared memory region
    reserved for this ring buffer is wastefully large. To remedy this, you can
    either reduce the shared memory region reserved for the `blk_req_queue`
    or increase the size of `MAX_NUM_BLK_REQUESTS`. */
    ERR_NULL_RET_VAL_PTR_PASSED_TO_BLK_REQ_QUEUE_FN = -4, /* A NULL `ret_val` pointer
    was passed into a `blk_req_queue` function. */
    ERR_NULL_BLK_REQ_BUF = -5, /* A NULL `req_buf` pointer was passed into
    blk_req_queue_enqueue */
    ERR_BLK_REQ_QUEUE_FULL = -6, /* The ring buffer is full. */
    ERR_BLK_REQ_QUEUE_EMPTY = -7, /* The ring buffer is empty. */
};
typedef enum blk_req_queue_result blk_req_queue_result_t;

/**
 * Initializes a Request Ring Buffer.
 * @param queue
 * @param req_queue_region_size Size of the shared memory region reserved for
 * this Request Ring Buffer in bytes.
 * @return
 */
blk_req_queue_result_t blk_req_queue_init(
        blk_req_queue_t *queue,
        size_t req_queue_region_size
);

/**
 * Returns the number of Requests that can be enqueued onto the Request Ring
 * Buffer.
 * @param queue
 * @param ret_val
 * @return
 */
blk_req_queue_result_t blk_req_queue_capacity(
        blk_req_queue_t *queue,
        size_t *ret_val
);

/**
 * Returns the number of Requests that are currently enqueued onto the Request
 * Ring Buffer.
 * @param queue
 * @param ret_val
 * @return
 */
blk_req_queue_result_t blk_req_queue_size(
        blk_req_queue_t *queue,
        size_t *ret_val
);

/**
 * Enqueues a Request onto the Request Ring Buffer.
 * @param queue
 * @param req The Request to enqueue.
 * @return
 */
blk_req_queue_result_t blk_req_queue_enqueue(
        blk_req_queue_t *queue,
        blk_req_t *req
);

/**
 * Dequeues a Request from the Request Ring Buffer.
 * @param queue
 * @param ret_val The dequeued Request.
 * @return
 */
blk_req_queue_result_t blk_req_queue_dequeue(
        blk_req_queue_t *queue,
        blk_req_t *ret_val
);


/**
 * Returns True if the ring buffer is empty, False otherwise.
 * @param queue
 * @param ret_val
 * @return
 */
blk_req_queue_result_t blk_req_queue_is_empty(
        blk_req_queue_t *queue,
        bool *ret_val
);

/**
 * Returns True if the ring buffer is full, False otherwise.
 * @param queue
 * @param ret_val
 * @return
 */
blk_req_queue_result_t blk_req_queue_is_full(
        blk_req_queue_t *queue,
        bool *ret_val
);

/**
 * Enqueues the `blk_req` onto the ring buffer.
 * @param queue
 * @param val
 * @return
 */
blk_req_queue_result_t blk_req_queue_enqueue(
        blk_req_queue_t *queue,
        blk_req_t *val
);

/**
 * Dequeues the `blk_req` from the ring buffer.
 * @param queue
 * @param ret_val
 * @return
 */
blk_req_queue_result_t blk_req_queue_dequeue(
        blk_req_queue_t *queue,
        blk_req_t *ret_val
);


