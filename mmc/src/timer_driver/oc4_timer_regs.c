#include "oc4_timer_regs.h"

oc4_timer_regs_t *oc4_timer_regs_get(uintptr_t base_vaddr) {
    return (oc4_timer_regs_t *) base_vaddr;
}

result_t oc4_timer_regs_get_counter_lo(oc4_timer_regs_t *oc4_timer_regs, uint32_t *ret_val) {
    if (oc4_timer_regs == NULL) {
        return result_err("NULL `oc4_timer_regs` passed to oc4_timer_regs_get_counter_lo().");
    }
    // oc4_timer_regs->timer_f = 256;
    // oc4_timer_regs->mux = TIMER_A_EN | (TIMESTAMP_TIMEBASE_100_US << TIMER_E_INPUT_CLK) | (TIMEOUT_TIMEBASE_1_MS << TIMER_A_INPUT_CLK);
    // oc4_timer->regs->timer_e = 0;
    *ret_val = oc4_timer_regs->timer_e;
    return result_ok();
}

result_t oc4_timer_regs_get_counter_hi(oc4_timer_regs_t *oc4_timer_regs, uint32_t *ret_val) {
    if (oc4_timer_regs == NULL) {
        return result_err("NULL `oc4_timer_regs` passed to oc4_timer_regs_get_counter_hi().");
    }
    // oc4_timer_regs->mux = TIMER_A_EN | (TIMESTAMP_TIMEBASE_100_US << TIMER_E_INPUT_CLK) | (TIMEOUT_TIMEBASE_1_MS << TIMER_A_INPUT_CLK);

    *ret_val = oc4_timer_regs->timer_e_hi;
    return result_ok();
}

