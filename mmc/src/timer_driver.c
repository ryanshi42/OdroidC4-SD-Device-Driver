#include "timer_driver.h"

uintptr_t timer_base_vaddr;

uintptr_t shared_dma;

/* `rx_` stands for "Receive". The following two Receive buffers are to assist
 * with transporting data from `timer_driver` to `timer_client`. */

/* In the SDDF's design-documentation, `rx_avail_ring_buf` is referred to as the
 * "Transmit-Free" (TxF) ring buffer. `rx_avail_ring_buf` holds all the buffers
 * in the `shared_dma` data region that are ready to be reused for transporting
 * new data from `timer_client` to `timer_driver`. */
uintptr_t rx_avail_ring_buf;

/* In the SDDF's design-documentation, `rx_used_ring_buf` is referred to as the
 * "Transmit-Available" (TxA) ring buffer. `rx_used_ring_buf` holds all the
 * buffers in the `shared_dma` data region that currently hold data sent by
 * `timer_client` for the `timer_driver` PD to process.  */
uintptr_t rx_used_ring_buf;

/* Global `timer_driver`. */
timer_driver_t global_timer_driver = {0};

result_t timer_driver_init(timer_driver_t *timer_driver, uintptr_t base_vaddr) {
    /* Initialise the timer. */
    result_t res = bcm_timer_init(
            &timer_driver->bcm_timer,
            base_vaddr
    );
    if (result_is_err(res)) {
        return result_ok_or(res, "Failed to initialise timer in timer_driver_init().");
    }
    /* Initialise our `rx_ring_buf_handle`, which is just a convenience struct
     * where all relevant Receive ring buffers are located. */
    ring_init(
            &timer_driver->rx_ring_buf_handle,
            (ring_buffer_t *) rx_avail_ring_buf,
            (ring_buffer_t *) rx_used_ring_buf,
            NULL,
            0
    );
    return result_ok();
}

void init(void) {
    result_t res = timer_driver_init(
            &global_timer_driver,
            timer_base_vaddr
    );
    if (result_is_err(res)) {
        /* TODO: Print the error out. */
        return;
    }
}

seL4_MessageInfo_t protected(sel4cp_channel ch, sel4cp_msginfo msginfo) {
    return sel4cp_msginfo_new(0, 0);
}

void notified(sel4cp_channel channel) {

}
