#include "bcm_timer.h"

result_t bcm_timer_init(
        bcm_timer_t *bcm_timer,
        uintptr_t base_vaddr
) {
    if (bcm_timer == NULL) {
        return result_err("NULL `bcm_timer` passed to bcm_timer_init().");
    }
    if (base_vaddr == 0) {
        return result_err("NULL `base_vaddr` passed to bcm_timer_init().");
    }
    bcm_timer->regs = bcm_timer_regs_get(base_vaddr);
    return result_ok();
}

result_t bcm_timer_get_num_ticks(
        bcm_timer_t *bcm_timer,
        uint64_t *ret_val
) {
    if (bcm_timer == NULL) {
        return result_err("NULL `bcm_timer` passed to bcm_timer_get_ticks().");
    }
    if (ret_val == NULL) {
        return result_err("NULL `ret_val` passed to bcm_timer_get_ticks().");
    }
    uint32_t counter_lo;
    uint32_t counter_hi;
    uint32_t counter_hi_tmp;
    do {
        result_t res = bcm_timer_regs_get_counter_lo(bcm_timer->regs, &counter_lo);
        if (result_is_err(res)) {
            return result_err("Failed to get `counter_lo` in bcm_timer_get_ticks().");
        }
        res = bcm_timer_regs_get_counter_hi(bcm_timer->regs, &counter_hi);
        if (result_is_err(res)) {
            return result_err("Failed to get `counter_hi` in bcm_timer_get_ticks().");
        }
        res = bcm_timer_regs_get_counter_hi(bcm_timer->regs, &counter_hi_tmp);
        if (result_is_err(res)) {
            return result_err("Failed to get `counter_hi_tmp` in bcm_timer_get_ticks().");
        }
    } while (counter_hi_tmp != counter_hi); /* Check system timer hasn't ticked over in that time. */
    /* Join the two 32-bit halves into a single 64-bit integer. */
    *ret_val = ((uint64_t) counter_hi << 32) | counter_lo;
    return result_ok();
}

