#pragma once

#include <stdint.h>

#include "sel4cp_facade.h"
#include "result.h"
#include "bcm_emmc.h"

#include "log.h"
#include "printf.h"
#include "timer_client.h"
#include "sdcard.h"
#include "e2e_test.h"
#include "diskio_test.h"

#define MMC_DRIVER_TO_SERIAL_CLIENT_PUTCHAR_CHANNEL (7)
#define MMC_DRIVER_TO_TIMER_DRIVER_GET_NUM_TICKS_CHANNEL (11)

/**
 * Returns the number of blocks on the SD card.
 * @param ret_val
 * @return
 */
result_t mmc_driver_get_num_blocks(uint64_t *ret_val);

/**
 * Returns the SD cards's block size.
 * @param ret_val
 * @return
 */
result_t mmc_driver_get_block_size(uint16_t *ret_val);

/**
 * Flushes any writes to the SD card.
 * @return
 */
result_t mmc_driver_write_flush(void);
