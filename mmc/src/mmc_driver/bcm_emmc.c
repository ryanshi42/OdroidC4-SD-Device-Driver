#include "bcm_emmc.h"


result_t bcm_emmc_init(
        bcm_emmc_t *bcm_emmc,
        bcm_emmc_regs_t *bcm_emmc_regs
) {
    if (bcm_emmc == NULL) {
        return result_err("NULL `bcm_emmc` passed to bcm_emmc_init().");
    }
    if (bcm_emmc_regs == 0) {
        return result_err("NULL `bcm_emmc_regs` passed to bcm_emmc_init().");
    }
    bcm_emmc->regs = bcm_emmc_regs;
    /* Set control0 to zero. */
    result_t res = bcm_emmc_regs_zero_control0(bcm_emmc->regs);
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to zero `control0` in bcm_emmc_init().");
    }
    /* Set control1 to zero. */
    res = bcm_emmc_regs_zero_control1(bcm_emmc->regs);
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to zero `control1` in bcm_emmc_init().");
    }
    /* Reset the complete host circuit */
    res = bcm_emmc_regs_reset_host_circuit(bcm_emmc->regs);
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to reset host circuit in bcm_emmc_init().");
    }
    /* Wait until host circuit has finished resetting. */
    size_t retries = 10000;
    bool is_host_circuit_reset = false;
    do {
        usleep(10); /* Wait for 10 microseconds. */
        res = bcm_emmc_regs_is_host_circuit_reset(
                bcm_emmc->regs,
                &is_host_circuit_reset
        );
        if (result_is_err(res)) {
            return result_err_chain(res, "Failed to check if host circuit was reset in bcm_emmc_init().");
        }
    } while (!is_host_circuit_reset && (retries-- > 0));
    if (!is_host_circuit_reset) {
        return result_err("Host circuit did not reset in bcm_emmc_init().");
    }
    /* Set the Data Timeout to the maximum value. */
    res = bcm_emmc_regs_set_max_data_timeout(bcm_emmc->regs);
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to set max data timeout in bcm_emmc_init().");
    }
    /* Enable the Internal Clock. */
    res = bcm_emmc_regs_enable_internal_clock(bcm_emmc->regs);
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to enable internal clock in bcm_emmc_init().");
    }
    /* Wait 10 microseconds. */
    usleep(10);

    /* Set clock to low-speed setup frequency (400KHz). */
    log_trace("Setting clock to low-speed setup frequency (400KHz).");
    res = bcm_emmc_set_sd_clock(bcm_emmc, 400000);
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to set clock to low-speed setup frequency in bcm_emmc_init().");
    }

    /* Enable interrupts. */
    log_trace("Enabling interrupts.");
    res = bcm_emmc_regs_enable_interrupts(bcm_emmc->regs);
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to enable interrupts in bcm_emmc_init().");
    }


    return result_ok();
}

result_t bcm_emmc_set_sd_clock(bcm_emmc_t *bcm_emmc, uint32_t freq) {
    if (bcm_emmc == NULL) {
        return result_err("NULL `bcm_emmc` passed to bcm_emmc_set_sd_clock().");
    }
    bcm_emmc_regs_t *bcm_emmc_regs = bcm_emmc->regs;
    size_t retries_busy = 10000;
    bool cmd_or_data_lines_busy = false;
    do {
        usleep(1);
        bool data_lines_busy = false;
        result_t res = bcm_emmc_regs_is_data_lines_busy(
                bcm_emmc_regs,
                &data_lines_busy
        );
        if (result_is_err(res)) {
            return result_err_chain(res, "Failed to check if data lines are busy in bcm_emmc_set_sd_clock().");
        }
        bool cmd_line_busy = false;
        res = bcm_emmc_regs_is_cmd_line_busy(
                bcm_emmc_regs,
                &cmd_line_busy
        );
        if (result_is_err(res)) {
            return result_err_chain(res, "Failed to check if cmd line is busy in bcm_emmc_set_sd_clock().");
        }
        cmd_or_data_lines_busy = data_lines_busy || cmd_line_busy;
    } while(cmd_or_data_lines_busy && (retries_busy-- > 0));
    if (cmd_or_data_lines_busy) {
        return result_err("Timed out waiting for data/cmd lines to be free in bcm_emmc_set_sd_clock().");
    }
    /* Disable SD clock. */
    result_t res = bcm_emmc_regs_disable_sd_clock(bcm_emmc_regs);
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to disable SD clock in bcm_emmc_set_sd_clock().");
    }
    usleep(10);
    /* Get the SD clock divisor we want to set. */
    uint32_t sd_clock_divisor = 0;
    res = bcm_emmc_get_sd_clock_divisor(
            bcm_emmc,
            freq,
            &sd_clock_divisor
    );
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to get SD clock divisor in bcm_emmc_set_sd_clock().");
    }
    /* Set SD Clock Mode to "Divided". */
    res = bcm_emmc_regs_set_sd_clock_mode_as_divided(bcm_emmc_regs);
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to set SD clock mode to \"Divided\" in bcm_emmc_set_sd_clock().");
    }
    /* Set SD Clock Divisor. */
    res = bcm_emmc_regs_set_sd_clock_divisor(
            bcm_emmc_regs,
            sd_clock_divisor
    );
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to set SD clock divisor in bcm_emmc_set_sd_clock().");
    }
    usleep(10);
    /* Enable the SD Clock. */
    res = bcm_emmc_regs_enable_sd_clock(bcm_emmc_regs);
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to enable SD clock in bcm_emmc_set_sd_clock().");
    }
    log_trace("Setting SD clock to %uHz (divisor: %u).", freq, sd_clock_divisor);
    /* Wait for the SD Clock to stabilise. The SDHCI specifies timeout as 150
     * milliseconds (150,000 microseconds) */
    size_t retries_stable = 15000;
    bool is_sd_clock_stable = false;
    do {
        usleep(10);
        res = bcm_emmc_regs_is_sd_clock_stable(
                bcm_emmc_regs,
                &is_sd_clock_stable
        );
        if (result_is_err(res)) {
            return result_err_chain(res, "Failed to check if SD clock is stable in bcm_emmc_set_sd_clock().");
        }
    } while (!is_sd_clock_stable && (retries_stable-- > 0));
    if (!is_sd_clock_stable) {
        return result_err("Timed out waiting for SD clock to be stable in bcm_emmc_set_sd_clock().");
    }
    log_trace("SD clock is stable.");
    return result_ok();
}

result_t bcm_emmc_get_sd_clock_divisor(
        bcm_emmc_t *bcm_emmc,
        uint32_t freq,
        uint32_t *ret_val
) {
    if (bcm_emmc == NULL) {
        return result_err("NULL `bcm_emmc` passed to bcm_emmc_get_sd_clock_divisor().");
    }
    if (freq == 0) {
        return result_err("Zero `freq` passed to bcm_emmc_get_sd_clock_divisor().");
    }
    /* The Pi's SD frequency is always 41.66667Mhz when running bare metal. */
    uint32_t divisor = (41666667 + freq - 1) / freq;
    /* Constrain divisor to max of 0x3FF. */
    if (divisor > 0x3FF) {
        divisor = 0x3FF;
    }
    /* Obtain the controller's Host Controller Spec Version. */
    uint8_t host_controller_spec_version = 0;
    result_t res = bcm_emmc_regs_get_host_controller_spec_version(
            bcm_emmc->regs,
            &host_controller_spec_version
    );
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to get host controller spec version in bcm_emmc_get_sd_clock_divisor().");
    }
    /* If the Host Controller Spec Version is 1 or 2, we're in "8-bit Divided
     * Clock Mode". */
    if (host_controller_spec_version < 2) {
        uint_fast8_t shiftcount = arith_find_most_sig_bit_set(divisor);
        /* Note the offset of shift by 1 (look at the spec) */
        if (shiftcount > 0) shiftcount--;
        /* It's only 8 bits maximum on HOST_SPEC_V2 */
        if (shiftcount > 7) shiftcount = 7;
        /* Version 1 and 2 take power 2 */
        divisor = ((uint32_t) 1 << shiftcount);
    }
    /* TODO: Check this. */
    else if (divisor < 3) {
        divisor = 4;
    }
    *ret_val = divisor;
    return result_ok();
}

