#pragma once

#include <stdint.h>

#include "sel4cp_facade.h"
#include "result.h"
#include "bcm_emmc.h"
#include "bcm_emmc_regs.h"
#include "oc4_emmc.h"
#include "oc4_emmc_regs.h"

#include "log.h"
#include "printf.h"
#include "timer_client.h"
#include "sdcard.h"
#include "mmc_driver_e2e.h"
#include "sdhci.h"
#include "sdhci_regs.h"
#include "blk_request_queue.h"
#include "blk_request.h"
#include "blk_response_queue.h"
#include "blk_response.h"
#include "blk_shared_data_buf.h"

#define MMC_DRIVER_TO_SERIAL_CLIENT_PUTCHAR_CHANNEL (7)
#define MMC_DRIVER_TO_TIMER_DRIVER_GET_NUM_TICKS_CHANNEL (11)
#define MMC_DRIVER_TO_FATFS_REQUEST_CHANNEL (16)

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

