#pragma once

/*
 * A Queue of Responses. The Queue is implemented as a Ring Buffer.
 */

#include <string.h>
#include <stddef.h>
#include <stdbool.h>

#include "fence.h"
#include "blk_response.h"

/* We've specified `MAX_NUM_BLK_RESPONSES` assuming the memory region has a size
 * of 0x200000 bytes. 0x200000 is the maximum page size in seL4 and is therefore
 * the largest chunk of contiguous memory you can reserve in seL4CP. */
#define MAX_NUM_BLK_RESPONSES (65535)

typedef struct blk_response_queue blk_response_queue_t;
struct blk_response_queue {
    size_t head_idx; /* We dequeue from the head. This is also known as the
    "read index". */
    size_t tail_idx; /* We enqueue onto the tail. This is also known as the
    "write index". */
    blk_response_t responses[MAX_NUM_BLK_RESPONSES];
};

enum blk_response_queue_result {
    OK_BLK_RESPONSE_QUEUE = 0,
    ERR_NULL_BLK_RESPONSE_QUEUE = -1, /* A NULL `queue` pointer was passed in. */
    ERR_BLK_RESPONSE_QUEUE_REGION_TOO_SMALL = -2, /* The shared memory region
    reserved for this ring buffer is too small. To remedy this, you can either
    increase the shared memory region reserved for the `blk_response
    structure or reduce the size of `MAX_NUM_BLK_RESPONSES`. */
    ERR_BLK_RESPONSE_QUEUE_REGION_TOO_LARGE = -3, /* The shared memory region
    reserved for this ring buffer is wastefully large. To remedy this, you can
    either reduce the shared memory region reserved for the `blk_response
    or increase the size of `MAX_NUM_BLK_RESPONSES`. */
    ERR_NULL_RET_VAL_PTR_PASSED_TO_BLK_RESPONSE_QUEUE_FN = -4, /* A NULL `ret_val` pointer
    was passed into a `blk_response_queue` function. */
    ERR_NULL_BLK_RESPONSE = -5, /* A NULL `request
    blk_response_queue_enqueue */
    ERR_BLK_RESPONSE_QUEUE_FULL = -6, /* The ring buffer is full. */
    ERR_BLK_RESPONSE_QUEUE_EMPTY = -7, /* The ring buffer is empty. */
};
typedef enum blk_response_queue_result blk_response_queue_result_t;

/**
 * Initializes a Request Ring Buffer.
 * @param queue
 * @param response_queue_region_size Size of the shared memory region reserved for
 * this Request Ring Buffer in bytes.
 * @return
 */
blk_response_queue_result_t blk_response_queue_init(
        blk_response_queue_t *queue,
        size_t response_queue_region_size
);

/**
 * Returns the number of Requests that can be enqueued onto the Request Ring
 * Buffer.
 * @param queue
 * @param ret_val
 * @return
 */
blk_response_queue_result_t blk_response_queue_capacity(
        blk_response_queue_t *queue,
        size_t *ret_val
);

/**
 * Returns the number of Requests that are currently enqueued onto the Request
 * Ring Buffer.
 * @param queue
 * @param ret_val
 * @return
 */
blk_response_queue_result_t blk_response_queue_size(
        blk_response_queue_t *queue,
        size_t *ret_val
);

/**
 * Enqueues a Request onto the Request Ring Buffer.
 * @param queue
 * @param req The Request to enqueue.
 * @return
 */
blk_response_queue_result_t blk_response_queue_enqueue(
        blk_response_queue_t *queue,
        blk_response_t *req
);

/**
 * Dequeues a Request from the Request Ring Buffer.
 * @param queue
 * @param ret_val The dequeued Request.
 * @return
 */
blk_response_queue_result_t blk_response_queue_dequeue(
        blk_response_queue_t *queue,
        blk_response_t *ret_val
);


/**
 * Returns True if the ring buffer is empty, False otherwise.
 * @param queue
 * @param ret_val
 * @return
 */
blk_response_queue_result_t blk_response_queue_is_empty(
        blk_response_queue_t *queue,
        bool *ret_val
);

/**
 * Returns True if the ring buffer is full, False otherwise.
 * @param queue
 * @param ret_val
 * @return
 */
blk_response_queue_result_t blk_response_queue_is_full(
        blk_response_queue_t *queue,
        bool *ret_val
);

/**
 * Enqueues the `blk_response` onto the ring buffer.
 * @param queue
 * @param val
 * @return
 */
blk_response_queue_result_t blk_response_queue_enqueue(
        blk_response_queue_t *queue,
        blk_response_t *val
);

/**
 * Dequeues the `blk_response` from the ring buffer.
 * @param queue
 * @param ret_val
 * @return
 */
blk_response_queue_result_t blk_response_queue_dequeue(
        blk_response_queue_t *queue,
        blk_response_t *ret_val
);



