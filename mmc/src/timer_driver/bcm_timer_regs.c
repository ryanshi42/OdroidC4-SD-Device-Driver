#include "bcm_timer_regs.h"

bcm_timer_regs_t *bcm_timer_regs_get(uintptr_t base_vaddr) {
    return (bcm_timer_regs_t *) base_vaddr;
}

result_t bcm_timer_regs_get_counter_lo(bcm_timer_regs_t *bcm_timer_regs, uint32_t *ret_val) {
    if (bcm_timer_regs == NULL) {
        return result_err("NULL `bcm_timer_regs` passed to bcm_timer_regs_get_counter_lo().");
    }
    *ret_val = bcm_timer_regs->counter_lo;
    return result_ok();
}

result_t bcm_timer_regs_get_counter_hi(bcm_timer_regs_t *bcm_timer_regs, uint32_t *ret_val) {
    if (bcm_timer_regs == NULL) {
        return result_err("NULL `bcm_timer_regs` passed to bcm_timer_regs_get_counter_hi().");
    }
    *ret_val = bcm_timer_regs->counter_hi;
    return result_ok();
}

