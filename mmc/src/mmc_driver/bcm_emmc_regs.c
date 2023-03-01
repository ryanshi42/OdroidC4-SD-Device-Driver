#include "bcm_emmc_regs.h"

result_t bcm_emmc_regs_zero_control0(bcm_emmc_regs_t *bcm_emmc_regs) {
    if (bcm_emmc_regs == NULL) {
        return result_err("NULL `bcm_emmc_regs` passed to bcm_emmc_regs_control0_zero().");
    }
    result_t res = control0_set_raw32(&bcm_emmc_regs->control0, 0);
    return result_ok_or(res, "Failed to set `control0` in bcm_emmc_regs_control0_zero().");
}

result_t bcm_emmc_regs_zero_control1(bcm_emmc_regs_t *bcm_emmc_regs) {
    if (bcm_emmc_regs == NULL) {
        return result_err("NULL `bcm_emmc_regs` passed to bcm_emmc_regs_control1_zero().");
    }
    result_t res = control1_set_raw32(&bcm_emmc_regs->control1, 0);
    return result_ok_or(res, "Failed to set `control1` in bcm_emmc_regs_control1_zero().");
}

result_t bcm_emmc_regs_reset_host_circuit(bcm_emmc_regs_t *bcm_emmc_regs) {
    if (bcm_emmc_regs == NULL) {
        return result_err("NULL `bcm_emmc_regs` passed to bcm_emmc_regs_reset_host_circuit().");
    }
    result_t res = control1_set_srst_hc(&bcm_emmc_regs->control1, true);
    return result_ok_or(res, "Failed to set `control1.SRST_HC` in bcm_emmc_regs_reset_host_circuit().");
}
