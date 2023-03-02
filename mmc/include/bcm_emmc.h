
#pragma once

#include "result.h"
#include "bcm_emmc_regs.h"
#include "sleep.h"
#include "arith.h"

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

/**
 * Sets the SD clock to the specified frequency..
 * @param bcm_emmc
 * @param freq Frequency in Hz.
 * @return
 */
result_t bcm_emmc_set_sd_clock(bcm_emmc_t *bcm_emmc, uint32_t freq);

/**
 * Returns the SD clock divisor for the given requested frequency. This is
 * calculated relative to the SD base clock of 41.66667Mhz
 * @param bcm_emmc
 * @param freq Frequency in Hz.
 * @param ret_val The range of divisors returned is 0x3 to 0x3FF.
 * @return
 */
result_t bcm_emmc_get_sd_clock_divisor(
        bcm_emmc_t *bcm_emmc,
        uint32_t freq,
        uint32_t *ret_val
);
