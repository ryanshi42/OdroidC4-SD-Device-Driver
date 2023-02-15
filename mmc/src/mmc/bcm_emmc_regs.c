#include "bcm_emmc_regs.h"

bcm_emmc_regs_t *bcm_emmc_regs_get(uintptr_t base_vaddr) {
    return (bcm_emmc_regs_t *) base_vaddr;
}

result_t bcm_emmc_regs_zero_control0(bcm_emmc_regs_t *bcm_emmc_regs) {
    if (bcm_emmc_regs == NULL) {
        return result_err("NULL `bcm_emmc_regs` passed to bcm_emmc_regs_control0_zero().");
    }
    result_t res = control0_set_raw32(&bcm_emmc_regs->control0, 0);
    return result_ok_or(res, "Failed to set `control0` in bcm_emmc_regs_control0_zero().");
}




