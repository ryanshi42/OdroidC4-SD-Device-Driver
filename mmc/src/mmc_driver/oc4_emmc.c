#include "oc4_emmc.h"


result_t oc4_emmc_init(
        sdhci_regs_t *sdhci_regs,
        oc4_gpio_regs_t *oc4_gpio_regs
) {
    if (sdhci_regs == NULL) {
        return result_err("NULL `sdhci_regs` passed to oc4_emmc_init().");
    }
    if (oc4_gpio_regs == NULL) {
        return result_err("NULL `oc4_gpio_regs` passed to oc4_emmc_init().");
    }

    /* ============================
     * Initialising the SDHCI SD card controller on the OC4.
     * ============================ */
    /* Set control0 to zero. */
    res = sdhci_regs_zero_control0(sdhci_regs);
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to zero `control0` in oc4_emmc_init().");
    }
    /* Set control1 to zero. */
    res = sdhci_regs_zero_control1(sdhci_regs);
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to zero `control1` in oc4_emmc_init().");
    }
    /* Reset the complete host circuit */
    res = sdhci_regs_reset_host_circuit(sdhci_regs);
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to reset host circuit in oc4_emmc_init().");
    }
    /* Wait until host circuit has finished resetting. */
    size_t retries_host_circuit = 10000;
    bool is_host_circuit_reset = false;
    do {
        usleep(10); /* Wait for 10 microseconds. */
        res = sdhci_regs_is_host_circuit_reset(
                sdhci_regs,
                &is_host_circuit_reset
        );
        if (result_is_err(res)) {
            return result_err_chain(res, "Failed to check if host circuit was reset in oc4_emmc_init().");
        }
    } while (!is_host_circuit_reset && (retries_host_circuit-- > 0));
    if (!is_host_circuit_reset) {
        return result_err("Host circuit did not reset in oc4_emmc_init().");
    }
    /* Set the Data Timeout to the maximum value. */
    res = sdhci_regs_set_max_data_timeout(sdhci_regs);
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to set max data timeout in oc4_emmc_init().");
    }
    /* Enable the Internal Clock. */
    res = sdhci_regs_enable_internal_clock(sdhci_regs);
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to enable internal clock in oc4_emmc_init().");
    }
    /* Wait 10 microseconds. */
    usleep(10);

    /* Set clock to low-speed setup frequency (400KHz). */
    log_trace("Setting clock to low-speed setup frequency (400KHz).");
    res = sdhci_set_sd_clock(sdhci_regs, 400000);
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to set clock to low-speed setup frequency in oc4_emmc_init().");
    }

    /* Enable interrupts. */
    log_trace("Enabling interrupts.");
    res = sdhci_regs_enable_interrupts(sdhci_regs);
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to enable interrupts in oc4_emmc_init().");
    }

    return result_ok();
}
