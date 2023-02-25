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


