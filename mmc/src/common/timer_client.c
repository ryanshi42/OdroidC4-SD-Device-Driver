#include "timer_client.h"


result_t timer_client_init(
        timer_client_t *timer_client,
        uintptr_t shared_dma,
        uintptr_t rx_avail_ring_buf,
        uintptr_t rx_used_ring_buf
) {
    if (timer_client == NULL) {
        return result_err("NULL `timer_client` passed to timer_client_init().");
    }
    /* Initialise our `rx_ring_buf_handle`, which is just a convenience struct
     * where all relevant Receive ring buffers are located. */
    ring_init(
            &timer_client->rx_ring_buf_handle,
            (ring_buffer_t *) rx_avail_ring_buf,
            (ring_buffer_t *) rx_used_ring_buf,
            NULL,
            1
    );
    /* Populate the available ring buffer with empty buffers from `shared_dma`
     * memory region. */
    for (int i = 0; i < NUM_BUFFERS - 1; i++) {
        int ret_enqueue_avail = enqueue_avail(
                &timer_client->rx_ring_buf_handle, /* Enqueuing Receive buffers
                onto the Receive buffer handle. */
                shared_dma + (BUF_SIZE * (i + NUM_BUFFERS)), /* Note that we are
                using the buffers in the data region located after the ones we
                used for the Transmit side earlier. */
                BUF_SIZE,
                NULL
        );
        if (ret_enqueue_avail < 0) {
            return result_err("Failed to enqueue buffer onto Receive available queue in timer_client_init().\n");
        }
    }
    return result_ok();
}
