#pragma once

/* Request Ring Buffer. */

#include <stddef.h>
#include "blk_req_buf.h"

#define NUM_BUFFERS (512)

typedef struct blk_req_ring_buf blk_req_ring_buf_t;
struct blk_req_ring_buf {
    size_t head;
    size_t tail;
    blk_req_buf_t buffers[NUM_BUFFERS];
};

/**
 * Initializes a Request Ring Buffer.
 * @param ring_buf
 * @return
 */
int blk_req_ring_buf_init(
        blk_req_ring_buf_t *ring_buf
);

/**
 * Enqueues a Request onto the Request Ring Buffer.
 * @param ring_buf
 * @param req The Request to enqueue.
 * @return
 */
int blk_req_ring_buf_enqueue(
        blk_req_ring_buf_t *ring_buf,
        blk_req_buf_t *req
);

/**
 * Dequeues a Request from the Request Ring Buffer.
 * @param ring_buf
 * @param ret_val The dequeued Request.
 * @return
 */
int blk_req_ring_buf_dequeue(
        blk_req_ring_buf_t *ring_buf,
        blk_req_buf_t *ret_val
);


