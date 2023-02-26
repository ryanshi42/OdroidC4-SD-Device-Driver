#pragma once

#include "shared_ringbuffer.h"
#include "shared_dma.h"
#include "result.h"
#include "sel4cp_facade.h"

typedef struct timer_client timer_client_t;
struct timer_client {
    /* Receive ring buffer handle. This is a convenience `struct` that contains
     * all the pointers to the relevant Receive "available" and "used" buffers. */
    ring_handle_t rx_ring_buf_handle;
    /* sel4cp channel ID to send notifications to `timer_driver`. */
    size_t sel4cp_channel_id_get_num_ticks;
};

/**
 * Initialise our `timer_client`.
 * @param timer_client
 */
result_t timer_client_init(
        timer_client_t *timer_client,
        uintptr_t shared_dma,
        uintptr_t rx_avail_ring_buf,
        uintptr_t rx_used_ring_buf,
        size_t sel4cp_channel_id_get_num_ticks
);

/**
 * Get the number of ticks since the timer was initialised.
 * @param timer_client
 * @param ret_val
 * @return
 */
result_t timer_client_get_num_ticks(
        timer_client_t *timer_client,
        uint64_t *ret_val
);

