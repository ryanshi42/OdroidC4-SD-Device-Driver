
#pragma once

#include "result.h"
#include "bcm_emmc_regs.h"
#include "sleep.h"
#include "sdhci.h"
#include "log.h"
#include "sdhci_result.h"
#include "bcm_gpio_regs.h"
#include "gpio_driver.h"

//typedef struct bcm_emmc bcm_emmc_t;
//struct bcm_emmc {
//    bcm_emmc_regs_t *regs;
//};

/**
 * Initialises the BCM EMMC device.
 * @param bcm_emmc_regs
 * @param bcm_gpio_regs
 * @return
 */
result_t bcm_emmc_init(
        bcm_emmc_regs_t *bcm_emmc_regs,
        bcm_gpio_regs_t *bcm_gpio_regs
);

