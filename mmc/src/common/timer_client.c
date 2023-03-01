#include "timer_client.h"


result_t timer_client_init(
        timer_client_t *timer_client,
        uintptr_t shared_dma,
        uintptr_t rx_avail_ring_buf,
        uintptr_t rx_used_ring_buf,
        size_t sel4cp_channel_id_get_num_ticks
) {
    if (timer_client == NULL) {
        return result_err("NULL `timer_client` passed to timer_client_init().");
    }
    /* Save the sel4cp channel ID. */
    timer_client->sel4cp_channel_id_get_num_ticks = sel4cp_channel_id_get_num_ticks;
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


result_t timer_client_get_num_ticks(
        timer_client_t *timer_client,
        uint64_t *ret_val
) {
    if (timer_client == NULL) {
        return result_err("NULL `timer_client` passed to timer_client_get_num_ticks().");
    }
    /* Send a notification to `timer_driver`. */
    sel4cp_notify(timer_client->sel4cp_channel_id_get_num_ticks);

    uintptr_t buf_addr = 0; /* The dequeued buffer's address will be stored in `buf_addr`. */
    unsigned int buf_len = 0; /* The dequeued buffer's length will be stored in `buf_len`. */
    /* We don't use the `cookie` but the `driver_dequeue` function call requires
     * a valid pointer for the `cookie` param, so we provide one to it anyway. */
    void *unused_cookie = NULL;
    /* Keep trying to dequeue until there is something available on the
     * Receive-Used queue. */
    while (driver_dequeue(
            timer_client->rx_ring_buf_handle.used_ring,
            &buf_addr,
            &buf_len,
            &unused_cookie
    ) != 0) {}
    if (buf_len != sizeof(uint64_t)) {
        return result_err("Received buffer from `timer_driver` is not the expected size in timer_client_get_num_ticks().");
    }
    /* Set the return value to the data in `buf_addr`. */
    *ret_val = *((uint64_t *) buf_addr);
    /* Return buffer back to the transmit-avail queue for the `timer_driver` PD
     * to use. */
    int ret_enqueue_avail = enqueue_avail(
            &timer_client->rx_ring_buf_handle,
            buf_addr,
            BUF_SIZE,
            NULL
    );
    if (ret_enqueue_avail < 0) {
        return result_err("Failed to enqueue buffer onto available queue in timer_client_get_num_ticks().\n");
    }
    return result_ok();
}
