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
    return result_ok_or(res, "Failed to set `control1.DATA_TIMEOUT_UNIT` in bcm_emmc_regs_set_max_data_timeout().");
}

result_t bcm_emmc_regs_enable_internal_clock(bcm_emmc_regs_t *bcm_emmc_regs) {
    if (bcm_emmc_regs == NULL) {
        return result_err("NULL `bcm_emmc_regs` passed to bcm_emmc_regs_enable_internal_clock().");
    }
    result_t res = control1_set_clk_intlen(&bcm_emmc_regs->control1, true);
    return result_ok_or(res, "Failed to set `control1.CLK_INTLEN` in bcm_emmc_regs_enable_internal_clock().");
}

result_t bcm_emmc_regs_is_data_lines_busy(bcm_emmc_regs_t *bcm_emmc_regs, bool *ret_val) {
    if (bcm_emmc_regs == NULL) {
        return result_err("NULL `bcm_emmc_regs` passed to bcm_emmc_regs_is_data_lines_busy().");
    }
    bool is_busy;
    result_t res = status_get_dat_inhibit(&bcm_emmc_regs->status, &is_busy);
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to get `status.DAT_INHIBIT` in bcm_emmc_regs_is_data_lines_busy().");
    }
    *ret_val = is_busy;
    return result_ok();
}

result_t bcm_emmc_regs_is_cmd_line_busy(bcm_emmc_regs_t *bcm_emmc_regs, bool *ret_val) {
    if (bcm_emmc_regs == NULL) {
        return result_err("NULL `bcm_emmc_regs` passed to bcm_emmc_regs_is_cmd_line_busy().");
    }
    bool is_busy;
    result_t res = status_get_cmd_inhibit(&bcm_emmc_regs->status, &is_busy);
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to get `status.CMD_INHIBIT` in bcm_emmc_regs_is_cmd_line_busy().");
    }
    *ret_val = is_busy;
    return result_ok();
}

result_t bcm_emmc_regs_disable_sd_clock(bcm_emmc_regs_t *bcm_emmc_regs) {
    if (bcm_emmc_regs == NULL) {
        return result_err("NULL `bcm_emmc_regs` passed to bcm_emmc_regs_disable_sd_clock().");
    }
    result_t res = control1_set_clk_en(&bcm_emmc_regs->control1, false);
    return result_ok_or(res, "Failed to set `control1.CLK_EN` in bcm_emmc_regs_disable_sd_clock().");
}

result_t bcm_emmc_regs_enable_sd_clock(bcm_emmc_regs_t *bcm_emmc_regs) {
    if (bcm_emmc_regs == NULL) {
        return result_err("NULL `bcm_emmc_regs` passed to bcm_emmc_regs_enable_sd_clock().");
    }
    result_t res = control1_set_clk_en(&bcm_emmc_regs->control1, true);
    return result_ok_or(res,"Failed to set `control1.CLK_EN` in bcm_emmc_regs_enable_sd_clock().");
}

result_t bcm_emmc_regs_get_host_controller_spec_version(
        bcm_emmc_regs_t *bcm_emmc_regs,
        uint8_t *ret_val
) {
    if (bcm_emmc_regs == NULL) {
        return result_err("NULL `bcm_emmc_regs` passed to bcm_emmc_regs_get_host_controller_spec_version().");
    }
    result_t res = slotisr_ver_get_sdversion(&bcm_emmc_regs->slotisr_ver, ret_val);
    return result_ok_or(res,"Failed to get `slotisr_ver.SDVERSION` in bcm_emmc_regs_get_host_controller_spec_version().");
}

result_t bcm_emmc_regs_set_sd_clock_mode_as_divided(
        bcm_emmc_regs_t *bcm_emmc_regs
) {
    if (bcm_emmc_regs == NULL) {
        return result_err("NULL `bcm_emmc_regs` passed to bcm_emmc_regs_set_sd_clock_mode_as_divided().");
    }
    result_t res = control1_set_clk_gensel(&bcm_emmc_regs->control1, false);
    return result_ok_or(res, "Failed to set `control1.CLK_GENSEL` in bcm_emmc_regs_set_sd_clock_mode_as_divided().");
}

result_t bcm_emmc_regs_set_sd_clock_divisor(
        bcm_emmc_regs_t *bcm_emmc_regs,
        uint16_t divisor
) {
    if (bcm_emmc_regs == NULL) {
        return result_err("NULL `bcm_emmc_regs` passed to bcm_emmc_regs_set_clock_divisor().");
    }
    if (divisor > 0b1111111111) {
        return result_err("Invalid `divisor` passed to bcm_emmc_regs_set_clock_divisor().");
    }
    /* Pass the 9th and 10th bit of `divisor` to control1_set_clk_freq_ms2(). */
    result_t res = control1_set_clk_freq_ms2(&bcm_emmc_regs->control1, (divisor >> 8) & 0b11);
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to set `control1.CLK_FREQ_MS2` in bcm_emmc_regs_set_clock_divisor().");
    }
    /* Pass the 1st to 8th bits of `divisor` to control1_set_clk_freq8(). */
    res = control1_set_clk_freq8(&bcm_emmc_regs->control1, (divisor) & 0b11111111);
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to set `control1.CLK_FREQ8` in bcm_emmc_regs_set_clock_divisor().");
    }
    return result_ok();
}

result_t bcm_emmc_regs_is_sd_clock_stable(
        bcm_emmc_regs_t *bcm_emmc_regs,
        bool *ret_val
) {
    if (bcm_emmc_regs == NULL) {
        return result_err("NULL `bcm_emmc_regs` passed to bcm_emmc_regs_is_sd_clock_stable().");
    }
    return control1_get_clk_stable(&bcm_emmc_regs->control1, ret_val);
}

result_t bcm_emmc_regs_enable_interrupts(
        bcm_emmc_regs_t *bcm_emmc_regs
) {
    if (bcm_emmc_regs == NULL) {
        return result_err("NULL `bcm_emmc_regs` passed to bcm_emmc_regs_enable_interrupts().");
    }
    result_t res = irpt_en_set_raw32(&bcm_emmc_regs->irpt_en, 0xffffffff);
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to set `irpt_en` in bcm_emmc_regs_enable_interrupts().");
    }
    res = irpt_mask_set_raw32(&bcm_emmc_regs->irpt_mask, 0xffffffff);
    return result_ok_or(res, "Failed to set `irpt_mask` in bcm_emmc_regs_enable_interrupts().");
}

result_t bcm_emmc_regs_mask_interrupt(
        bcm_emmc_regs_t *bcm_emmc_regs,
        uint32_t mask,
        bool *ret_val
) {
    if (bcm_emmc_regs == NULL) {
        return result_err("NULL `bcm_emmc_regs` passed to bcm_emmc_regs_mask_interrupt().");
    }
    return interrupt_mask_raw32(&bcm_emmc_regs->interrupt, mask, ret_val);
}

result_t bcm_emmc_regs_get_interrupt_raw32(
        bcm_emmc_regs_t *bcm_emmc_regs,
        uint32_t *ret_val
) {
    if (bcm_emmc_regs == NULL) {
        return result_err("NULL `bcm_emmc_regs` passed to bcm_emmc_regs_get_interrupt_raw32().");
    }
    return interrupt_get_raw32(&bcm_emmc_regs->interrupt, ret_val);
}

result_t bcm_emmc_regs_set_interrupt_raw32(
        bcm_emmc_regs_t *bcm_emmc_regs,
        uint32_t val
) {
    if (bcm_emmc_regs == NULL) {
        return result_err("NULL `bcm_emmc_regs` passed to bcm_emmc_regs_set_interrupt_raw32().");
    }
    return interrupt_set_raw32(&bcm_emmc_regs->interrupt, val);
}

result_t bcm_emmc_regs_is_cmd_timeout_err(
        bcm_emmc_regs_t *bcm_emmc_regs,
        bool *ret_val
) {
    if (bcm_emmc_regs == NULL) {
        return result_err("NULL `bcm_emmc_regs` passed to bcm_emmc_regs_is_cmd_timeout_err().");
    }
    return interrupt_get_cto_err(&bcm_emmc_regs->interrupt, ret_val);
}

result_t bcm_emmc_regs_is_data_timeout_err(
        bcm_emmc_regs_t *bcm_emmc_regs,
        bool *ret_val
) {
    if (bcm_emmc_regs == NULL) {
        return result_err("NULL `bcm_emmc_regs` passed to bcm_emmc_regs_is_data_timeout_err().");
    }
    return interrupt_get_dto_err(&bcm_emmc_regs->interrupt, ret_val);
}

//result_t bcm_emmc_regs_wait_for_interrupt(
//        bcm_emmc_regs_t *bcm_emmc_regs,
//        uint32_t mask
//) {
//    if (bcm_emmc_regs == NULL) {
//        return result_err("NULL `bcm_emmc_regs` passed to bcm_emmc_regs_wait_for_interrupt().");
//    }
//    interrupt_t *interrupt = &bcm_emmc_regs->interrupt;
//    uint32_t mask_with_error = mask | INT_ERROR_MASK;
//    bool is_finished_or_error = false;
//    do {
//        result_t res = interrupt_mask_raw32(
//                interrupt,
//                mask_with_error,
//                &is_finished_or_error
//        );
//        if (result_is_err(res)) {
//            return result_err_chain(res, "Failed to wait for interrupt in bcm_emmc_regs_wait_for_interrupt().");
//        }
//    } while(!is_finished_or_error);
//    /* Read interrupt. */
//    uint32_t interrupt_raw32 = 0;
//    result_t res_get_raw32 = interrupt_get_raw32(interrupt, &interrupt_raw32);
//    if (result_is_err(res_get_raw32)) {
//        return result_err_chain(res_get_raw32, "Failed to get `interrupt` in bcm_emmc_regs_wait_for_interrupt().");
//    }
//    /* Timeout case. */
//    if (!is_finished_or_error) {
//        bool is_cmd_timeout = false;
//        result_t res = interrupt_get_cto_err(interrupt,&is_cmd_timeout);
//        if (result_is_err(res)) {
//            return result_err_chain(res, "Failed to get `interrupt.CTO_ERR` in bcm_emmc_regs_wait_for_interrupt().");
//        }
//        bool is_data_timeout = false;
//        res = interrupt_get_dto_err(interrupt,&is_data_timeout);
//        if (result_is_err(res)) {
//            return result_err_chain(res, "Failed to get `interrupt.DTO_ERR` in bcm_emmc_regs_wait_for_interrupt().");
//        }
//        /* Clear the interrupt register. */
//        res = interrupt_set_raw32(interrupt, interrupt_raw32);
//        if (result_is_err(res)) {
//            return result_err_chain(res, "Failed to clear `interrupt` in bcm_emmc_regs_wait_for_interrupt().");
//        }
//        /* Return error depending on the type of timeout. */
//        if (is_cmd_timeout) {
//            return result_err("Command timeout error in bcm_emmc_regs_wait_for_interrupt().");
//        }
//        if (is_data_timeout) {
//            return result_err("Data timeout error in bcm_emmc_regs_wait_for_interrupt().");
//        }
//        return result_err("Timed out waiting for interrupt in bcm_emmc_regs_wait_for_interrupt().");
//    }
//    /* Error case. */
//    bool is_error = false;
//    result_t res = interrupt_mask_raw32(interrupt, INT_ERROR_MASK, &is_error);
//    if (result_is_err(res)) {
//        return result_err_chain(res, "Failed to check for error in bcm_emmc_regs_wait_for_interrupt().");
//    }
//    if (is_error) {
//        /* Clear the interrupt register. */
//        res = interrupt_set_raw32(interrupt, interrupt_raw32);
//        if (result_is_err(res)) {
//            return result_err_chain(res, "Failed to clear `interrupt` in bcm_emmc_regs_wait_for_interrupt().");
//        }
//        return result_err("Error interrupt in bcm_emmc_regs_wait_for_interrupt().");
//    }
//    /* Clear the interrupt register. */
//    res = interrupt_set_raw32(interrupt, mask);
//    if (result_is_err(res)) {
//        return result_err_chain(res, "Failed to clear `interrupt` in bcm_emmc_regs_wait_for_interrupt().");
//    }
//    if (result_is_err(res)) {
//        return result_err_chain(res, "Failed to clear `interrupt` in bcm_emmc_regs_wait_for_interrupt().");
//    }
//
//    /* Success case. */
//    return result_ok();
//}