#include "oc4_timer.h"

result_t oc4_timer_init(
        oc4_timer_t *oc4_timer,
        uintptr_t base_vaddr
) {
    if (oc4_timer == NULL) {
        return result_err("NULL `oc4_timer` passed to oc4_timer_init().");
    }
    if (base_vaddr == 0) {
        return result_err("NULL `base_vaddr` passed to oc4_timer_init().");
    }
    oc4_timer->regs = oc4_timer_regs_get(base_vaddr);
    return result_ok();
}

result_t oc4_timer_get_num_ticks(
        oc4_timer_t *oc4_timer,
        uint64_t *ret_val
) {
    if (oc4_timer == NULL) {
        return result_err("NULL `oc4_timer` passed to oc4_timer_get_ticks().");
    }
    if (ret_val == NULL) {
        return result_err("NULL `ret_val` passed to oc4_timer_get_ticks().");
    }
    uint32_t counter_lo;
    uint32_t counter_hi;
    uint32_t counter_hi_tmp;
    do {
        result_t res = oc4_timer_regs_get_counter_lo(oc4_timer->regs, &counter_lo);
        if (result_is_err(res)) {
            return result_err("Failed to get `counter_lo` in oc4_timer_get_ticks().");
        }
        res = oc4_timer_regs_get_counter_hi(oc4_timer->regs, &counter_hi);
        if (result_is_err(res)) {
            return result_err("Failed to get `counter_hi` in oc4_timer_get_ticks().");
        }
        res = oc4_timer_regs_get_counter_hi(oc4_timer->regs, &counter_hi_tmp);
        if (result_is_err(res)) {
            return result_err("Failed to get `counter_hi_tmp` in oc4_timer_get_ticks().");
        }
    } while (counter_hi_tmp != counter_hi); /* Check system timer hasn't ticked over in that time. */
    /* Join the two 32-bit halves into a single 64-bit integer. */
    *ret_val = ((uint64_t) counter_hi << 32) | counter_lo;
    return result_ok();
}

