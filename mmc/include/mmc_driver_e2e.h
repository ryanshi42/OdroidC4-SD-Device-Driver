#pragma once

#include <stdint.h>

#include "result.h"
#include "sleep.h"
#include "assert.h"
#include "clock.h"
#include "log.h"
#include "sdhci.h"
#include "sdhci_regs.h"
#include "sdcard.h"

/**
 * Tests if sleep is working.
 * @return
 */
result_t mmc_driver_e2e_sleep();

/**
 * Simple read/write test on the SD card.
 * @param sdhci_regs
 * @param sdcard
 * @return
 */
result_t mmc_driver_e2e_read_write_simple(
        sdhci_regs_t *sdhci_regs,
        sdcard_t *sdcard
);

/**
 * Read/write test on the SD card.
 * @param sdhci_regs
 * @param sdcard
 * @return
 */
result_t mmc_driver_e2e_read_write_multiple_blocks(
        sdhci_regs_t *sdhci_regs,
        sdcard_t *sdcard
);

/**
 * Tests the SD card CSD register.
 * @param sdcard
 * @return
 */
result_t mmc_driver_e2e_sdcard_card_specific_data(sdcard_t *sdcard);

