#include "oc4_emmc_regs.h"

//* These functions are included simply to make the compiler happy. They do not work and are RPi3 specific!
//* Scroll down below for the OC4 functions.

//! This does not work!
result_t oc4_emmc_regs_zero_control0(oc4_emmc_regs_t *oc4_emmc_regs) {
    if (oc4_emmc_regs == NULL) {
        return result_err("NULL `oc4_emmc_regs` passed to oc4_emmc_regs_control0_zero().");
    }
    return result_err("I should never be called!");


    // result_t res = control0_set_raw32(&oc4_emmc_regs->control0, 0);
    // return result_ok_or(res, "Failed to set `control0` in oc4_emmc_regs_control0_zero().");
}

//! This does not work!
result_t oc4_emmc_regs_zero_control1(oc4_emmc_regs_t *oc4_emmc_regs) {
    if (oc4_emmc_regs == NULL) {
        return result_err("NULL `oc4_emmc_regs` passed to oc4_emmc_regs_control1_zero().");
    }
    return result_err("I should never be called!");


    // result_t res = control1_set_raw32(&oc4_emmc_regs->control1, 0);
    // return result_ok_or(res, "Failed to set `control1` in oc4_emmc_regs_control1_zero().");
}

//! This does not work!
result_t oc4_emmc_regs_reset_host_circuit(oc4_emmc_regs_t *oc4_emmc_regs) {
    if (oc4_emmc_regs == NULL) {
        return result_err("NULL `oc4_emmc_regs` passed to oc4_emmc_regs_reset_host_circuit().");
    }
    return result_err("I should never be called!");


    // result_t res = control1_set_srst_hc(&oc4_emmc_regs->control1, true);
    // return result_ok_or(res, "Failed to set `control1.SRST_HC` in oc4_emmc_regs_reset_host_circuit().");
}

//! This does not work!
result_t oc4_emmc_regs_is_host_circuit_reset(oc4_emmc_regs_t *oc4_emmc_regs, bool *ret_val) {
    if (oc4_emmc_regs == NULL) {
        return result_err("NULL `oc4_emmc_regs` passed to oc4_emmc_regs_is_host_circuit_reset().");
    }
    if (ret_val == NULL) {
        return result_err("NULL `ret_val` passed to oc4_emmc_regs_is_host_circuit_reset().");
    }
    *ret_val = true;
    return result_err("I should never be called!");


    // bool srst_hc;
    // result_t res = control1_get_srst_hc(&oc4_emmc_regs->control1, &srst_hc);
    // if (result_is_err(res)) {
    //     return result_err_chain(res, "Failed to get `control1.SRST_HC` in oc4_emmc_regs_is_host_circuit_reset().");
    // }
    // /* The host circuit has reset successfully when `control1.SRST_HC` has been reset back to 0. */
    // *ret_val = (srst_hc == 0);
    // return result_ok();
}

//! This does not work!
result_t oc4_emmc_regs_enable_internal_clock(oc4_emmc_regs_t *oc4_emmc_regs) {
    if (oc4_emmc_regs == NULL) {
        return result_err("NULL `oc4_emmc_regs` passed to oc4_emmc_regs_enable_internal_clock().");
    }
    return result_err("I should never be called!");

    // result_t res = control1_set_clk_intlen(&oc4_emmc_regs->control1, true);
    // return result_ok_or(res, "Failed to set `control1.CLK_INTLEN` in oc4_emmc_regs_enable_internal_clock().");
}

//! This does not work!
result_t oc4_emmc_regs_is_data_lines_busy(oc4_emmc_regs_t *oc4_emmc_regs, bool *ret_val) {
    if (oc4_emmc_regs == NULL) {
        return result_err("NULL `oc4_emmc_regs` passed to oc4_emmc_regs_is_data_lines_busy().");
    }
    if (ret_val == NULL) {
        return result_err("NULL `ret_val` passed to oc4_emmc_regs_is_data_lines_busy().");
    }
    *ret_val = false;
    return result_err("I should never be called!");
    
    // bool is_busy;
    // result_t res = status_get_dat_inhibit(&oc4_emmc_regs->status, &is_busy);
    // if (result_is_err(res)) {
    //     return result_err_chain(res, "Failed to get `status.DAT_INHIBIT` in oc4_emmc_regs_is_data_lines_busy().");
    // }
    // *ret_val = is_busy;

    // return result_ok();
}

//! This does not work!
result_t oc4_emmc_regs_is_cmd_line_busy(oc4_emmc_regs_t *oc4_emmc_regs, bool *ret_val) {
    if (oc4_emmc_regs == NULL) {
        return result_err("NULL `oc4_emmc_regs` passed to oc4_emmc_regs_is_cmd_line_busy().");
    }
    if (ret_val == NULL) {
        return result_err("NULL `ret_val` passed to oc4_emmc_regs_is_cmd_line_busy().");
    }
    *ret_val = false;

    // bool is_busy;
    // result_t res = status_get_cmd_inhibit(&oc4_emmc_regs->status, &is_busy);
    // if (result_is_err(res)) {
    //     return result_err_chain(res, "Failed to get `status.CMD_INHIBIT` in oc4_emmc_regs_is_cmd_line_busy().");
    // }
    // *ret_val = is_busy;
    return result_err("I should never be called!");
}

//! This does not work!
result_t oc4_emmc_regs_disable_sd_clock(oc4_emmc_regs_t *oc4_emmc_regs) {
    if (oc4_emmc_regs == NULL) {
        return result_err("NULL `oc4_emmc_regs` passed to oc4_emmc_regs_disable_sd_clock().");
    }
    // result_t res = control1_set_clk_en(&oc4_emmc_regs->control1, false);
    // return result_ok_or(res, "Failed to set `control1.CLK_EN` in oc4_emmc_regs_disable_sd_clock().");
    return result_err("I should never be called!");
}

//! This does not work!
result_t oc4_emmc_regs_enable_sd_clock(oc4_emmc_regs_t *oc4_emmc_regs) {
    if (oc4_emmc_regs == NULL) {
        return result_err("NULL `oc4_emmc_regs` passed to oc4_emmc_regs_enable_sd_clock().");
    }
    // result_t res = control1_set_clk_en(&oc4_emmc_regs->control1, true);
    // return result_ok_or(res,"Failed to set `control1.CLK_EN` in oc4_emmc_regs_enable_sd_clock().");
    return result_err("I should never be called!");
}

//! This does not work!
result_t oc4_emmc_regs_get_host_controller_spec_version(
        oc4_emmc_regs_t *oc4_emmc_regs,
        uint8_t *ret_val
) {
    if (oc4_emmc_regs == NULL) {
        return result_err("NULL `oc4_emmc_regs` passed to oc4_emmc_regs_get_host_controller_spec_version().");
    }
    if (ret_val == NULL) {
        return result_err("NULL `ret_val` passed to oc4_emmc_regs_get_host_controller_spec_version().");
    }
    return result_err("I should never be called!");
    // result_t res = slotisr_ver_get_sdversion(&oc4_emmc_regs->slotisr_ver, ret_val);
    // return result_ok_or(res,"Failed to get `slotisr_ver.SDVERSION` in oc4_emmc_regs_get_host_controller_spec_version().");
}

//! This does not work!
result_t oc4_emmc_regs_set_sd_clock_mode_as_divided(
        oc4_emmc_regs_t *oc4_emmc_regs
) {
    if (oc4_emmc_regs == NULL) {
        return result_err("NULL `oc4_emmc_regs` passed to oc4_emmc_regs_set_sd_clock_mode_as_divided().");
    }
    return result_err("I should never be called!");
    // result_t res = control1_set_clk_gensel(&oc4_emmc_regs->control1, false);
    // return result_ok_or(res, "Failed to set `control1.CLK_GENSEL` in oc4_emmc_regs_set_sd_clock_mode_as_divided().");
}

//! This does not work!
result_t oc4_emmc_regs_set_sd_clock_divisor(
        oc4_emmc_regs_t *oc4_emmc_regs,
        uint16_t divisor
) {
    if (oc4_emmc_regs == NULL) {
        return result_err("NULL `oc4_emmc_regs` passed to oc4_emmc_regs_set_clock_divisor().");
    }
    if (divisor > 0b1111111111) {
        return result_err("Invalid `divisor` passed to oc4_emmc_regs_set_clock_divisor().");
    }
    // /* Pass the 9th and 10th bit of `divisor` to control1_set_clk_freq_ms2(). */
    // result_t res = control1_set_clk_freq_ms2(&oc4_emmc_regs->control1, (divisor >> 8) & 0b11);
    // if (result_is_err(res)) {
    //     return result_err_chain(res, "Failed to set `control1.CLK_FREQ_MS2` in oc4_emmc_regs_set_clock_divisor().");
    // }
    // /* Pass the 1st to 8th bits of `divisor` to control1_set_clk_freq8(). */
    // res = control1_set_clk_freq8(&oc4_emmc_regs->control1, (divisor) & 0b11111111);
    // if (result_is_err(res)) {
    //     return result_err_chain(res, "Failed to set `control1.CLK_FREQ8` in oc4_emmc_regs_set_clock_divisor().");
    // }
    return result_err("I should never be called!");

}

//! This does not work!
result_t oc4_emmc_regs_is_sd_clock_stable(
        oc4_emmc_regs_t *oc4_emmc_regs,
        bool *ret_val
) {
    if (oc4_emmc_regs == NULL) {
        return result_err("NULL `oc4_emmc_regs` passed to oc4_emmc_regs_is_sd_clock_stable().");
    }
    if (ret_val == NULL) {
        return result_err("NULL `ret_val` passed to oc4_emmc_regs_is_sd_clock_stable().");
    }
    // return control1_get_clk_stable(&oc4_emmc_regs->control1, ret_val);
    return result_err("I should never be called!");
}

//! This does not work!
result_t oc4_emmc_regs_is_cmd_in_progress(
        oc4_emmc_regs_t *oc4_emmc_regs,
        bool *ret_val
) {
    if (oc4_emmc_regs == NULL) {
        return result_err("NULL `oc4_emmc_regs` passed to oc4_emmc_regs_is_cmd_in_progress().");
    }
    if (ret_val == NULL) {
        return result_err("NULL `ret_val` passed to oc4_emmc_regs_is_cmd_in_progress().");
    }
    // return status_get_cmd_inhibit(&oc4_emmc_regs->sd_emmc_status, ret_val);
    return result_err("I should never be called!");
}

//! This does not work!
result_t oc4_emmc_regs_set_cmdtm(
        oc4_emmc_regs_t *oc4_emmc_regs,
        cmdtm_t val
) {
    if (oc4_emmc_regs == NULL) {
        return result_err("NULL `oc4_emmc_regs` passed to oc4_emmc_regs_set_cmdtm().");
    }
    // oc4_emmc_regs->cmdtm = val;
    // return result_ok();
    return result_err("I should never be called!");
}

//! This does not work!
result_t oc4_emmc_regs_set_block_size(
        oc4_emmc_regs_t *oc4_emmc_regs,
        uint32_t val
) {
    if (oc4_emmc_regs == NULL) {
        return result_err("NULL `oc4_emmc_regs` passed to oc4_emmc_regs_set_blk_size().");
    }
    // return result_ok();
    return result_err("I should never be called!");
    // return blksizecnt_set_blksize(&oc4_emmc_regs->blksizecnt, val);
}

//! This does not work!
result_t oc4_emmc_regs_set_block_count(
        oc4_emmc_regs_t *oc4_emmc_regs,
        uint32_t val
) {
    if (oc4_emmc_regs == NULL) {
        return result_err("NULL `oc4_emmc_regs` passed to oc4_emmc_regs_set_blk_cnt().");
    }
    // return result_ok();
    return result_err("I should never be called!");

    // return blksizecnt_set_blkcnt(&oc4_emmc_regs->blksizecnt, val);
}

result_t oc4_emmc_regs_is_read_ready(
        oc4_emmc_regs_t *oc4_emmc_regs,
        bool *ret_val
) {
    if (oc4_emmc_regs == NULL) {
        return result_err("NULL `oc4_emmc_regs` passed to oc4_emmc_regs_is_read_ready().");
    }
    if (ret_val == NULL) {
        return result_err("NULL `ret_val` passed to oc4_emmc_regs_is_read_ready().");
    }
    // return status_get_read_transfer(&oc4_emmc_regs->sd_emmc_status, ret_val);
    return result_err("I should never be called!");
}

result_t oc4_emmc_regs_is_write_ready(
        oc4_emmc_regs_t *oc4_emmc_regs,
        bool *ret_val
) {
    if (oc4_emmc_regs == NULL) {
        return result_err("NULL `oc4_emmc_regs` passed to oc4_emmc_regs_is_write_ready().");
    }
    if (ret_val == NULL) {
        return result_err("NULL `ret_val` passed to oc4_emmc_regs_is_write_ready().");
    }
    // return status_get_write_transfer(&oc4_emmc_regs->sd_emmc_status, ret_val);
    return result_err("I should never be called!");
}

result_t oc4_emmc_regs_is_data_in_progress(
        oc4_emmc_regs_t *oc4_emmc_regs,
        bool *ret_val
) {
    if (oc4_emmc_regs == NULL) {
        return result_err("NULL `oc4_emmc_regs` passed to oc4_emmc_regs_is_data_in_progress().");
    }
    if (ret_val == NULL) {
        return result_err("NULL `ret_val` passed to oc4_emmc_regs_is_data_in_progress().");
    }
    // *ret_val = oc4_emmc_regs->sd_emmc_status;
    *ret_val = true;
    // return result_ok(); 
    return result_err("I should never be called!");
}














//* The following functions are shared between RPi3 and OC4.

result_t oc4_emmc_regs_set_max_data_timeout(oc4_emmc_regs_t *oc4_emmc_regs) {
    if (oc4_emmc_regs == NULL) {
        return result_err("NULL `oc4_emmc_regs` passed to oc4_emmc_regs_set_max_data_timeout().");
    }
    //? Unsure whether this is the correct DATA timeout or not, just based it off Linux!
    //? The other timeout refers to the config timeout, but that is in clock cycles whereas presumably this one is not

    //? The documentation is badly formatted - if timeout is set to 12, then it is 2 ** 12 = 4096 ms ~= 4s
    oc4_emmc_regs->sd_emmc_cfg |= ~CMD_CFG_TIMEOUT_MASK;
    oc4_emmc_regs->sd_emmc_cfg |= CMD_CFG_TIMEOUT_4S;
    return result_ok();

    // result_t res = control1_set_data_tounit(&oc4_emmc_regs->control1, 0b1110);
    // return result_ok_or(res, "Failed to set `control1.DATA_TIMEOUT_UNIT` in oc4_emmc_regs_set_max_data_timeout().");
}

result_t oc4_emmc_regs_enable_interrupts(
        oc4_emmc_regs_t *oc4_emmc_regs
) {
    if (oc4_emmc_regs == NULL) {
        return result_err("NULL `oc4_emmc_regs` passed to oc4_emmc_regs_enable_interrupts().");
    }
    oc4_emmc_regs->sd_emmc_irq_en = 0xffffffff;
    // result_t res = irpt_en_set_raw32(&oc4_emmc_regs->sd_emmc_irq_en, 0xffffffff);
    // if (result_is_err(res)) {
    //     return result_err_chain(res, "Failed to set `irpt_en` in oc4_emmc_regs_enable_interrupts().");
    // }
    return result_ok();
    // res = irpt_mask_set_raw32(&oc4_emmc_regs->irpt_mask, 0xffffffff);
    // return result_ok_or(res, "Failed to set `irpt_mask` in oc4_emmc_regs_enable_interrupts().");
}

result_t oc4_emmc_regs_mask_interrupt(
        oc4_emmc_regs_t *oc4_emmc_regs,
        uint32_t mask,
        bool *ret_val
) {
    if (oc4_emmc_regs == NULL) {
        return result_err("NULL `oc4_emmc_regs` passed to oc4_emmc_regs_mask_interrupt().");
    }
    if (ret_val == NULL) {
        return result_err("NULL `ret_val` passed to oc4_emmc_regs_mask_interrupt().");
    }
    // return interrupt_mask_raw32(&oc4_emmc_regs->sd_emmc_irq_en, mask, ret_val);
    *ret_val = oc4_emmc_regs->sd_emmc_irq_en & mask;
    return result_ok();

}

result_t oc4_emmc_regs_get_interrupt_raw32(
        oc4_emmc_regs_t *oc4_emmc_regs,
        uint32_t *ret_val
) {
    if (oc4_emmc_regs == NULL) {
        return result_err("NULL `oc4_emmc_regs` passed to oc4_emmc_regs_get_interrupt_raw32().");
    }
    if (ret_val == NULL) {
        return result_err("NULL `ret_val` passed to oc4_emmc_regs_get_interrupt_raw32().");
    }
    *ret_val = oc4_emmc_regs->sd_emmc_irq_en;
    return result_ok();
    // return interrupt_get_raw32(&oc4_emmc_regs->sd_emmc_irq_en, ret_val);
}

result_t oc4_emmc_regs_set_interrupt_raw32(
        oc4_emmc_regs_t *oc4_emmc_regs,
        uint32_t val
) {
    if (oc4_emmc_regs == NULL) {
        return result_err("NULL `oc4_emmc_regs` passed to oc4_emmc_regs_set_interrupt_raw32().");
    }
    oc4_emmc_regs->sd_emmc_irq_en = val;
    return result_ok();
}

// TODO: This may not work?
result_t oc4_emmc_regs_is_cmd_timeout_err(
        oc4_emmc_regs_t *oc4_emmc_regs,
        bool *ret_val
) {
    if (oc4_emmc_regs == NULL) {
        return result_err("NULL `oc4_emmc_regs` passed to oc4_emmc_regs_is_cmd_timeout_err().");
    }
    if (ret_val == NULL) {
        return result_err("NULL `ret_val` passed to oc4_emmc_regs_is_cmd_timeout_err().");
    }
    *ret_val = oc4_emmc_regs->sd_emmc_status & STATUS_DESC_TIMEOUT;
    return result_ok();
}

// TODO: This may not work?
result_t oc4_emmc_regs_is_data_timeout_err(
        oc4_emmc_regs_t *oc4_emmc_regs,
        bool *ret_val
) {
    if (oc4_emmc_regs == NULL) {
        return result_err("NULL `oc4_emmc_regs` passed to oc4_emmc_regs_is_data_timeout_err().");
    }
    if (ret_val == NULL) {
        return result_err("NULL `ret_val` passed to oc4_emmc_regs_is_data_timeout_err().");
    }
    *ret_val = oc4_emmc_regs->sd_emmc_status & STATUS_RESP_TIMEOUT;
    return result_ok();
}

result_t oc4_emmc_regs_is_any_err(
        oc4_emmc_regs_t *oc4_emmc_regs,
        bool *ret_val
) {
    if (oc4_emmc_regs == NULL) {
        return result_err("NULL `oc4_emmc_regs` passed to oc4_emmc_regs_is_err().");
    }
    if (ret_val == NULL) {
        return result_err("NULL `ret_val` passed to oc4_emmc_regs_is_err().");
    }
    *ret_val = oc4_emmc_regs->sd_emmc_status & STATUS_ERR_MASK;
    return result_ok();
}

//? This seems to do nothing
result_t oc4_emmc_regs_clear_interrupt(
        oc4_emmc_regs_t *oc4_emmc_regs
) {
    if (oc4_emmc_regs == NULL) {
        return result_err("NULL `oc4_emmc_regs` passed to oc4_emmc_regs_clear_interrupt().");
    }
    // return interrupt_clear(&oc4_emmc_regs->sd_emmc_irq_en);
    return result_ok();
}

result_t oc4_emmc_regs_set_arg1(
        oc4_emmc_regs_t *oc4_emmc_regs,
        uint32_t val
) {
    if (oc4_emmc_regs == NULL) {
        return result_err("NULL `oc4_emmc_regs` passed to oc4e_emmc_regs_set_arg1_raw32().");
    }
    oc4_emmc_regs->sd_emmc_cmd_arg = val;
    return result_ok();
}

result_t oc4_emmc_regs_get_resp0(
        oc4_emmc_regs_t *oc4_emmc_regs,
        uint32_t *ret_val
) {
    if (oc4_emmc_regs == NULL) {
        return result_err("NULL `oc4_emmc_regs` passed to oc4_emmc_regs_get_resp0().");
    }
    if (ret_val == NULL) {
        return result_err("NULL `ret_val` passed to oc4_emmc_regs_get_resp0().");
    }
    *ret_val = oc4_emmc_regs->sd_emmc_cmd_rsp;
    return result_ok();
}

result_t oc4_emmc_regs_get_resp1(
        oc4_emmc_regs_t *oc4_emmc_regs,
        uint32_t *ret_val
) {
    if (oc4_emmc_regs == NULL) {
        return result_err("NULL `oc4_emmc_regs` passed to oc4_emmc_regs_get_resp1().");
    }
    if (ret_val == NULL) {
        return result_err("NULL `ret_val` passed to oc4_emmc_regs_get_resp1().");
    }
    *ret_val = oc4_emmc_regs->sd_emmc_cmd_rsp1;
    return result_ok();
}

result_t oc4_emmc_regs_get_resp2(
        oc4_emmc_regs_t *oc4_emmc_regs,
        uint32_t *ret_val
) {
    if (oc4_emmc_regs == NULL) {
        return result_err("NULL `oc4_emmc_regs` passed to oc4_emmc_regs_get_resp2().");
    }
    if (ret_val == NULL) {
        return result_err("NULL `ret_val` passed to oc4_emmc_regs_get_resp2().");
    }
    *ret_val = oc4_emmc_regs->sd_emmc_cmd_rsp2;
    return result_ok();
}

result_t oc4_emmc_regs_get_resp3(
        oc4_emmc_regs_t *oc4_emmc_regs,
        uint32_t *ret_val
) {
    if (oc4_emmc_regs == NULL) {
        return result_err("NULL `oc4_emmc_regs` passed to oc4_emmc_regs_get_resp3().");
    }
    if (ret_val == NULL) {
        return result_err("NULL `ret_val` passed to oc4_emmc_regs_get_resp3().");
    }
    *ret_val = oc4_emmc_regs->sd_emmc_cmd_rsp3;
    return result_ok();
}

result_t oc4_emmc_regs_get_data(
        oc4_emmc_regs_t *oc4_emmc_regs,
        uint32_t *ret_val
) {
    if (oc4_emmc_regs == NULL) {
        return result_err("NULL `oc4_emmc_regs` passed to oc4_emmc_regs_get_data().");
    }
    if (ret_val == NULL) {
        return result_err("NULL `ret_val` passed to oc4_emmc_regs_get_data().");
    }
    *ret_val = oc4_emmc_regs->sd_emmc_cmd_dat;
    return result_ok();
}

result_t oc4_emmc_regs_set_data(
        oc4_emmc_regs_t *oc4_emmc_regs,
        uint32_t val
) {
    if (oc4_emmc_regs == NULL) {
        return result_err("NULL `oc4_emmc_regs` passed to oc4_emmc_regs_set_data().");
    }
    oc4_emmc_regs->sd_emmc_cmd_dat = val;
    return result_ok();
}

result_t oc4_emmc_regs_set_bus_width_4(
        oc4_emmc_regs_t *oc4_emmc_regs,
        bool val
) {
    if (oc4_emmc_regs == NULL) {
        return result_err("NULL `oc4_emmc_regs` passed to oc4_emmc_regs_use_4_data_lines().");
    }
    oc4_emmc_regs->sd_emmc_cfg |= CFG_BUS_WIDTH_4;
    return result_ok();
}
