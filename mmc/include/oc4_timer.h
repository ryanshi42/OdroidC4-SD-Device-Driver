#pragma once

#include "result.h"
#include "oc4_timer_regs.h"

typedef struct oc4_timer oc4_timer_t;
struct oc4_timer {
    oc4_timer_regs_t *regs;
};

/**
 * Initialises the oc4 timer device.
 * @param oc4_timer
 * @param base_vaddr
 * @return
 */
result_t oc4_timer_init(
        oc4_timer_t *oc4_timer,
        uintptr_t base_vaddr
);

/**
 * Gets the number of ticks since the oc4 timer was initialised.
 * @param oc4_timer
 * @param ret_val
 * @return
 */
result_t oc4_timer_get_num_ticks(
        oc4_timer_t *oc4_timer,
        uint64_t *ret_val
);
