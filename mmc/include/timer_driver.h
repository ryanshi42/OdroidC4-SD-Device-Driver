#pragma once

#include <stdint.h>

#include "result.h"
#include "oc4_timer.h"
#include "shared_ringbuffer.h"
#include "sel4cp_facade.h"
#include "printf.h"

#define TIMER_DRIVER_TO_SERIAL_CLIENT_PUTCHAR_CHANNEL (9)
#define TIMER_DRIVER_TO_MMC_DRIVER_GET_NUM_TICKS_CHANNEL (12)

typedef struct timer_driver timer_driver_t;
struct timer_driver {
    /* The OC4 Timer Device. */
    oc4_timer_t oc4_timer;
};

/**
 * Initialise our `timer_driver`.
 * @param timer_driver
 * @param base_vaddr Base virtual address of the memory-mapped timer registers.
 */
result_t timer_driver_init(timer_driver_t *timer_driver, uintptr_t base_vaddr);