#pragma once

#include "result.h"
#include "bcm_emmc_regs.h"
#include "sleep.h"
#include "log.h"
#include "arith.h"
#include "sdhci_result.h"
#include "sdhci_cmds.h"
#include "sdhci_cmd.h"
#include "cmd_rspns_type.h"
#include "interrupt.h"
#include "cmd_index.h"
#include "sdcard.h"

/**
 * Initialises an SD card.
 * @param bcm_emmc_regs
 * @param sdcard
 * @return
 */
result_t sdhci_card_init_and_id(
        bcm_emmc_regs_t *bcm_emmc_regs,
        sdcard_t *sdcard,
        sdhci_result_t *sdhci_result
);

/**
 * Reads the specified number of blocks from the SD card.
 * @param bcm_emmc_regs
 * @param sdcard
 * @param lba
 * @param num_blocks
 * @param dst_buffer
 * @param sdhci_result
 * @return
 */
result_t sdhci_read_blocks(
        bcm_emmc_regs_t *bcm_emmc_regs,
        sdcard_t *sdcard,
        size_t lba,
        size_t num_blocks,
        char *dst_buffer,
        sdhci_result_t *sdhci_result
);

/**
 * Writes the specified number of blocks to the SD card.
 * @param bcm_emmc_regs
 * @param sdcard
 * @param lba
 * @param num_blocks
 * @param src_buffer
 * @param sdhci_result
 * @return
 */
result_t sdhci_write_blocks(
        bcm_emmc_regs_t *bcm_emmc_regs,
        sdcard_t *sdcard,
        size_t lba,
        size_t num_blocks,
        char *src_buffer,
        sdhci_result_t *sdhci_result
);

/**
 * Transfers specified number of blocks to/from the SD card.
 * @param bcm_emmc_regs
 * @param sdcard
 * @param lba
 * @param num_blocks
 * @param is_write
 * @param buffer
 * @param sdhci_result
 * @return
 */
result_t sdhci_transfer_blocks(
        bcm_emmc_regs_t *bcm_emmc_regs,
        sdcard_t *sdcard,
        size_t lba,
        size_t num_blocks,
        bool is_write,
        char *buffer,
        sdhci_result_t *sdhci_result
);

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

/**
 * Sends the specified command to the SD card.
 * @param bcm_emmc_regs
 * @param sdhci_cmd_index
 * @param arg
 * @param sdcard
 * @param sdhci_result
 * @return
 */
result_t sdhci_send_cmd(
        bcm_emmc_regs_t *bcm_emmc_regs,
        size_t sdhci_cmd_index,
        uint32_t arg,
        sdcard_t *sdcard,
        sdhci_result_t *sdhci_result
);

