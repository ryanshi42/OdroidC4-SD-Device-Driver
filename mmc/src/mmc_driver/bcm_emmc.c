#include "bcm_emmc.h"


result_t bcm_emmc_init(
        bcm_emmc_regs_t *bcm_emmc_regs
) {
    if (bcm_emmc_regs == 0) {
        return result_err("NULL `bcm_emmc_regs` passed to bcm_emmc_init().");
    }
    /* Set control0 to zero. */
    result_t res = bcm_emmc_regs_zero_control0(bcm_emmc_regs);
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to zero `control0` in bcm_emmc_init().");
    }
    /* Set control1 to zero. */
    res = bcm_emmc_regs_zero_control1(bcm_emmc_regs);
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to zero `control1` in bcm_emmc_init().");
    }
    /* Reset the complete host circuit */
    res = bcm_emmc_regs_reset_host_circuit(bcm_emmc_regs);
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to reset host circuit in bcm_emmc_init().");
    }
    /* Wait until host circuit has finished resetting. */
    size_t retries_host_circuit = 10000;
    bool is_host_circuit_reset = false;
    do {
        usleep(10); /* Wait for 10 microseconds. */
        res = bcm_emmc_regs_is_host_circuit_reset(
                bcm_emmc_regs,
                &is_host_circuit_reset
        );
        if (result_is_err(res)) {
            return result_err_chain(res, "Failed to check if host circuit was reset in bcm_emmc_init().");
        }
    } while (!is_host_circuit_reset && (retries_host_circuit-- > 0));
    if (!is_host_circuit_reset) {
        return result_err("Host circuit did not reset in bcm_emmc_init().");
    }
    /* Set the Data Timeout to the maximum value. */
    res = bcm_emmc_regs_set_max_data_timeout(bcm_emmc_regs);
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to set max data timeout in bcm_emmc_init().");
    }
    /* Enable the Internal Clock. */
    res = bcm_emmc_regs_enable_internal_clock(bcm_emmc_regs);
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to enable internal clock in bcm_emmc_init().");
    }
    /* Wait 10 microseconds. */
    usleep(10);

    /* Set clock to low-speed setup frequency (400KHz). */
    log_trace("Setting clock to low-speed setup frequency (400KHz).");
    res = sdhci_set_sd_clock(bcm_emmc_regs, 400000);
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to set clock to low-speed setup frequency in bcm_emmc_init().");
    }

    /* Enable interrupts. */
    log_trace("Enabling interrupts.");
    res = bcm_emmc_regs_enable_interrupts(bcm_emmc_regs);
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to enable interrupts in bcm_emmc_init().");
    }

    return result_ok();
}
