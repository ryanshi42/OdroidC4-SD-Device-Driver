#pragma once

#include <stdint.h>

#include "result.h"
#include "bcm_timer.h"
#include "shared_ringbuffer.h"
#include "sel4cp_facade.h"
#include "printf.h"

#define TIMER_DRIVER_TO_SERIAL_CLIENT_PUTCHAR_CHANNEL (9)

typedef struct timer_driver timer_driver_t;
struct timer_driver {
    bcm_timer_t bcm_timer;
    /* Receive ring buffer handle. This is a convenience `struct` that
     * contains all the pointers to the relevant Receive "available" and "used"
     * buffers. */
    ring_handle_t rx_ring_buf_handle;
};

/**
 * Initialise our `timer_driver`.
 * @param timer_driver
 * @param base_vaddr Base virtual address of the memory-mapped timer registers.
 */
result_t timer_driver_init(timer_driver_t *timer_driver, uintptr_t base_vaddr);