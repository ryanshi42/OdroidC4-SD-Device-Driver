#pragma once

#include <stdint.h>

#include "result.h"
#include "sleep.h"
#include "assert.h"
#include "clock.h"
#include "log.h"
#include "sdhci.h"
#include "bcm_emmc_regs.h"
#include "sdcard.h"

/**
 * Tests if sleep is working.
 * @return
 */
result_t e2e_test_sleep();

/**
 * Simple read/write test on the SD card.
 * @return
 */
result_t e2e_test_read_write_simple(
        bcm_emmc_regs_t *bcm_emmc_regs,
        sdcard_t *sdcard
);

/**
 * Read/write test on the SD card.
 * @return
 * @param bcm_emmc_regs
 * @param sdcard
 * @return
 */
result_t e2e_test_read_write_multiple_blocks(
        bcm_emmc_regs_t *bcm_emmc_regs,
        sdcard_t *sdcard
);
