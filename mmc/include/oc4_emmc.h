
#pragma once

#include "result.h"
#include "sdhci_regs.h"
#include "sleep.h"
#include "sdhci.h"
#include "log.h"
#include "sdhci_result.h"
#include "oc4_gpio_regs.h"
#include "gpio_driver.h"

/**
 * Initialises the oc4 EMMC device.
 * @param sdhci_regs
 * @param oc4_gpio_regs
 * @return
 */
result_t oc4_emmc_init(
        sdhci_regs_t *sdhci_regs,
        oc4_gpio_regs_t *oc4_gpio_regs
);

