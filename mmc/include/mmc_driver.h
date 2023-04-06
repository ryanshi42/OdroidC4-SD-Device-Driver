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
#include "sdhci.h"
#include "fatfs_e2e.h"

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

/**
 * Writes blocks to the SD card.
 * @param lba
 * @param num_blocks
 * @param block_size
 * @param dst_buffer
 * @param dst_buffer_len
 * @return
 */
result_t mmc_driver_write_blocks(
        size_t lba,
        size_t num_blocks,
        size_t block_size,
        char *src_buffer,
        size_t src_buffer_len
);

/**
 * Reads blocks from the SD card.
 * @param lba
 * @param num_blocks
 * @param block_size
 * @param dst_buffer
 * @param dst_buffer_len
 * @return
 */
result_t mmc_driver_read_blocks(
        size_t lba,
        size_t num_blocks,
        size_t block_size,
        char *dst_buffer,
        size_t dst_buffer_len
);

