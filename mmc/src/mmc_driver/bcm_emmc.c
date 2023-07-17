#include "bcm_emmc.h"


result_t bcm_emmc_init(
        sdhci_regs_t *sdhci_regs,
        bcm_gpio_regs_t *bcm_gpio_regs
) {
    log_trace("Called BCM EMMC.");
    sel4cp_dbg_puts("unrecognisable string\n");


    if (sdhci_regs == NULL) {
        return result_err("NULL `sdhci_regs` passed to bcm_emmc_init().");
    }
    if (bcm_gpio_regs == NULL) {
        return result_err("NULL `bcm_gpio_regs` passed to bcm_emmc_init().");
    }
    /* ============================
     * Setting GPIO pin functions to enable the SDHCI SD card controller on the
     * Pi.
     * ============================ */
//    long r = 0;
//    // GPIO_CD
//    r = *GPFSEL4;
//    r &= ~(7 << (7 * 3));
//    *GPFSEL4 = r;
    result_t res;
    res = gpio_driver_fix_resistor(
            bcm_gpio_regs,
            47,
            PULLDOWN
    );
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to set GPIO 47 to pull down in bcm_emmc_init().");
    }
//    r = *GPHEN1;
//    r |= 1 << 15;
//    *GPHEN1 = r;
    res = gpio_driver_set_pin_function(
            bcm_gpio_regs,
            48,
            GPIO_ALTFUNC3
    );
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to set GPIO 48 to altfunc3 in bcm_emmc_init().");
    }
    res = gpio_driver_set_pin_function(
            bcm_gpio_regs,
            49,
            GPIO_ALTFUNC3
    );
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to set GPIO 49 to altfunc3 in bcm_emmc_init().");
    }
    res = gpio_driver_fix_resistor(
            bcm_gpio_regs,
            48,
            PULLDOWN
    );
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to set GPIO 48 to pull down in bcm_emmc_init().");
    }
    res = gpio_driver_fix_resistor(
            bcm_gpio_regs,
            49,
            PULLDOWN
    );
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to set GPIO 49 to pull down in bcm_emmc_init().");
    }
    // GPIO_DAT0, GPIO_DAT1, GPIO_DAT2, GPIO_DAT3
    res = gpio_driver_set_pin_function(
            bcm_gpio_regs,
            50,
            GPIO_ALTFUNC3
    );
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to set GPIO 50 to altfunc3 in bcm_emmc_init().");
    }
    res = gpio_driver_set_pin_function(
            bcm_gpio_regs,
            51,
            GPIO_ALTFUNC3
    );
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to set GPIO 51 to altfunc3 in bcm_emmc_init().");
    }
    res = gpio_driver_set_pin_function(
            bcm_gpio_regs,
            52,
            GPIO_ALTFUNC3
    );
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to set GPIO 52 to altfunc3 in bcm_emmc_init().");
    }
    res = gpio_driver_set_pin_function(
            bcm_gpio_regs,
            53,
            GPIO_ALTFUNC3
    );
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to set GPIO 53 to altfunc3 in bcm_emmc_init().");
    }
    res = gpio_driver_fix_resistor(
            bcm_gpio_regs,
            50,
            PULLDOWN
    );
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to set GPIO 50 to pull down in bcm_emmc_init().");
    }
    res = gpio_driver_fix_resistor(
            bcm_gpio_regs,
            51,
            PULLDOWN
    );
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to set GPIO 51 to pull down in bcm_emmc_init().");
    }
    res = gpio_driver_fix_resistor(
            bcm_gpio_regs,
            52,
            PULLDOWN
    );
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to set GPIO 52 to pull down in bcm_emmc_init().");
    }
    res = gpio_driver_fix_resistor(
            bcm_gpio_regs,
            53,
            PULLDOWN
    );
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to set GPIO 53 to pull down in bcm_emmc_init().");
    }
    /* ============================
     * Initialising the SDHCI SD card controller on the Pi.
     * ============================ */
    /* Set control0 to zero. */
    res = sdhci_regs_zero_control0(sdhci_regs);
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to zero `control0` in bcm_emmc_init().");
    }
    /* Set control1 to zero. */
    res = sdhci_regs_zero_control1(sdhci_regs);
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to zero `control1` in bcm_emmc_init().");
    }
    /* Reset the complete host circuit */
    res = sdhci_regs_reset_host_circuit(sdhci_regs);
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to reset host circuit in bcm_emmc_init().");
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
            return result_err_chain(res, "Failed to check if host circuit was reset in bcm_emmc_init().");
        }
    } while (!is_host_circuit_reset && (retries_host_circuit-- > 0));
    if (!is_host_circuit_reset) {
        return result_err("Host circuit did not reset in bcm_emmc_init().");
    }
    /* Set the Data Timeout to the maximum value. */
    res = sdhci_regs_set_max_data_timeout(sdhci_regs);
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to set max data timeout in bcm_emmc_init().");
    }
    /* Enable the Internal Clock. */
    res = sdhci_regs_enable_internal_clock(sdhci_regs);
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to enable internal clock in bcm_emmc_init().");
    }
    /* Wait 10 microseconds. */
    usleep(10);

    /* Set clock to low-speed setup frequency (400KHz). */
    log_trace("Setting clock to low-speed setup frequency (400KHz).");
    res = sdhci_set_sd_clock(sdhci_regs, 400000);
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to set clock to low-speed setup frequency in bcm_emmc_init().");
    }

    /* Enable interrupts. */
    log_trace("Enabling interrupts.");
    res = sdhci_regs_enable_interrupts(sdhci_regs);
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to enable interrupts in bcm_emmc_init().");
    }

    return result_ok();
}
