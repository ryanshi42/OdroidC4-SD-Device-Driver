#pragma once

/*
 * A Queue of Requests. The Queue is implemented as a Ring Buffer.
 */

#include <string.h>
#include <stddef.h>
#include <stdbool.h>

#include "fence.h"
#include "blk_request.h"

/* We've specified `MAX_NUM_BLK_REQUESTS` assuming the memory region has a size
 * of 0x200000 bytes. 0x200000 is the maximum page size in seL4 and is therefore
 * the largest chunk of contiguous memory you can reserve in seL4CP. */
#define MAX_NUM_BLK_REQUESTS (52428)

typedef struct blk_request_queue blk_request_queue_t;
struct blk_request_queue {
    size_t head_idx; /* We dequeue from the head. This is also known as the
    "read index". */
    size_t tail_idx; /* We enqueue onto the tail. This is also known as the
    "write index". */
    blk_request_t requests[MAX_NUM_BLK_REQUESTS];
};

enum blk_request_queue_result {
    OK_BLK_REQUEST_QUEUE = 0,
    ERR_NULL_BLK_REQUEST_QUEUE = -1, /* A NULL `queue` pointer was passed in. */
    ERR_BLK_REQUEST_QUEUE_REGION_TOO_SMALL = -2, /* The shared memory region
    reserved for this ring buffer is too small. To remedy this, you can either
    increase the shared memory region reserved for the `blk_request
    structure or reduce the size of `MAX_NUM_BLK_REQUESTS`. */
    ERR_BLK_REQUEST_QUEUE_REGION_TOO_LARGE = -3, /* The shared memory region
    reserved for this ring buffer is wastefully large. To remedy this, you can
    either reduce the shared memory region reserved for the `blk_request
    or increase the size of `MAX_NUM_BLK_REQUESTS` so that it uses up as much of
    the memory region as possible. */
    ERR_NULL_RET_VAL_PTR_PASSED_TO_BLK_REQUEST_QUEUE_FN = -4, /* A NULL `ret_val` pointer
    was passed into a `blk_request_queue` function. */
    ERR_NULL_BLK_REQUEST_IN_BLK_REQUEST_QUEUE = -5, /* A NULL `request
    blk_request_queue_enqueue */
    ERR_BLK_REQUEST_QUEUE_FULL = -6, /* The ring buffer is full. */
    ERR_BLK_REQUEST_QUEUE_EMPTY = -7, /* The ring buffer is empty. */
};
typedef enum blk_request_queue_result blk_request_queue_result_t;

/**
 * Initializes a Request Ring Buffer.
 * @param queue
 * @param request_queue_region_size Size of the shared memory region reserved for
 * this Request Ring Buffer in bytes.
 * @return
 */
blk_request_queue_result_t blk_request_queue_init(
        blk_request_queue_t *queue,
        size_t request_queue_region_size
);

/**
 * Returns the number of Requests that can be enqueued onto the Request Ring
 * Buffer.
 * @param queue
 * @param ret_val
 * @return
 */
blk_request_queue_result_t blk_request_queue_capacity(
        blk_request_queue_t *queue,
        size_t *ret_val
);

/**
 * Returns the number of Requests that are currently enqueued onto the Request
 * Ring Buffer.
 * @param queue
 * @param ret_val
 * @return
 */
blk_request_queue_result_t blk_request_queue_size(
        blk_request_queue_t *queue,
        size_t *ret_val
);

/**
 * Enqueues a Request onto the Request Ring Buffer.
 * @param queue
 * @param req The Request to enqueue.
 * @return
 */
blk_request_queue_result_t blk_request_queue_enqueue(
        blk_request_queue_t *queue,
        blk_request_t *req
);

/**
 * Dequeues a Request from the Request Ring Buffer.
 * @param queue
 * @param ret_val The dequeued Request.
 * @return
 */
blk_request_queue_result_t blk_request_queue_dequeue(
        blk_request_queue_t *queue,
        blk_request_t *ret_val
);


/**
 * Returns True if the ring buffer is empty, False otherwise.
 * @param queue
 * @param ret_val
 * @return
 */
blk_request_queue_result_t blk_request_queue_is_empty(
        blk_request_queue_t *queue,
        bool *ret_val
);

/**
 * Returns True if the ring buffer is full, False otherwise.
 * @param queue
 * @param ret_val
 * @return
 */
blk_request_queue_result_t blk_request_queue_is_full(
        blk_request_queue_t *queue,
        bool *ret_val
);

/**
 * Enqueues the `blk_request` onto the ring buffer.
 * @param queue
 * @param val
 * @return
 */
blk_request_queue_result_t blk_request_queue_enqueue(
        blk_request_queue_t *queue,
        blk_request_t *val
);

/**
 * Dequeues the `blk_request` from the ring buffer.
 * @param queue
 * @param ret_val
 * @return
 */
blk_request_queue_result_t blk_request_queue_dequeue(
        blk_request_queue_t *queue,
        blk_request_t *ret_val
);


