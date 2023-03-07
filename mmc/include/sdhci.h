#pragma once

#include "result.h"
#include "bcm_emmc_regs.h"
#include "sleep.h"
#include "log.h"
#include "arith.h"
#include "sdhci_result.h"
#include "sdhci_cmd.h"

/**
 * Returns the SD clock divisor for the given requested frequency. This is
 * calculated relative to the SD base clock of 41.66667Mhz
 * @param bcm_emmc
 * @param freq Frequency in Hz.
 * @param ret_val The range of divisors returned is 0x3 to 0x3FF.
 * @return
 */
result_t sdhci_get_sd_clock_divisor(
        bcm_emmc_regs_t *bcm_emmc_regs,
        uint32_t freq,
        uint32_t *ret_val
);

/**
 * Sets the SD clock to the specified frequency..
 * @param bcm_emmc
 * @param freq Frequency in Hz.
 * @return
 */
result_t sdhci_set_sd_clock(
        bcm_emmc_regs_t *bcm_emmc_regs,
        uint32_t freq
);

/**
 * Waits for the specified interrupt to be set.
 * @param bcm_emmc_regs
 * @param interrupt_mask
 * @param sdhci_result
 * @return
 */
result_t sdhci_wait_for_interrupt(
        bcm_emmc_regs_t *bcm_emmc_regs,
        uint32_t interrupt_mask,
        sdhci_result_t *sdhci_result
);

/**
 * Waits for cmd_in_progress to be cleared.
 * @param bcm_emmc_regs
 * @param sdhci_result
 * @return
 */
result_t sdhci_wait_for_cmd_in_progress(
        bcm_emmc_regs_t *bcm_emmc_regs,
        sdhci_result_t *sdhci_result
);

/**
 * Waits for data_in_progress to be cleared.
 * @param bcm_emmc_regs
 * @param sdhci_result
 * @return
 */
result_t sdhci_wait_for_data_in_progress(
        bcm_emmc_regs_t *bcm_emmc_regs,
        sdhci_result_t *sdhci_result
);

result_t sdhci_send_cmd(
        bcm_emmc_regs_t *bcm_emmc_regs,
        size_t sdhci_cmd_index,
        sdhci_result_t *sdhci_result
);

