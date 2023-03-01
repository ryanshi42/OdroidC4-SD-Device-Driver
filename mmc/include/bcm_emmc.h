
#pragma once

#include "result.h"
#include "bcm_emmc_regs.h"
#include "sleep.h"

typedef struct bcm_emmc bcm_emmc_t;
struct bcm_emmc {
    bcm_emmc_regs_t *regs;
};

/**
 * Initialises the BCM EMMC device.
 * @param bcm_emmc
 * @param bcm_emmc_regs
 * @return
 */
result_t bcm_emmc_init(
        bcm_emmc_t *bcm_emmc,
        bcm_emmc_regs_t *bcm_emmc_regs
);


