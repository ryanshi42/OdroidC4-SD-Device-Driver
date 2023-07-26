#include "sdhci.h"

result_t sdhci_card_init_and_id(
        sdhci_regs_t *sdhci_regs,
        sdcard_t *sdcard,
        sdhci_result_t *sdhci_result
) {

    //TODO: fix the sdhci card and init function
    // return 0;

    //? Steps in initilaising SD card
    //? Send Go Idle command
    //? Send If Cond command

    sel4cp_dbg_puts("Beginning SDHCI init...");

    if (sdhci_regs == NULL) {
        sel4cp_dbg_puts("SDHCI regs...");
        return result_err("sdhci_regs is NULL in sdhci_card_init_and_id().");
    }
    if (sdcard == NULL) {
        sel4cp_dbg_puts("SDHCI card...");
        return result_err("sdcard is NULL in sdhci_card_init_and_id().");
    }
    if (sdhci_result == NULL) {
        sel4cp_dbg_puts("SDHCI result...");
        return result_err("sdhci_result is NULL in sdhci_card_init_and_id().");
    }
    *sdhci_result = SD_ERROR;
    result_t res;

    //? (https://electronics.stackexchange.com/questions/77417/what-is-the-correct-command-sequence-for-microsd-card-initialization-in-spi)

    /* Sending GO_IDLE command. */
    sel4cp_dbg_puts("Sending GO_IDLE (CMD0) command...");
    sdhci_result_t sdhci_res_go_idle;
    res = sdhci_send_cmd(
            sdhci_regs,
            IDX_GO_IDLE_STATE,
            0,
            sdcard,
            &sdhci_res_go_idle
    );
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to send `IDX_GO_IDLE_STATE` in sdhci_card_init_and_id().");
    }
    sel4cp_dbg_puts("\n\n Finished setting GO_IDLE");


    /* Sending SEND_IF_COND,0x000001AA (CMD8) voltage range 0x1 check pattern 0xAA.
     * TODO: If voltage range and check pattern don't match, look for older card. */
    sel4cp_dbg_puts("Sending IF_COND (CMD8) command...");
    sdhci_result_t sdhci_res_if_cond;
    res = sdhci_send_cmd(
            sdhci_regs,
            IDX_SEND_IF_COND,
            0x000001AA,
            sdcard,
            &sdhci_res_if_cond
    );
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to send `IDX_SEND_IF_COND` in sdhci_card_init_and_id().");
    }
    sel4cp_dbg_puts("\n\n Finished setting IF_COND");


    /* If card responded with voltage and check pattern, resolve voltage and
     * check for high capacity card with IDX_APP_SEND_OP_COND. */

    //! Current bug, here!

    size_t retries = 7;
    bool has_powered_up = false;
    do {
        usleep(400000);
        res = sdhci_send_cmd(
                sdhci_regs,
                IDX_APP_SEND_OP_COND,
                ACMD41_ARG_HC,
                sdcard,
                sdhci_result
        );
        if (result_is_err(res)) {
            return result_err_chain(res, "Failed to send `IDX_APP_SEND_OP_COND` in sdhci_card_init_and_id().");
        }
        res = sdcard_has_powered_up(sdcard, &has_powered_up);
        if (result_is_err(res)) {
            return result_err_chain(res, "Failed to check if card has powered up in sdhci_card_init_and_id().");
        }
    } while (!has_powered_up && (retries-- > 0));

    sel4cp_dbg_puts("\n\n Finished setting IF_COND");

    /* If the card still hasn't powered up, we timeout. */
    if (!has_powered_up) {
        sel4cp_dbg_puts("\n\n Timeout, could not power up \n\n");

        *sdhci_result = SD_TIMEOUT;
        return result_err("EMMC card did not power up in sdhci_card_init_and_id().");
    }

    /* Check voltage */
    sel4cp_dbg_puts("Checking card voltage is 3v3...");
    bool has_correct_voltage = false;
    res = sdcard_is_voltage_3v3(sdcard, &has_correct_voltage);
    if (result_is_err(res)) {
        return result_err("Failed to check if card has correct voltage in sdhci_card_init_and_id().");
    }
    if (!has_correct_voltage) {
        *sdhci_result = SD_ERROR_VOLTAGE;
        return result_err("EMMC card does not have correct voltage in sdhci_card_init_and_id().");
    }
    sel4cp_dbg_puts("Voltage good.");

    /* Check card capacity. */
    sel4cp_dbg_puts("Checking card type...");
    bool is_high_capacity = false;
    res = sdcard_is_high_capacity(sdcard, &is_high_capacity);
    if (result_is_err(res)) {
        return result_err("Failed to check if card is high capacity in sdhci_card_init_and_id().");
    }
    if (is_high_capacity) {
        res = sdcard_set_type(sdcard, SD_TYPE_2_HC);
        if (result_is_err(res)) {
            return result_err_chain(res, "Failed to set card type to SD_TYPE_2_HC in sdhci_card_init_and_id().");
        }
        sel4cp_dbg_puts("Card high capacity (HC).");
    } else {
        res = sdcard_set_type(sdcard, SD_TYPE_2_SC);
        if (result_is_err(res)) {
            return result_err_chain(res, "Failed to set card type to SD_TYPE_2_SC in sdhci_card_init_and_id().");
        }
        sel4cp_dbg_puts("Card standard capacity (SC).");
    }

    /* Send ALL_SEND_CID (CMD2). */
    sel4cp_dbg_puts("Sending ALL_SEND_CID (CMD2) command...");
    res = sdhci_send_cmd(
            sdhci_regs,
            IDX_ALL_SEND_CID,
            0,
            sdcard,
            sdhci_result
    );
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to send `IDX_ALL_SEND_CID` in sdhci_card_init_and_id().");
    }

    /* Send SEND_REL_ADDR (CMD3). */
    /* TODO: In theory, loop back to SEND_IF_COND to find additional cards. */
    sel4cp_dbg_puts("Sending SEND_REL_ADDR (CMD3) command...");
    res = sdhci_send_cmd(
            sdhci_regs,
            IDX_SEND_REL_ADDR,
            0,
            sdcard,
            sdhci_result
    );
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to send `IDX_SEND_REL_ADDR` in sdhci_card_init_and_id().");
    }
    /* SD card will have an RCA at this point. */


    //! SDHCI CONTROLLER HERE

    /* ===================================
     * Setting SD Clock Frequency to full-speed.
     * =================================== */
    //! RPi3 specific code
    // res = sdhci_set_sd_clock(
    //         sdhci_regs,
    //         25000000
    // );
    // if (result_is_err(res)) {
    //     return result_err_chain(res, "Failed to set SD clock frequency to full-speed in sdhci_card_init_and_id().");
    // }
    // sel4cp_dbg_puts("Finished setting SD clock to full-speed frequency (25GHz).");

    /* ===================================
     * Populating `sdcard` with data from CSD Card Specific Data Register (CSD):
     * =================================== */

    /* Get the sdcard's RCA. */
    sel4cp_dbg_puts("Obtaining SD card's Relative Card Address (RCA)...");
    uint32_t rca = 0;
    res = sdcard_get_rca(sdcard, &rca);
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to get card's RCA in sdhci_card_init_and_id().");
    }

    sel4cp_dbg_puts("Sending SEND_CSD (CMD9)...");
    res = sdhci_send_cmd(
            sdhci_regs,
            IDX_SEND_CSD,
            rca,
            sdcard,
            sdhci_result
    );
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to send `IDX_CARD_SELECT` in sdhci_card_init_and_id().");
    }

    /* ===================================
     * Populating `sdcard` with data from SD Card Configuration Register (SCR):
     * =================================== */

    /* Call the `IDX_CARD_SELECT` CMD using the RCA. */
    /* TODO: Check card_is_locked status in the R1 response from CMD7 [bit 25],
     * if so, use CMD42 to unlock CMD42 structure [4.3.7] same as a single block
     * write; data block includes PWD setting mode, PWD len, PWD data.*/
    sel4cp_dbg_puts("Sending CARD_SELECT (CMD7)...");
    res = sdhci_send_cmd(
            sdhci_regs,
            IDX_CARD_SELECT,
            rca,
            sdcard,
            sdhci_result
    );
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to send `IDX_CARD_SELECT` in sdhci_card_init_and_id().");
    }

    /* Reading from the SD Card Configuration Register (SCR).
     * SEND_SCR command is like a READ_SINGLE but for a block of 8 bytes.*/
    sel4cp_dbg_puts("Reading from the SD Card Configuration Register (SCR)...");
    /* Wait for any data operation that might be in progress before reading the block. */
    res = sdhci_wait_for_data_in_progress(sdhci_regs, sdhci_result);
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to wait for data in progress in sdhci_card_init_and_id().");
    }
    /* Set BLKSIZECNT to 1 block of 8 bytes. */
    // sel4cp_dbg_puts("Setting BLKSIZECNT to 1 block of 8 bytes...");

    //! There are no such fields in the Odroid C4
    // res = sdhci_regs_set_block_count(sdhci_regs, 1);
    // if (result_is_err(res)) {
    //     return result_err_chain(res, "Failed to set block count to 1 in sdhci_card_init_and_id().");
    // }
    // res = sdhci_regs_set_block_size(sdhci_regs, 8);
    // if (result_is_err(res)) {
    //     return result_err_chain(res, "Failed to set block size to 8 in sdhci_card_init_and_id().");
    // }
    /* Get the SCR by sending the `SEND_SCR` command. We need the SCR to figure
     * out what the allowed bus widths are. */
    sel4cp_dbg_puts("Sending SEND_SCR (CMD51) command...");
    res = sdhci_send_cmd(
            sdhci_regs,
            IDX_SEND_SCR,
            0,
            sdcard,
            sdhci_result
    );
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to send `IDX_SEND_SCR` in sdhci_card_init_and_id().");
    }
    /* Wait until we're ready to read. */
    res = sdhci_wait_for_interrupt(
            sdhci_regs,
            INT_READ_RDY,
            sdhci_result
    );
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to wait for `INT_READ_RDY` in sdhci_card_init_and_id().");
    }
    /* Start reading SCR from EMMC. */
    int num_scr_reads = 0;
    int num_scr_retries = 100000;
    bool is_read_ready = false;
    do {
        // res = sdhci_regs_is_read_ready(sdhci_regs, &is_read_ready);
        // if (result_is_err(res)) {
        //     return result_err_chain(res, "Failed to check if read ready in sdhci_card_init_and_id().");
        // }
        is_read_ready = true;
        if (is_read_ready) {
            /* Read the SCR from the data register. */
            uint32_t data = 0;
            res = sdhci_regs_get_data(sdhci_regs, &data);
            if (result_is_err(res)) {
                return result_err_chain(res, "Failed to get data in sdhci_card_init_and_id().");
            }
            if (num_scr_reads == 0) {
                /* Save low portion of SCR register to sdcard. */
                res = sdcard_set_scr_raw32_lo(sdcard, data);
                if (result_is_err(res)) {
                    return result_err_chain(res, "Failed to set SCR raw32 lo in sdhci_card_init_and_id().");
                }
            } else {
                /* Save high portion of SCR register to sdcard. */
                res = sdcard_set_scr_raw32_hi(sdcard, data);
                if (result_is_err(res)) {
                    return result_err_chain(res, "Failed to set SCR raw32 hi in sdhci_card_init_and_id().");
                }
            }
            num_scr_reads++;
        } else {
            usleep(1);
            if (--num_scr_retries == 0) {
                break;
            }
        }
    } while (num_scr_reads < 2);
    if (num_scr_reads < 2) {
        *sdhci_result = SD_TIMEOUT;
        return result_err("Failed to read SCR in sdhci_card_init_and_id().");
    }

    *sdhci_result = SD_OK;
    return result_ok();
}








result_t sdhci_set_max_bus_width(
        sdhci_regs_t *sdhci_regs,
        sdcard_t *sdcard,
        sdhci_result_t *sdhci_result
) {
    if (sdhci_regs == NULL) {
        return result_err("NULL `sdhci_regs` in sdhci_set_max_bus_width().");
    }
    if (sdcard == NULL) {
        return result_err("NULL `sdcard` in sdhci_set_max_bus_width().");
    }
    if (sdhci_result == NULL) {
        return result_err("NULL `sdhci_result` in sdhci_set_max_bus_width().");
    }
    *sdhci_result = SD_ERROR;
    result_t res;

    /* Get the sdcard's RCA. */
    sel4cp_dbg_puts("Obtaining SD card's Relative Card Address (RCA)...");
    uint32_t rca = 0;
    res = sdcard_get_rca(sdcard, &rca);
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to get card's RCA in sdhci_set_max_bus_width().");
    }
    /* Check the bus width. */
    bool is_bus_width_4_bit = false;
    res = sdcard_is_bus_width_4(sdcard, &is_bus_width_4_bit);
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to check if bus width is 4 bit in sdhci_set_max_bus_width().");
    }
    /* Send APP_SET_BUS_WIDTH (ACMD6) command. If supported, set 4 bit bus width
     * and update the CONTROL0 register. */
    if (is_bus_width_4_bit) {
        sel4cp_dbg_puts("Setting bus width to 4 bit...");
        res = sdhci_send_cmd(
                sdhci_regs,
                IDX_SET_BUS_WIDTH,
                rca | 2,
                sdcard,
                sdhci_result
        );
        if (result_is_err(res)) {
            return result_err_chain(res, "Failed to send `IDX_SET_BUS_WIDTH` in sdhci_set_max_bus_width().");
        }
        /* Sets the bus width to 4 bit. */
        res = sdhci_regs_set_bus_width_4(
                sdhci_regs,
                true
        );
        if (result_is_err(res)) {
            return result_err_chain(res, "Failed to use 4 data lines in sdhci_set_max_bus_width().");
        }
    }
    *sdhci_result = SD_OK;
    return result_ok();
}

result_t sdhci_read_blocks(
        sdhci_regs_t *sdhci_regs,
        sdcard_t *sdcard,
        size_t lba,
        size_t num_blocks,
        size_t block_size,
        char *dst_buffer,
        size_t dst_buffer_len,
        sdhci_result_t *sdhci_result
) {
    if (sdhci_regs == NULL) {
        return result_err("NULL `sdhci_regs` passed to sdhci_read_blocks().");
    }
    if (sdcard == NULL) {
        return result_err("NULL `sdcard` passed to sdhci_read_blocks().");
    }
    if (dst_buffer == NULL) {
        return result_err("NULL `dst_buffer` passed to sdhci_read_blocks().");
    }
    if (sdhci_result == NULL) {
        return result_err("NULL `sdhci_result` passed to sdhci_read_blocks().");
    }
    *sdhci_result = SD_ERROR;
    /* Transfer blocks for reading from the SD Card. */
    return sdhci_transfer_blocks(
            sdhci_regs,
            sdcard,
            lba,
            num_blocks,
            block_size,
            false,
            dst_buffer,
            dst_buffer_len,
            sdhci_result
    );
}

result_t sdhci_write_blocks(
        sdhci_regs_t *sdhci_regs,
        sdcard_t *sdcard,
        size_t lba,
        size_t num_blocks,
        size_t block_size,
        char *src_buffer,
        size_t src_buffer_len,
        sdhci_result_t *sdhci_result
) {
    if (sdhci_regs == NULL) {
        return result_err("NULL `sdhci_regs` passed to sdhci_write_blocks().");
    }
    if (sdcard == NULL) {
        return result_err("NULL `sdcard` passed to sdhci_write_blocks().");
    }
    if (src_buffer == NULL) {
        return result_err("NULL `src_buffer` passed to sdhci_write_blocks().");
    }
    if (sdhci_result == NULL) {
        return result_err("NULL `sdhci_result` passed to sdhci_write_blocks().");
    }
    *sdhci_result = SD_ERROR;
    sel4cp_dbg_puts("\n\nSDHCI write block checks finished successfully.\n\n");

    /* Transfer blocks for writing to the SD Card. */
    return sdhci_transfer_blocks(
            sdhci_regs,
            sdcard,
            lba,
            num_blocks,
            block_size,
            true,
            src_buffer,
            src_buffer_len,
            sdhci_result
    );
}

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
) {
    sel4cp_dbg_puts("\n\nBeginning blocks transfer\n\n");

    if (sdhci_regs == NULL) {
        return result_err("NULL `sdhci_regs` passed to sdhci_transfer_blocks().");
    }
    if (sdcard == NULL) {
        return result_err("NULL `sdcard` passed to sdhci_transfer_blocks().");
    }
    if (buffer == NULL) {
        return result_err("NULL `buffer` passed to sdhci_transfer_blocks().");
    }
    if (sdhci_result == NULL) {
        return result_err("NULL `sdhci_result` passed to sdhci_transfer_blocks().");
    }
    *sdhci_result = SD_ERROR;
    sel4cp_dbg_puts("\n\n SDHCI transfer blocks check complete\n\n");

    /* All buffers must be word-aligned i.e. multiples of 4 bytes (32 bits)
     * since we'll be transferring to/from the 32 bit EMMC "data" register in 32
     * bit chunks. */
    if (buffer_len % sizeof(uint32_t) != 0) {
        return result_err("Buffer length is not a multiple of 4 in sdhci_transfer_blocks().");
    }

    /* All buffers must be the same size as the amount of data we are transferring. */
    if (buffer_len != num_blocks * block_size) {
        return result_err("Buffer length is not equal to the number of blocks times the block size in sdhci_transfer_blocks().");
    }
    sel4cp_dbg_puts("\n\nBuffer length is valid\n\n");

    result_t res;
    /* Check the SD card type. */
    bool is_sdcard_type_unknown = true;
    res = sdcard_is_type_unknown(sdcard, &is_sdcard_type_unknown);
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to check if SD Card type is unknown in sdhci_transfer_blocks().");
    }
    sel4cp_dbg_puts("\n\nSD card type is known1\n\n");

    if (is_sdcard_type_unknown) {
        *sdhci_result = SD_NO_RESP;
        return result_err("SD Card type is unknown in sdhci_transfer_blocks().");
    }

    sel4cp_dbg_puts("\n\nSD card type is known2\n\n");

    /* Wait for any data command in progress. */
    res = sdhci_wait_for_data_in_progress(sdhci_regs, sdhci_result);
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to wait for data in progress in sdhci_transfer_blocks().");
    }

    sel4cp_dbg_puts("\n\nSD card can wait for data in progress\n\n");

    /* Determine the interrupt and command values for the transfer. */
    int ready_interrupt;
    int transfer_cmd;
    if (is_write) {
        ready_interrupt = INT_WRITE_RDY;
        if (num_blocks == 1) {
            transfer_cmd = IDX_WRITE_SINGLE;
        } else {
            transfer_cmd = IDX_WRITE_MULTI;
        }
    } else {
        ready_interrupt = INT_READ_RDY;
        if (num_blocks == 1) {
            transfer_cmd = IDX_READ_SINGLE;
        } else {
            transfer_cmd = IDX_READ_MULTI;
        }
    }
    /* Check to see if the card supports multi block transfers. */
    bool is_set_block_count_cmd_supported = false;
    res = sdcard_is_set_block_count_cmd_supported(sdcard, &is_set_block_count_cmd_supported);
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to check if SD Card supports multi block transfers in sdhci_transfer_blocks().");
    }

    sel4cp_dbg_puts("\n\nSD card can support multi block transfers\n\n");

    /* If more than one block to transfer, and the card supports it, send
     * SET_BLOCK_COUNT command to indicate the number of blocks to transfer. */
    if (num_blocks > 1 && is_set_block_count_cmd_supported) {
        res = sdhci_send_cmd(
                sdhci_regs,
                IDX_SET_BLOCKCNT,
                num_blocks,
                sdcard,
                sdhci_result
        );
        if (result_is_err(res)) {
            return result_err_chain(res, "Failed to send SET_BLOCKCNT command in sdhci_transfer_blocks().");
        }
    }

    sel4cp_dbg_puts("\n\nSent block count command\n\n");

    /* Address is different depending on the card type.
     * HC pass address as block # so just pass it through.
     * SC pass address so need to multiply by 512 which is shift left 9. */
    uint32_t block_addr = 0;
    bool is_type_standard_capacity = false;
    res = sdcard_is_type_standard_capacity(sdcard, &is_type_standard_capacity);
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to check if SD Card is standard capacity in sdhci_transfer_blocks().");
    }
    if (is_type_standard_capacity) {
        block_addr = (lba << 9);
    } else {
        block_addr = lba;
    }

    sel4cp_dbg_puts("\n\nSDHCI standard capacity\n\n");

    /* Set BLKSIZECNT to number of blocks * 512 bytes, send the read or write command.
     * Once the data transfer has started and the TM_BLKCNT_EN bit in the CMDTM
     * register is set the EMMC module automatically decreases the BLKCNT value
     * as the data blocks are transferred and stops the transfer once BLKCNT
     * reaches 0.
     * TODO: TM_AUTO_CMD12 - is this needed?  What effect does it have? */
    // res = sdhci_regs_set_block_count(sdhci_regs, num_blocks);
    // if (result_is_err(res)) {
    //     return result_err_chain(res, "Failed to set block count to 1 in sdhci_card_init_and_id().");
    // }
    // res = sdhci_regs_set_block_size(sdhci_regs, block_size);
    // if (result_is_err(res)) {
    //     return result_err_chain(res, "Failed to set block size to 8 in sdhci_card_init_and_id().");
    // }
    /* Send the Transfer Command. */
    res = sdhci_send_cmd(
            sdhci_regs,
            transfer_cmd,
            block_addr,
            sdcard,
            sdhci_result
    );
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to send transfer command in sdhci_transfer_blocks().");
    }
    /* Transfer all blocks. */
    size_t blocks_done = 0;
    while (blocks_done < num_blocks) {
        /* Wait for ready interrupt for the next block. */
        res = sdhci_wait_for_interrupt(
                sdhci_regs,
                ready_interrupt,
                sdhci_result
        );
        if (result_is_err(res)) {
            return result_err_chain(res, "Failed to wait for ready interrupt in sdhci_transfer_blocks().");
        }
        /* Loop through the block 4 bytes (32 bits) at a time. */
        for (size_t i = 0; i < (block_size / sizeof(uint32_t)); i++) {
            if (is_write) {
                res = sdhci_regs_set_data(sdhci_regs, ((uint32_t *) buffer)[i]);
                if (result_is_err(res)) {
                    return result_err_chain(res, "Failed to set data in sdhci_transfer_blocks().");
                }
            } else {
                res = sdhci_regs_get_data(sdhci_regs, &((uint32_t *) buffer)[i]);
                if (result_is_err(res)) {
                    return result_err_chain(res, "Failed to get data in sdhci_transfer_blocks().");
                }
            }
        }
        blocks_done++;
        buffer += block_size;
    }

    /* If not all bytes were read/written, the operation timed out. */
    if (blocks_done != num_blocks) {
        *sdhci_result = SD_TIMEOUT;
        if (!is_write && num_blocks > 1) {
            res = sdhci_send_cmd(
                    sdhci_regs,
                    IDX_STOP_TRANS,
                    0,
                    sdcard,
                    sdhci_result
            );
            if (result_is_err(res)) {
                return result_err_chain(res, "Failed to send STOP_TRANSMISSION command in sdhci_transfer_blocks().");
            }
        }
    }
    sel4cp_dbg_puts("\n\nAll blocks transferred\n\n");

    /* For a write operation, ensure DATA_DONE interrupt before we stop transmission. */
    if (is_write) {
        res = sdhci_wait_for_interrupt(
                sdhci_regs,
                INT_DATA_DONE,
                sdhci_result
        );
        if (result_is_err(res)) {
            return result_err_chain(res, "Failed to wait for data done interrupt in sdhci_transfer_blocks().");
        }
    }
    sel4cp_dbg_puts("\n\n Data done interrupt correctly set\n\n");

    /* For a multi-block operation, if SET_BLOCKCNT is not supported, we need to indicate
     * that there are no more blocks to be transferred. */
    if (num_blocks > 1 && !is_set_block_count_cmd_supported) {
        res = sdhci_send_cmd(
                sdhci_regs,
                IDX_STOP_TRANS,
                0,
                sdcard,
                sdhci_result
        );
        if (result_is_err(res)) {
            return result_err_chain(res, "Failed to send STOP_TRANSMISSION command in sdhci_transfer_blocks().");
        }
    }
    sel4cp_dbg_puts("\n\n Transmission stpoped successfully \n\n");

    *sdhci_result = SD_OK;
    return result_ok();
}

result_t sdhci_get_sd_clock_divisor(
        sdhci_regs_t *sdhci_regs,
        uint32_t freq,
        uint32_t *ret_val
) {
    if (sdhci_regs == NULL) {
        return result_err("NULL `sdhci_regs` passed to sdhci_get_sd_clock_divisor().");
    }
    if (freq == 0) {
        return result_err("Zero `freq` passed to sdhci_get_sd_clock_divisor().");
    }
    if (ret_val == NULL) {
        return result_err("NULL `ret_val` passed to sdhci_get_sd_clock_divisor().");
    }
    /* The Pi's SD frequency is always 41.66667Mhz when running bare metal. */
    uint32_t divisor = (41666667 + freq - 1) / freq;
    /* Constrain divisor to max of 0x3FF. */
    if (divisor > 0x3FF) {
        divisor = 0x3FF;
    }

    //! Raspberry Pi Specific things
    // /* Obtain the controller's Host Controller Spec Version. */
    // uint8_t host_controller_spec_version = 0;
    // result_t res = sdhci_regs_get_host_controller_spec_version(
    //         sdhci_regs,
    //         &host_controller_spec_version
    // );
    // if (result_is_err(res)) {
    //     return result_err_chain(res, "Failed to get host controller spec version in sdhci_get_sd_clock_divisor().");
    // }
    // /* If the Host Controller Spec Version is 1 or 2, we're in "8-bit Divided
    //  * Clock Mode". */
    // if (host_controller_spec_version < 2) {
    //     uint_fast8_t shiftcount = arith_find_most_sig_bit_set(divisor);
    //     /* Note the offset of shift by 1 (look at the spec) */
    //     if (shiftcount > 0) shiftcount--;
    //     /* It's only 8 bits maximum on HOST_SPEC_V2 */
    //     if (shiftcount > 7) shiftcount = 7;
    //     /* Version 1 and 2 take power 2 */
    //     divisor = ((uint32_t) 1 << shiftcount);
    // }
    /* TODO: Check this. */
    else if (divisor < 3) {
        divisor = 4;
    }
    *ret_val = divisor;
    return result_ok();
}

//! Raspberry Pi specific function
result_t sdhci_set_sd_clock(sdhci_regs_t *sdhci_regs, uint32_t freq) {
    if (sdhci_regs == NULL) {
        return result_err("NULL `sdhci_regs` passed to sdhci_set_sd_clock().");
    }
    size_t retries_busy = 10000;
    bool cmd_or_data_lines_busy = false;
    do {
        usleep(1);
        bool data_lines_busy = false;
        result_t res = sdhci_regs_is_data_lines_busy(
                sdhci_regs,
                &data_lines_busy
        );
        if (result_is_err(res)) {
            return result_err_chain(res, "Failed to check if data lines are busy in sdhci_set_sd_clock().");
        }
        bool cmd_line_busy = false;
        res = sdhci_regs_is_cmd_line_busy(
                sdhci_regs,
                &cmd_line_busy
        );
        if (result_is_err(res)) {
            return result_err_chain(res, "Failed to check if cmd line is busy in sdhci_set_sd_clock().");
        }
        cmd_or_data_lines_busy = data_lines_busy | cmd_line_busy;
    } while(cmd_or_data_lines_busy && (retries_busy-- > 0));
    if (cmd_or_data_lines_busy) {
        return result_err("Timed out waiting for data/cmd lines to be free in sdhci_set_sd_clock().");
    }
    /* Disable SD clock. */
    result_t res = sdhci_regs_disable_sd_clock(sdhci_regs);
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to disable SD clock in sdhci_set_sd_clock().");
    }
    usleep(10);
    /* Get the SD clock divisor we want to set. */
    uint32_t sd_clock_divisor = 0;
    res = sdhci_get_sd_clock_divisor(
            sdhci_regs,
            freq,
            &sd_clock_divisor
    );
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to get SD clock divisor in sdhci_set_sd_clock().");
    }
    /* Set SD Clock Mode to "Divided". */
    res = sdhci_regs_set_sd_clock_mode_as_divided(sdhci_regs);
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to set SD clock mode to \"Divided\" in sdhci_set_sd_clock().");
    }
    /* Set SD Clock Divisor. */
    res = sdhci_regs_set_sd_clock_divisor(
            sdhci_regs,
            sd_clock_divisor
    );
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to set SD clock divisor in sdhci_set_sd_clock().");
    }
    usleep(10);
    /* Enable the SD Clock. */
    res = sdhci_regs_enable_sd_clock(sdhci_regs);
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to enable SD clock in sdhci_set_sd_clock().");
    }
    // sel4cp_dbg_puts("Setting SD clock to %uHz (divisor: %u).", freq, sd_clock_divisor);
    /* Wait for the SD Clock to stabilise. The SDHCI specifies timeout as 150
     * milliseconds (150,000 microseconds) */
    size_t retries_stable = 15000;
    bool is_sd_clock_stable = false;
    do {
        usleep(10);
        res = sdhci_regs_is_sd_clock_stable(
                sdhci_regs,
                &is_sd_clock_stable
        );
        if (result_is_err(res)) {
            return result_err_chain(res, "Failed to check if SD clock is stable in sdhci_set_sd_clock().");
        }
    } while (!is_sd_clock_stable && (retries_stable-- > 0));
    if (!is_sd_clock_stable) {
        return result_err("Timed out waiting for SD clock to be stable in sdhci_set_sd_clock().");
    }
    sel4cp_dbg_puts("SD clock is stable.");
    return result_ok();
}

//! This seems like a Raspberry Pi specific function.
result_t sdhci_wait_for_interrupt(
        sdhci_regs_t *sdhci_regs,
        uint32_t interrupt_mask,
        sdhci_result_t *sdhci_result
) {
    if (sdhci_regs == NULL) {
        return result_err("NULL `sdhci_regs` passed to sdhci_wait_for_interrupt().");
    }
    if (sdhci_result == NULL) {
        return result_err("NULL `sdhci_result` passed to sdhci_wait_for_interrupt().");
    }
    *sdhci_result = SD_ERROR;
    // uint32_t mask_with_error = interrupt_mask | INT_ERROR_MASK;
    // /* Wait for the interrupt. We specify a timeout of 1 second. */
    // size_t retries = 100000;
    // bool is_finished_or_error = false;
    // do {
    //     usleep(10);
    //     result_t res = sdhci_regs_mask_interrupt(
    //             sdhci_regs,
    //             mask_with_error,
    //             &is_finished_or_error
    //     );
    //     if (result_is_err(res)) {
    //         return result_err_chain(res, "Failed to wait for interrupt in sdhci_wait_for_interrupt().");
    //     }
    // } while (!is_finished_or_error && (retries-- > 0));
    // /* Read interrupt. */
    // uint32_t interrupt_raw32 = 0;
    // result_t res_get_raw32 = sdhci_regs_get_interrupt_raw32(
    //         sdhci_regs,
    //         &interrupt_raw32
    // );
    // if (result_is_err(res_get_raw32)) {
    //     return result_err_chain(res_get_raw32, "Failed to get `interrupt` in sdhci_wait_for_interrupt().");
    // }
    // /* Timeout case. */
    // if (!is_finished_or_error) {
    //     *sdhci_result = SD_TIMEOUT;
    //     bool is_cmd_timeout = false;
    //     result_t res = sdhci_regs_is_cmd_timeout_err(
    //             sdhci_regs,
    //             &is_cmd_timeout
    //     );
    //     if (result_is_err(res)) {
    //         return result_err_chain(res, "Failed to get `interrupt.CTO_ERR` in sdhci_wait_for_interrupt().");
    //     }
    //     bool is_data_timeout = false;
    //     res = sdhci_regs_is_data_timeout_err(
    //             sdhci_regs,
    //             &is_data_timeout
    //     );
    //     if (result_is_err(res)) {
    //         return result_err_chain(res, "Failed to get `interrupt.DTO_ERR` in sdhci_wait_for_interrupt().");
    //     }
    //     /* Clear the interrupt register. */
    //     res = sdhci_regs_set_interrupt_raw32(
    //             sdhci_regs,
    //             interrupt_raw32
    //     );
    //     if (result_is_err(res)) {
    //         return result_err_chain(res, "Failed to clear `interrupt` in sdhci_wait_for_interrupt().");
    //     }
    //     /* Return error depending on the type of timeout. */
    //     if (is_cmd_timeout) {
    //         return result_err("Command timeout error in sdhci_wait_for_interrupt().");
    //     }
    //     if (is_data_timeout) {
    //         return result_err("Data timeout error in sdhci_wait_for_interrupt().");
    //     }
    //     return result_err("Timed out waiting for interrupt in sdhci_wait_for_interrupt().");
    // }
    // /* Error case. */
    // bool is_error = false;
    // result_t res = sdhci_regs_mask_interrupt(sdhci_regs, INT_ERROR_MASK, &is_error);
    // if (result_is_err(res)) {
    //     return result_err_chain(res, "Failed to check for error in sdhci_wait_for_interrupt().");
    // }
    // if (is_error) {
    //     /* Clear the interrupt register. */
    //     res = sdhci_regs_set_interrupt_raw32(sdhci_regs, interrupt_raw32);
    //     if (result_is_err(res)) {
    //         return result_err_chain(res, "Failed to clear `interrupt` in sdhci_wait_for_interrupt().");
    //     }
    //     return result_err("Error interrupt in sdhci_wait_for_interrupt().");
    // }
    // /* Clear the interrupt register. */
    // res = sdhci_regs_set_interrupt_raw32(sdhci_regs, interrupt_mask);
    // if (result_is_err(res)) {
    //     return result_err_chain(res, "Failed to clear `interrupt` in sdhci_wait_for_interrupt().");
    // }
    *sdhci_result = SD_OK;
    /* Success case. */
    return result_ok();
}

result_t sdhci_wait_for_cmd_in_progress(
        sdhci_regs_t *sdhci_regs,
        sdhci_result_t *sdhci_result
) {
    if (sdhci_regs == NULL) {
        return result_err("NULL `sdhci_regs` passed to sdhci_wait_for_cmd_in_progress().");
    }
    if (sdhci_result == NULL) {
        return result_err("NULL `sdhci_result` passed to sdhci_wait_for_cmd_in_progress().");
    }
    *sdhci_result = SD_ERROR;
    bool cmd_in_progress = false;
    bool has_any_err = false;
    size_t retries = 100000;
    do {
        usleep(10);
        result_t res_cmd = sdhci_regs_is_cmd_in_progress(
                sdhci_regs,
                &cmd_in_progress
        );
        if (result_is_err(res_cmd)) {
            return result_err_chain(res_cmd, "Failed to check if command is in progress in sdhci_wait_for_cmd_in_progress().");
        }
        result_t res_err = sdhci_regs_is_any_err(
                sdhci_regs,
                &has_any_err
        );
        if (result_is_err(res_err)) {
            return result_err_chain(res_err, "Failed to check if any error occurred in sdhci_wait_for_cmd_in_progress().");
        }
    } while (cmd_in_progress && !has_any_err && (retries-- > 0));
    if (has_any_err) {
        return result_err("Error occurred in sdhci_wait_for_cmd_in_progress().");
    }
    if (cmd_in_progress) {
        *sdhci_result = SD_BUSY;
        return result_err("Timed out waiting for command in sdhci_wait_for_cmd_in_progress().");
    }
    *sdhci_result = SD_OK;
    return result_ok();
}

result_t sdhci_wait_for_data_in_progress(
        sdhci_regs_t *sdhci_regs,
        sdhci_result_t *sdhci_result
) {
    if (sdhci_regs == NULL) {
        return result_err("NULL `sdhci_regs` passed to sdhci_wait_for_data_in_progress().");
    }
    if (sdhci_result == NULL) {
        return result_err("NULL `sdhci_result` passed to sdhci_wait_for_data_in_progress().");
    }
    *sdhci_result = SD_ERROR;
    bool data_in_progress = false;
    bool has_any_err = false;
    size_t retries = 100000;
    do {
        usleep(10);
        result_t res_cmd = result_ok();
        data_in_progress = true;
        // result_t res_cmd = sdhci_regs_is_data_in_progress(
        //         sdhci_regs,
        //         &data_in_progress
        // );
        if (result_is_err(res_cmd)) {
            return result_err_chain(res_cmd, "Failed to check if data is in progress in sdhci_wait_for_data_in_progress().");
        }
        result_t res_err = sdhci_regs_is_any_err(
                sdhci_regs,
                &has_any_err
        );
        if (result_is_err(res_err)) {
            return result_err_chain(res_err, "Failed to check if any error occurred in sdhci_wait_for_data_in_progress().");
        }
    } while (data_in_progress && !has_any_err && (retries-- > 0));
    if (has_any_err) {
        return result_err("Error occurred in sdhci_wait_for_data_in_progress().");
    }
    if (data_in_progress) {
        *sdhci_result = SD_BUSY;
        return result_err("Timed out waiting for data in sdhci_wait_for_data_in_progress().");
    }
    *sdhci_result = SD_OK;
    return result_ok();
}

//! Send CMD

result_t sdhci_send_cmd(
        sdhci_regs_t *sdhci_regs,
        size_t sdhci_cmd_index,
        uint32_t arg,
        sdcard_t *sdcard,
        sdhci_result_t *sdhci_result
) {
    if (sdhci_regs == NULL) {
        return result_err("NULL `sdhci_regs` passed to sdhci_send_cmd().");
    }
    if (sdcard == NULL) {
        return result_err("NULL `sdcard` passed to sdhci_send_cmd().");
    }
    if (sdhci_result == NULL) {
        return result_err("NULL `sdhci_result` passed to sdhci_send_cmd().");
    }
    *sdhci_result = SD_ERROR;

    sel4cp_dbg_puts("\n\nSDHCI send commands checks finished\n\n");
    result_t res;
    /* Obtain the command from the list of commands we can send. */
    sdhci_cmd_t *sdhci_cmd = NULL;
    res = sdhci_cmds_get_cmd(
            sdhci_cmd_index,
            &sdhci_cmd
    );
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to get command in sdhci_send_cmd().");
    }
    /* Determine if this command requires an RCA or not. */
    bool requires_rca = false;
    res = sdhci_cmd_uses_rca(
            sdhci_cmd,
            &requires_rca
    );
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to check if command uses RCA in sdhci_send_cmd().");
    }
    /* If the command requires an RCA, check the argument passed into this
     * function is the RCA or throw an error. */
    if (requires_rca) {
        uint32_t rca = 0;
        res = sdcard_get_rca(sdcard, &rca);
        if (result_is_err(res)) {
            return result_err_chain(res, "Failed to get RCA in sdhci_send_cmd().");
        }
        if (rca != arg) {
            return result_err("Illegal Argument Exception: Command requires RCA, but RCA passed to sdhci_send_cmd() is not the RCA of the sdcard.");
        }
    }
    sel4cp_dbg_puts("\n\nRCA found\n\n");


    /* Check whether command is an APP Command. */
    bool is_app_cmd = false;
    result_t res_is_app_cmd = sdhci_cmds_is_app_cmd(
            sdhci_cmd_index,
            &is_app_cmd
    );
    if (result_is_err(res_is_app_cmd)) {
        return result_err_chain(res_is_app_cmd, "Failed to check if command is an app command in sdhci_send_cmd().");
    }

    if (is_app_cmd) {
        /* Check if Relative Card Address (RCA) has been saved yet. */
        bool has_rca = false;
        res = sdcard_has_rca(sdcard, &has_rca);
        if (result_is_err(res)) {
            return result_err_chain(res, "Failed to check if sdcard has RCA in sdhci_send_cmd().");
        }
        size_t app_cmd_index = IDX_APP_CMD;
        uint32_t app_cmd_arg = 0;
        /* If yes, use the RCA as the `app_cmd_arg`. */
        if (has_rca) {
            app_cmd_index = IDX_APP_CMD_RCA;
            res = sdcard_get_rca(sdcard, &app_cmd_arg);
            if (result_is_err(res)) {
                return result_err_chain(res, "Failed to get RCA in sdhci_send_cmd().");
            }
        }
        /* `app_cmd_arg` will be 0 if no RCA and will be RCA otherwise. This is
         * a recursive call to ourselves. */
        res = sdhci_send_cmd(
                sdhci_regs,
                app_cmd_index, /* This will change depending on if RCA exists or not. */
                app_cmd_arg,
                sdcard,
                sdhci_result
        );
        if (result_is_err(res)) {
            return result_err_chain(res, "Failed to send app command in sdhci_send_cmd().");
        }
        /* When there is an RCA, we should check the status indicates APP_CMD accepted. */
        if (app_cmd_index == IDX_APP_CMD_RCA) {
            bool is_app_cmd_accepted;
            res = sdcard_is_app_cmd_accepted(sdcard, &is_app_cmd_accepted);
            if (result_is_err(res)) {
                return result_err_chain(res, "Failed to check if app command was accepted in sdhci_send_cmd().");
            }
            if (!is_app_cmd_accepted) {
                *sdhci_result = SD_ERROR;
                return result_err("App command was not accepted in sdhci_send_cmd().");
            }
        }
    }

    sel4cp_dbg_puts("\n\nApp commands check done\n\n");

    /* Wait for command in progress. */
    res = sdhci_wait_for_cmd_in_progress(sdhci_regs, sdhci_result);
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to wait for command in progress in sdhci_send_cmd().");
    }

    /* Clear interrupt flags. */
    res = sdhci_regs_clear_interrupt(sdhci_regs);
    if (sdhci_regs->regs == NULL) {
        sel4cp_dbg_puts("\n\nNo inner OC4 regs....\n\n");
    }
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to clear interrupt flags in sdhci_send_cmd().");
    }

    sel4cp_dbg_puts("\n\nReady to run sdhci init\n\n");


    /* Set the argument register first. */
    res = sdhci_regs_set_arg1(
            sdhci_regs,
            arg
    );
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to set argument in sdhci_send_cmd().");
    }

    char tnum[10];
    sel4cp_dbg_puts("getting regs1: ");

    sel4cp_dbg_puts(itoa(sdhci_regs->regs->sd_emmc_cmd_arg, tnum, 10));

    //! Raspberry Pi specific!
    /* Get the command register value stored in `sdhci_cmd`. */
    // cmdtm_t cmdtm;
    // res = sdhci_cmd_get_cmdtm(
    //         sdhci_cmd,
    //         &cmdtm
    // );
    // if (result_is_err(res)) {
    //     return result_err_chain(res, "Failed to get cmdtm in sdhci_send_cmd().");
    // }

    // /* Set the command register to the value obtained from `sdhci_cmd`. */
    // res = sdhci_regs_set_cmdtm(
    //         sdhci_regs,
    //         cmdtm
    // );
    // if (result_is_err(res)) {
    //     return result_err_chain(res, "Failed to set cmdtm in sdhci_send_cmd().");
    // }
    /* Obtain the delay from the command. */
    size_t delay_us = 0;
    res = sdhci_cmd_get_delay(
            sdhci_cmd,
            &delay_us
    );
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to get delay in sdhci_send_cmd().");
    }
    /* Wait for the delay. */
    if (delay_us) {
        usleep(delay_us);
    }

    sel4cp_dbg_puts("\n\nGotten delay\n\n");

    //! This code won't work as we don't have the interrupt register in the OC4

    /* Wait until command complete interrupt */
    res = sdhci_wait_for_interrupt(
            sdhci_regs,
            INT_CMD_DONE,
            sdhci_result
    );
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to wait for command complete interrupt in sdhci_send_cmd().");
    }

    sel4cp_dbg_puts("\n\nGotten interrupt\n\n");


    /* Get the response from `resp0`. */
    uint32_t resp0;
    res = sdhci_regs_get_resp0(
            sdhci_regs,
            &resp0
    );
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to get resp0 in sdhci_send_cmd().");
    }

    /* Get the SDHCI command's response type. */
    cmd_rspns_type_t cmd_rspns_type;
    res = sdhci_cmd_get_cmd_rspns_type(
            sdhci_cmd,
            &cmd_rspns_type
    );
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to get cmd_rspns_type in sdhci_send_cmd().");
    }

    sel4cp_dbg_puts("\n\nGotten cmd response type\n\n");

    /* Handle response depending on the SDHCI command's response type. */
    switch (cmd_rspns_type) {
        case CMD_NO_RESP: {
            sel4cp_dbg_puts("\n\nGotten cmd response type, it's CMD_NO_RESP\n\n");

            *sdhci_result = SD_OK;
            return result_ok();
        }
        case CMD_BUSY48BIT_RESP: {
            /* `IDX_CARD_SELECT` will enter here. */
            sel4cp_dbg_puts("\n\nGotten cmd response type, it's CMD_BUSY48BIT_RESP\n\n");

            res = sdcard_set_status(sdcard, resp0);
            if (result_is_err(res)) {
                return result_err_chain(res, "Failed to set status in sdhci_send_cmd().");
            }
            bool resp_r1_has_err = true;
            res = sdhci_resp_r1_has_error(resp0, &resp_r1_has_err);
            if (result_is_err(res)) {
                return result_err_chain(res, "Failed to check if resp_r1 has error in sdhci_send_cmd().");
            }
            if (resp_r1_has_err) {
                return result_err("Response from SD card indicates error in `CMD_BUSY48BIT_RESP` case in sdhci_send_cmd().");
            }
            *sdhci_result = SD_OK;
            return result_ok();
        }
        case CMD_48BIT_RESP: {
            sel4cp_dbg_puts("\n\nGotten cmd response type, it's CMD_48BIT_RESP\n\n");

            /* Obtain the command index. */
            cmd_index_t cmd_index;
            result_t res_get_cmd_index = sdhci_cmd_get_cmd_index(
                    sdhci_cmd,
                    &cmd_index
            );
            if (result_is_err(res_get_cmd_index)) {
                return result_err_chain(res_get_cmd_index, "Failed to get cmd_index in sdhci_send_cmd().");
            }
            switch (cmd_index) {
                case 0x03: {
                    /* This is the switch-case for `IDX_SEND_REL_ADDR`.
                     * RESP0 contains the RCA and status bits 23,22,19,12:0. */
                    /* Get the RCA from `resp0`. */
                    sel4cp_dbg_puts("getting resp0: xxxxxxxx");

                    char xnum[10];
                    sel4cp_dbg_puts(itoa(resp0, xnum, 10));
                    sel4cp_dbg_puts("\n\nGotten cmd response type, it's CMD_48BIT_RESP, 0x03\n\n");

                    uint32_t rca = 0;
                    res = sdhci_resp_r6_get_rca(resp0, &rca);
                    if (result_is_err(res)) {
                        return result_err_chain(res, "Failed to get RCA in sdhci_send_cmd().");
                    }
                    /* Get the status from `resp0`. */
                    uint32_t status = 0;
                    res = sdhci_resp_r6_get_status(resp0, &status);
                    if (result_is_err(res)) {
                        return result_err_chain(res, "Failed to get status in sdhci_send_cmd().");
                    }
                    /* Save the RCA. */
                    res = sdcard_set_rca(sdcard, rca);
                    if (result_is_err(res)) {
                        return result_err_chain(res, "Failed to set RCA in sdhci_send_cmd().");
                    }
                    /* Save the status. */
                    res = sdcard_set_status(sdcard, status);
                    if (result_is_err(res)) {
                        return result_err_chain(res, "Failed to set status in sdhci_send_cmd().");
                    }
                    bool resp_r1_has_err = true;
                    res = sdhci_resp_r1_has_error(status, &resp_r1_has_err);
                    if (result_is_err(res)) {
                        return result_err_chain(res, "Failed to check if resp_r1 has error in sdhci_send_cmd().");
                    }
                    if (resp_r1_has_err) {
                        return result_err("Response from SD card indicates error in sdhci_send_cmd().");
                    }
                    *sdhci_result = SD_OK;
                    return result_ok();
                }
                case 0x08: {
                    /* This is the switch-case for `IDX_SEND_IF_COND`. RESP0 contains
                     * voltage acceptance and check pattern, which should match
                     * the argument. */
                    sel4cp_dbg_puts("\n\nGotten cmd response type, it's CMD_48BIT_RESP, 0x08\n\n");

                    // TODO: fix IFCOND

                    char snum[10];
                    sel4cp_dbg_puts("getting arg: ");

                    sel4cp_dbg_puts(itoa(arg, snum, 10));

                    sel4cp_dbg_puts("getting resp0: ");

                    sel4cp_dbg_puts(itoa(resp0, snum, 10));

                    sel4cp_dbg_puts("getting resp1: ");

                    sel4cp_dbg_puts(itoa(sdhci_regs->regs->sd_emmc_cmd_rsp1, snum, 10));

                    sel4cp_dbg_puts("getting resp2: ");

                    sel4cp_dbg_puts(itoa(sdhci_regs->regs->sd_emmc_cmd_rsp2, snum, 10));

                    sel4cp_dbg_puts("getting resp3: ");

                    sel4cp_dbg_puts(itoa(sdhci_regs->regs->sd_emmc_cmd_rsp3, snum, 10));

                    // TODO: fix
                    return result_ok();


                    if (resp0 == arg) {
                        *sdhci_result = SD_OK;
                        return result_ok();
                    } else {
                        *sdhci_result = SD_ERROR;
                        sel4cp_dbg_puts("\n\nGotten cmd response type, not match\n\n");

                        return result_err("Response from SD card does not match argument in sdhci_send_cmd().");
                    }
                }
                case 0x29: {
                    /* Response handling for `IDX_APP_SEND_OP_COND`. Save the
                     * RESP0 register as the Operation Conditions Register (OCR)
                     * for the `sdcard`. */
                    sel4cp_dbg_puts("\n\nGotten cmd response type, it's CMD_48BIT_RESP, 0x29\n\n");

                    sel4cp_dbg_puts("getting resp0: ");

                    char snum[10];
                    sel4cp_dbg_puts(itoa(resp0, snum, 10));
                    sel4cp_dbg_puts("\n");



                    res = sdcard_set_ocr_raw32(sdcard, resp0);
                    while (result_is_err(res)) {
                        // res = sdcard_set_ocr_raw32(sdcard, resp0);
                        // sel4cp_dbg_puts("trying 0x29 and ");
                        return result_err_chain(res, "Failed to set OCR in sdhci_send_cmd().");
                    }
                    *sdhci_result = SD_OK;
                    return result_ok();
                }
                default: {
                    /* Save the response as the SD card's status. */
                    sel4cp_dbg_puts("\n\nGotten cmd response type, it's CMD_48BIT_RESP, default\n\n");

                    res = sdcard_set_status(sdcard, resp0);
                    if (result_is_err(res)) {
                        return result_err_chain(res, "Failed to set status in sdhci_send_cmd().");
                    }
                    bool resp_r1_has_err = true;
                    res = sdhci_resp_r1_has_error(resp0, &resp_r1_has_err);
                    if (result_is_err(res)) {
                        return result_err_chain(res, "Failed to check if resp_r1 has error in sdhci_send_cmd().");
                    }
                    if (resp_r1_has_err) {
                        return result_err("Response from SD card indicates error in sdhci_send_cmd().");
                    }
                    *sdhci_result = SD_OK;
                    return result_ok();
                }
            }
        }
        case CMD_136BIT_RESP: {
            sel4cp_dbg_puts("\n\nGotten cmd response type, it's CMD_136BIT_RESP\n\n");

            /* Obtain the command index. */
            
            cmd_index_t cmd_index;
            result_t res_get_cmd_index = sdhci_cmd_get_cmd_index(
                    sdhci_cmd,
                    &cmd_index
            );
            if (result_is_err(res_get_cmd_index)) {
                return result_err_chain(res_get_cmd_index, "Failed to get cmd_index in sdhci_send_cmd().");
            }
            if (cmd_index == 0x09) {
                /* `IDX_SEND_CSD` will enter this branch. */
                /* Get the response from `resp1`. */
                uint32_t resp1 = 0;
                res = sdhci_regs_get_resp1(
                        sdhci_regs,
                        &resp1
                );
                if (result_is_err(res)) {
                    return result_err_chain(res, "Failed to get resp1 in sdhci_send_cmd().");
                }
                /* Get the response from `resp2`. */
                uint32_t resp2 = 0;
                res = sdhci_regs_get_resp2(
                        sdhci_regs,
                        &resp2
                );
                if (result_is_err(res)) {
                    return result_err_chain(res, "Failed to get resp2 in sdhci_send_cmd().");
                }
                /* Get the response from `resp3`. */
                uint32_t resp3 = 0;
                res = sdhci_regs_get_resp3(
                        sdhci_regs,
                        &resp3
                );
                if (result_is_err(res)) {
                    return result_err_chain(res, "Failed to get resp3 in sdhci_send_cmd().");
                }
                /* Save the response as the SD card's CSD. */
                res = sdcard_set_sdcard_data(sdcard, resp0, resp1, resp2, resp3);
                sel4cp_dbg_puts("getting resp0 and setting as csd: ");

                char fnum[10];
                sel4cp_dbg_puts(itoa(resp0, fnum, 10));
                sel4cp_dbg_puts("\n");
                if (result_is_err(res)) {
                    return result_err_chain(res, "Failed to set CSD in sdhci_send_cmd().");
                }
                *sdhci_result = SD_OK;
                return result_ok();
            } else {
                /* `IDX_ALL_SEND_CID` will enter this branch. */
                /* Get the response from `resp1`. */
                uint32_t resp1 = 0;
                res = sdhci_regs_get_resp1(
                        sdhci_regs,
                        &resp1
                );
                if (result_is_err(res)) {
                    return result_err_chain(res, "Failed to get resp1 in sdhci_send_cmd().");
                }
                /* Get the response from `resp2`. */
                uint32_t resp2 = 0;
                res = sdhci_regs_get_resp2(
                        sdhci_regs,
                        &resp2
                );
                if (result_is_err(res)) {
                    return result_err_chain(res, "Failed to get resp2 in sdhci_send_cmd().");
                }
                /* Get the response from `resp3`. */
                uint32_t resp3 = 0;
                res = sdhci_regs_get_resp3(
                        sdhci_regs,
                        &resp3
                );
                if (result_is_err(res)) {
                    return result_err_chain(res, "Failed to get resp3 in sdhci_send_cmd().");
                }
                /* Save the response as the SD card's CID. */
                res = sdcard_set_cid(sdcard, resp0, resp1, resp2, resp3);
                if (result_is_err(res)) {
                    return result_err_chain(res, "Failed to set CID in sdhci_send_cmd().");
                }


                sel4cp_dbg_puts("getting resp0 and setting as cid: ");

                char dnum[10];
                sel4cp_dbg_puts(itoa(resp0, dnum, 10));
                sel4cp_dbg_puts("\n");
                *sdhci_result = SD_OK;
                return result_ok();
            }
        }
    }

    return result_err("Response handling for command has not been implemented in sdhci_send_cmd().");
}




