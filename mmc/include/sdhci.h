#pragma once

#include "result.h"
#include "sdhci_regs.h"
//#include "sdhci_regs.h"
#include "sleep.h"
#include "log.h"
#include "arith.h"
#include "sdhci_result.h"
#include "sdhci_data.h"
#include "sdhci_cmds.h"
#include "sdhci_cmd.h"
#include "cmd_rspns_type.h"
#include "interrupt.h"
#include "cmd_index.h"
#include "sdcard.h"
#include "sdhci_resp_r1.h"
#include "sdhci_resp_r6.h"

/**
 * Initialises an SD card.
 * @param sdhci_regs
 * @param sdcard
 * @return
 */
result_t sdhci_card_init_and_id(
        sdhci_regs_t *sdhci_regs,
        sdcard_t *sdcard,
        sdhci_result_t *sdhci_result
);

/**
 * Sets the Bus Width to its maximum value as specified by the SD Card
 * Configuration Register (SCR), which is stored in our `sdcard`.
 * @param sdhci_regs
 * @param sdcard
 * @param sd_clock_freq_hz
 * @param sdhci_result
 * @return
 */
result_t sdhci_set_max_bus_width(
        sdhci_regs_t *sdhci_regs,
        sdcard_t *sdcard,
        sdhci_result_t *sdhci_result
);

/**
 * Reads the specified number of blocks from the SD card.
 * @param sdhci_regs
 * @param sdcard
 * @param lba
 * @param num_blocks
 * @param block_size The size of each block in bytes.
 * @param dst_buffer The destination buffer.
 * @param dst_buffer_len The length of the destination buffer in bytes.
 * @param sdhci_result
 * @return
 */
result_t sdhci_read_blocks(
        sdhci_regs_t *sdhci_regs,
        sdcard_t *sdcard,
        size_t lba,
        size_t num_blocks,
        size_t block_size,
        char *dst_buffer,
        size_t dst_buffer_len,
        sdhci_result_t *sdhci_result
);

/**
 * Writes the specified number of blocks to the SD card.
 * @param sdhci_regs
 * @param sdcard
 * @param lba
 * @param num_blocks
 * @param block_size The size of each block in bytes.
 * @param src_buffer The source buffer.
 * @param src_buffer_len The length of the source buffer in bytes.
 * @param sdhci_result
 * @return
 */
result_t sdhci_write_blocks(
        sdhci_regs_t *sdhci_regs,
        sdcard_t *sdcard,
        size_t lba,
        size_t num_blocks,
        size_t block_size,
        char *src_buffer,
        size_t src_buffer_len,
        sdhci_result_t *sdhci_result
);

/**
 * Transfers specified number of blocks to/from the SD card.
 * @param sdhci_regs
 * @param sdcard
 * @param lba
 * @param num_blocks
 * @param block_size The size of each block in bytes.
 * @param is_write
 * @param buffer The buffer.
 * @param buffer_len The length of the buffer in bytes.
 * @param sdhci_result
 * @return
 */
result_t sdhci_transfer_blocks(
        sdhci_regs_t *sdhci_regs,
        sdcard_t *sdcard,
        size_t lba,
        size_t num_blocks,
        size_t block_size,
        bool is_write,
        char *buffer,
        size_t buffer_len,
        sdhci_result_t *sdhci_result
);

/**
 * Returns the SD clock divisor for the given requested frequency. This is
 * calculated relative to the SD base clock of 41.66667Mhz
 * @param oc4_emmc
 * @param freq Frequency in Hz.
 * @param ret_val The range of divisors returned is 0x3 to 0x3FF.
 * @return
 */
result_t sdhci_get_sd_clock_divisor(
        sdhci_regs_t *sdhci_regs,
        uint32_t freq,
        uint32_t *ret_val
);

/**
 * Sets the SD clock to the specified frequency..
 * @param oc4_emmc
 * @param freq Frequency in Hz.
 * @return
 */
result_t sdhci_set_sd_clock(
        sdhci_regs_t *sdhci_regs,
        uint32_t freq
);

/**
 * Waits for the specified interrupt to be set.
 * @param sdhci_regs
 * @param interrupt_mask
 * @param sdhci_result
 * @return
 */
result_t sdhci_wait_for_interrupt(
        sdhci_regs_t *sdhci_regs,
        uint32_t interrupt_mask,
        sdhci_result_t *sdhci_result
);

/**
 * Waits for cmd_in_progress to be cleared.
 * @param sdhci_regs
 * @param sdhci_result
 * @return
 */
result_t sdhci_wait_for_cmd_in_progress(
        sdhci_regs_t *sdhci_regs,
        sdhci_result_t *sdhci_result
);

/**
 * Waits for data_in_progress to be cleared.
 * @param sdhci_regs
 * @param sdhci_result
 * @return
 */
result_t sdhci_wait_for_data_in_progress(
        sdhci_regs_t *sdhci_regs,
        sdhci_result_t *sdhci_result
);

/**
 * Sends the specified command to the SD card.
 * @param sdhci_regs
 * @param sdhci_cmd_index
 * @param arg
 * @param sdcard
 * @param sdhci_result
 * @return
 */
result_t sdhci_send_cmd(
        sdhci_data_t *sdhci_data,
        sdhci_regs_t *sdhci_regs,
        size_t sdhci_cmd_index,
        uint32_t arg,
        sdcard_t *sdcard,
        sdhci_result_t *sdhci_result
);

