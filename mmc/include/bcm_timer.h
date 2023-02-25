#pragma once

#include "result.h"
#include "bcm_timer_regs.h"

typedef struct bcm_timer bcm_timer_t;
struct bcm_timer {
    bcm_timer_regs_t *regs;
};

/**
 * Initialises the BCM timer device.
 * @param bcm_timer
 * @param base_vaddr
 * @return
 */
result_t bcm_timer_init(
        bcm_timer_t *bcm_timer,
        uintptr_t base_vaddr
);

