#include "bcm_emmc.h"

/**
 * Sets the SD clock to the specified frequency..
 * @param bcm_emmc
 * @param freq Frequency in Hz.
 * @return
 */
static result_t bcm_emmc_set_sd_clock(bcm_emmc_t *bcm_emmc, uint32_t freq);

static result_t bcm_emmc_set_sd_clock(bcm_emmc_t *bcm_emmc, uint32_t freq) {
    if (bcm_emmc == NULL) {
        return result_err("NULL `bcm_emmc` passed to bcm_emmc_set_sd_clock().");
    }
    size_t retries = 10000;
    bool cmd_or_data_lines_busy = false;
    do {
        usleep(1);
        bool data_lines_busy = false;
        result_t res = bcm_emmc_regs_is_data_lines_busy(
                bcm_emmc->regs,
                &data_lines_busy
        );
        if (result_is_err(res)) {
            return result_err_chain(res, "Failed to check if data lines are busy in bcm_emmc_set_sd_clock().");
        }
        bool cmd_line_busy = false;
        res = bcm_emmc_regs_is_cmd_line_busy(
                bcm_emmc->regs,
                &cmd_line_busy
        );
        if (result_is_err(res)) {
            return result_err_chain(res, "Failed to check if cmd line is busy in bcm_emmc_set_sd_clock().");
        }
        cmd_or_data_lines_busy = data_lines_busy || cmd_line_busy;
    } while(cmd_or_data_lines_busy && (retries-- > 0));
    if (cmd_or_data_lines_busy) {
        return result_err("Timed out waiting for data/cmd lines to be free in bcm_emmc_set_sd_clock().");
    }
    /* Disable SD clock. */
    result_t res = bcm_emmc_regs_disable_sd_clock(bcm_emmc->regs);
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to disable SD clock in bcm_emmc_set_sd_clock().");
    }
    usleep(10);


    return result_ok();
}

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
    bcm_emmc_regs_set_max_data_timeout(bcm_emmc->regs);
    /* Enable the Internal Clock. */
    bcm_emmc_regs_enable_internal_clock(bcm_emmc->regs);
    /* Wait 10 microseconds. */
    usleep(10);

    /* Set clock to low-speed setup frequency (400KHz). */
    res = bcm_emmc_set_sd_clock(bcm_emmc, 400000);
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to set clock to low-speed setup frequency in bcm_emmc_init().");
    }

    return result_ok();
}


