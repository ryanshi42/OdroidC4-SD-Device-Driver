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

result_t bcm_emmc_regs_is_host_circuit_reset(bcm_emmc_regs_t *bcm_emmc_regs, bool *ret_val) {
    if (bcm_emmc_regs == NULL) {
        return result_err("NULL `bcm_emmc_regs` passed to bcm_emmc_regs_is_host_circuit_reset().");
    }
    if (ret_val == NULL) {
        return result_err("NULL `ret_val` passed to bcm_emmc_regs_is_host_circuit_reset().");
    }
    bool srst_hc;
    result_t res = control1_get_srst_hc(&bcm_emmc_regs->control1, &srst_hc);
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to get `control1.SRST_HC` in bcm_emmc_regs_is_host_circuit_reset().");
    }
    /* The host circuit has reset successfully when `control1.SRST_HC` has been
     * reset back to 0. */
    *ret_val = (srst_hc == 0);
    return result_ok();
}

result_t bcm_emmc_regs_set_max_data_timeout(bcm_emmc_regs_t *bcm_emmc_regs) {
    if (bcm_emmc_regs == NULL) {
        return result_err("NULL `bcm_emmc_regs` passed to bcm_emmc_regs_set_max_data_timeout().");
    }
    /* Per the BCM2835's manual, setting the Data Timeout Unit to 0b1111
     * disables the internal clock, which means the maximum value we can set the
     * register to is 0b1110. */
    result_t res = control1_set_data_tounit(&bcm_emmc_regs->control1, 0b1110);
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to set `control1.DATA_TIMEOUT_UNIT` in bcm_emmc_regs_set_max_data_timeout().");
    }
    return result_ok();
}

result_t bcm_emmc_regs_enable_internal_clock(bcm_emmc_regs_t *bcm_emmc_regs) {
    if (bcm_emmc_regs == NULL) {
        return result_err("NULL `bcm_emmc_regs` passed to bcm_emmc_regs_enable_internal_clock().");
    }
    result_t res = control1_set_clk_intlen(&bcm_emmc_regs->control1, true);
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to set `control1.CLK_INTLEN` in bcm_emmc_regs_enable_internal_clock().");
    }
    return result_ok();
}
