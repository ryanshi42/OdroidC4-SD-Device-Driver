
#pragma once

#include "result.h"
#include "sdhci_regs.h"
#include "sleep.h"
#include "sdhci.h"
#include "log.h"
#include "sdhci_result.h"
#include "bcm_gpio_regs.h"
#include "gpio_driver.h"

/**
 * Initialises the BCM EMMC device.
 * @param sdhci_regs
 * @param bcm_gpio_regs
 * @return
 */
result_t bcm_emmc_init(
        sdhci_regs_t *sdhci_regs,
        bcm_gpio_regs_t *bcm_gpio_regs
);

