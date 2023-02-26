#pragma once

#include "shared_ringbuffer.h"
#include "shared_dma.h"
#include "result.h"

typedef struct timer_client timer_client_t;
struct timer_client {
    /* Receive ring buffer handle. This is a convenience `struct` that contains
     * all the pointers to the relevant Receive "available" and "used" buffers. */
    ring_handle_t rx_ring_buf_handle;
};

/**
 * Initialise our `timer_client`.
 * @param timer_client
 */
result_t timer_client_init(
        timer_client_t *timer_client,
        uintptr_t shared_dma,
        uintptr_t rx_avail_ring_buf,
        uintptr_t rx_used_ring_buf
);