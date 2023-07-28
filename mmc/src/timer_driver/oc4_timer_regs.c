#include "oc4_timer_regs.h"

oc4_timer_regs_t *oc4_timer_regs_get(uintptr_t base_vaddr) {
    return (oc4_timer_regs_t *) base_vaddr;
}

result_t oc4_timer_regs_get_counter_lo(oc4_timer_regs_t *oc4_timer_regs, uint32_t *ret_val) {
    if (oc4_timer_regs == NULL) {
        return result_err("NULL `oc4_timer_regs` passed to oc4_timer_regs_get_counter_lo().");
    }
    *ret_val = oc4_timer_regs->timer_e;
    // *ret_val = oc4_timer_regs->mux;
    return result_ok();
}

result_t oc4_timer_regs_get_counter_hi(oc4_timer_regs_t *oc4_timer_regs, uint32_t *ret_val) {
    if (oc4_timer_regs == NULL) {
        return result_err("NULL `oc4_timer_regs` passed to oc4_timer_regs_get_counter_hi().");
    }
    *ret_val = oc4_timer_regs->timer_e_hi;
    // *ret_val = oc4_timer_regs->mux;
    return result_ok();
}

