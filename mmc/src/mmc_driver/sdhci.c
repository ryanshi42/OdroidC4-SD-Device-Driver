#include "sdhci.h"

//TODO Integrate this nicer later
// struct mmc_cmd {
// 	ushort cmdidx;
// 	uint resp_type;
// 	uint cmdarg;
// 	uint response[4];
// };

static char
hexchar(unsigned int v)
{
    return v < 10 ? '0' + v : ('a' - 10) + v;
}

static void
puthex32(uint32_t val)
{
    char buffer[8 + 3];
    buffer[0] = '0';
    buffer[1] = 'x';
    buffer[8 + 3 - 1] = 0;
    for (unsigned i = 8 + 1; i > 1; i--) {
        buffer[i] = hexchar(val & 0xf);
        val >>= 4;
    }
    sel4cp_dbg_puts(buffer);
}

result_t sdhci_card_init_and_id(
        sdhci_regs_t *sdhci_regs,
        sdcard_t *sdcard,
        sdhci_result_t *sdhci_result
) {

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
            NULL,
            sdhci_regs,
            IDX_GO_IDLE_STATE,
            0,
            sdcard,
            &sdhci_res_go_idle
    );
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to send `IDX_GO_IDLE_STATE` in sdhci_card_init_and_id().");
    }
    sel4cp_dbg_puts("\nFinished setting GO_IDLE.\n");


    // // Sending Test command: send_CSD
    // uint32_t temp_rca = 0;
    // res = sdcard_get_rca(sdcard, &temp_rca);

    // sel4cp_dbg_puts("The current RCA is;");

    // char jnum[16];
    // sel4cp_dbg_puts(itoa(temp_rca, jnum, 16));
    // sel4cp_dbg_puts("\n");
    
    // if (result_is_err(res)) {
    //     return result_err_chain(res, "Failed to get card's RCA in sdhci_card_init_and_id().");
    // }
    // sel4cp_dbg_puts("Sending SEND_CSD test command...");
    // sdhci_result_t sdhci_res_send_csd;
    // res = sdhci_send_cmd(
    //         sdhci_regs,
    //         IDX_SEND_CSD,
    //         temp_rca,
    //         sdcard,
    //         &sdhci_res_send_csd
    // );
    // if (result_is_err(res)) {
    //     return result_err_chain(res, "Failed to send `IDX_SEND_IF_COND` in sdhci_card_init_and_id().");
    // }
    // sel4cp_dbg_puts("\nFinished setting SEND_CSD");


    /* Sending SEND_IF_COND,0x000001AA (CMD8) voltage range 0x1 check pattern 0xAA.
     * TODO: If voltage range and check pattern don't match, look for older card. */
    sel4cp_dbg_puts("\nSending IF_COND (CMD8) command...\n");
    sdhci_result_t sdhci_res_if_cond;
    res = sdhci_send_cmd(
            NULL,
            sdhci_regs,
            IDX_SEND_IF_COND,
            0x000001AA,
            sdcard,
            &sdhci_res_if_cond
    );
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to send `IDX_SEND_IF_COND` in sdhci_card_init_and_id().");
    }
    sel4cp_dbg_puts("\nFinished sending IF_COND. \n");


    /* If card responded with voltage and check pattern, resolve voltage and
     * check for high capacity card with IDX_APP_SEND_OP_COND. */

    sel4cp_dbg_puts("\nTrying to power up...\n");


    size_t retries = 7;
    bool has_powered_up = false;
    do {
        usleep(400000);
        res = sdhci_send_cmd(
                NULL,
                sdhci_regs,
                IDX_APP_SEND_OP_COND,
                ACMD41_ARG_HC,
                sdcard,
                sdhci_result
        );


        if (result_is_err(res)) {
            sel4cp_dbg_puts("\nResult error when sending command to power up. \n");

            // return result_err_chain(res, "Failed to send `IDX_APP_SEND_OP_COND` in sdhci_card_init_and_id().");
        }
        res = sdcard_has_powered_up(sdcard, &has_powered_up);
        if (result_is_err(res)) {
            puthex32(has_powered_up);
            sel4cp_dbg_puts("\nResult error when powering up. \n");

            return result_err_chain(res, "Failed to check if card has powered up in sdhci_card_init_and_id().");
        }
    } while (!has_powered_up && (retries-- > 0));

    // sel4cp_dbg_puts("\nFinished setting IF_COND");

    /* If the card still hasn't powered up, we timeout. */
    if (!has_powered_up) {
        sel4cp_dbg_puts("\nTimeout, could not power up \n");

        *sdhci_result = SD_TIMEOUT;
        return result_err("EMMC card did not power up in sdhci_card_init_and_id().");
    }

    sel4cp_dbg_puts("\nSD Card powered up. \n");


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
    sel4cp_dbg_puts("\nVoltage good. \n");


    /* Check card capacity. */
    sel4cp_dbg_puts("Checking card type...");
    bool is_high_capacity = false;
    res = sdcard_is_high_capacity(sdcard, &is_high_capacity);
    if (result_is_err(res)) {
        return result_err("Failed to check if card is high capacity in sdhci_card_init_and_id().");
    }

    //? The card we are using seems to be high capacity.
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
            NULL,
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
            NULL,
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

    /* ===================================
     * Setting SD Clock Frequency to full-speed.
     * =================================== */
    //! RPi3 specific code. I am not actually sure that this is needed or what happens if it doesn't work.
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
    sel4cp_dbg_puts("\nObtaining SD card's Relative Card Address (RCA)...\n");
    uint32_t rca = 0;
    res = sdcard_get_rca(sdcard, &rca);
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to get card's RCA in sdhci_card_init_and_id().");
    }

    sel4cp_dbg_puts("\nObtained the RCA, it has value: ");
    puthex32(rca);
    sel4cp_dbg_puts("\n");

    /* Send (CMD13). */
    /* TODO: In theory, loop back to SEND_IF_COND to find additional cards. */
    sel4cp_dbg_puts("Sending SEND_STATUS (CMD13) command...\n");
    res = sdhci_send_cmd(
            NULL,
            sdhci_regs,
            IDX_SEND_STATUS,
            rca,
            sdcard,
            sdhci_result
    );
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to send `SEND_STATUS` in sdhci_card_init_and_id().");
    }

    sel4cp_dbg_puts("\nStatus should be equal to 0x00000700 (stby status).\n");

    // TODO: Embed programmatic checking for send_status in the future.
    // uint32_t curr_status = res & GENMASK_UNSAFE(12, 9);
    // if (curr_status != MMC_STATUS_STBY) {
    //     puthex32(curr_status);
    //     puthex32(MMC_STATUS_STBY);
    //     puthex32(res);
    //     sel4cp_dbg_puts("Card is not in the correct mode (stby mode)...\n");
    //     return result_err_chain(res, "`SEND_STATUS` in sdhci_card_init_and_id() is not in stby mode, the expected mode.");
    // } 

    sel4cp_dbg_puts("Sending SEND_CSD (CMD9)...");
    res = sdhci_send_cmd(
            NULL,
            sdhci_regs,
            IDX_SEND_CSD,
            rca,
            sdcard,
            sdhci_result
    );
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to send `IDX_CARD_SELECT` in sdhci_card_init_and_id().");
    }
    sel4cp_dbg_puts("\nSent the CSD successfully.\n");

    /* Send (CMD13). */
    /* TODO: In theory, loop back to SEND_IF_COND to find additional cards. */
    sel4cp_dbg_puts("Sending SEND_STATUS (CMD13) command...\n");
    res = sdhci_send_cmd(
            NULL,
            sdhci_regs,
            IDX_SEND_STATUS,
            rca,
            sdcard,
            sdhci_result
    );
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to send `SEND_STATUS` in sdhci_card_init_and_id().");
    }

    sel4cp_dbg_puts("\nStatus should be equal to 0x00000700 (stby status).\n");

    //! CAUTION: Do NOT reverse the order of commands. This order of commands is strictly necessary.

    /* ===================================
     * Populating `sdcard` with data from SD Card Configuration Register (SCR):
     * =================================== */

    /* Call the `IDX_CARD_SELECT` CMD using the RCA. */
    /* TODO: Check card_is_locked status in the R1 response from CMD7 [bit 25],
     * if so, use CMD42 to unlock CMD42 structure [4.3.7] same as a single block
     * write; data block includes PWD setting mode, PWD len, PWD data.*/

    sel4cp_dbg_puts("Sending CARD_SELECT (CMD7)...");
    res = sdhci_send_cmd(
            NULL,
            sdhci_regs,
            IDX_CARD_SELECT,
            rca,
            sdcard,
            sdhci_result
    );
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to send `IDX_CARD_SELECT` in sdhci_card_init_and_id().");
    }

    /* Send (CMD13). */
    /* TODO: In theory, loop back to SEND_IF_COND to find additional cards. */
    sel4cp_dbg_puts("Sending SEND_STATUS (CMD13) command...\n");
    res = sdhci_send_cmd(
            NULL,
            sdhci_regs,
            IDX_SEND_STATUS,
            rca,
            sdcard,
            sdhci_result
    );
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to send `SEND_STATUS` in sdhci_card_init_and_id().");
    }
    sel4cp_dbg_puts("\nStatus should be equal to 0x00000900 (tran status + ready for data).\n");

    // Now we need to start reading the SCR

    //? DO NOT REMOVE: this does not work as of now but it might later on.

    sdhci_data_t sdhci_data = {0};

    sdhci_data_t* sdhci_data_ptr = &sdhci_data;

    sdhci_data_ptr->blocks = 1;

    //? We take 3 as the block size is 2 ^ 3
    sdhci_data_ptr->blocksize = 8;

    sdhci_data_ptr->flags = MMC_DATA_READ;

    char buffer[512] = {0};
    sdhci_data_ptr->dest = buffer;

    // puthex32(sdhci_data_ptr->blocksize);

    /* Reading from the SD Card Configuration Register (SCR).
     * SEND_SCR command is like a READ_SINGLE but for a block of 8 bytes.*/
    sel4cp_dbg_puts("Reading from the SD Card Configuration Register (SCR)...");

    //! This probably won't work on the Odroid C4.
    /* Wait for any data operation that might be in progress before reading the block. */
    res = sdhci_wait_for_data_in_progress(sdhci_regs, sdhci_result);
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to wait for data in progress in sdhci_card_init_and_id().");
    }

    // /* Set BLKSIZECNT to 1 block of 8 bytes. */
    // sel4cp_dbg_puts("\nSetting BLKSIZECNT to 1 block of 8 bytes...\n");

    sel4cp_dbg_puts("\nFinished, now we need to send SCR. \n");
    
    /* Get the SCR by sending the `SEND_SCR` command. We need the SCR to figure
     * out what the allowed bus widths are. */
    sel4cp_dbg_puts("Sending SEND_SCR (CMD51) command...");
    res = sdhci_send_cmd(
            sdhci_data_ptr,
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

    sel4cp_dbg_puts("SEND_SCR Buffer is:");
    for (int i = 0; i < 512; i++) {
        sel4cp_dbg_putc(buffer[i]);
    }


    //? The following may not work, we just want to test it out.
    /* Save low portion of SCR register to sdcard. */
    res = sdcard_set_scr_raw32_lo(sdcard, buffer[0]);
    sel4cp_dbg_puts("\n\nSCR LO is: \n");
    // puthex32(data);
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to set SCR raw32 lo in sdhci_card_init_and_id().");
    }

    /* Save high portion of SCR register to sdcard. */
    res = sdcard_set_scr_raw32_hi(sdcard, buffer[32]);
    sel4cp_dbg_puts("\n\nSCR HI is: \n");
    // puthex32(data);

    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to set SCR raw32 hi in sdhci_card_init_and_id().");
    }

    //     sel4cp_dbg_puts("Sending CARD_SELECT (CMD7)...");
    // res = sdhci_send_cmd(
    //         NULL,
    //         sdhci_regs,
    //         IDX_CARD_SELECT,
    //         0,
    //         sdcard,
    //         sdhci_result
    // );
    // if (result_is_err(res)) {
    //     return result_err_chain(res, "Failed to send `IDX_CARD_SELECT` in sdhci_card_init_and_id().");
    // }




    // /* Start reading SCR from EMMC. */
    // int num_scr_reads = 0;
    // int num_scr_retries = 160000;
    // bool is_read_ready = false;
    // do {
    //     // res = sdhci_regs_is_read_ready(sdhci_regs, &is_read_ready);
    //     // if (result_is_err(res)) {
    //     //     return result_err_chain(res, "Failed to check if read ready in sdhci_card_init_and_id().");
    //     // }
    //     is_read_ready = true;

    //     //! this data does not work properly.
    //     if (is_read_ready) {
    //         /* Read the SCR from the data register. */
    //         uint32_t data = 0;
    //         res = sdhci_regs_get_data(sdhci_regs, &data);
    //         if (result_is_err(res)) {
    //             return result_err_chain(res, "Failed to get data in sdhci_card_init_and_id().");
    //         }
    //         if (num_scr_reads == 0) {
    //             /* Save low portion of SCR register to sdcard. */
    //             res = sdcard_set_scr_raw32_lo(sdcard, data);
    //             sel4cp_dbg_puts("SCR LO is \n");
    //             puthex32(data);
    //             if (result_is_err(res)) {
    //                 return result_err_chain(res, "Failed to set SCR raw32 lo in sdhci_card_init_and_id().");
    //             }
    //         } else {
    //             /* Save high portion of SCR register to sdcard. */
    //             res = sdcard_set_scr_raw32_hi(sdcard, data);
    //             sel4cp_dbg_puts("SCR HI is \n");
    //             puthex32(data);

    //             if (result_is_err(res)) {
    //                 return result_err_chain(res, "Failed to set SCR raw32 hi in sdhci_card_init_and_id().");
    //             }
    //         }
    //         num_scr_reads++;
    //     } else {
    //         usleep(1);
    //         if (--num_scr_retries == 0) {
    //             break;
    //         }
    //     }
    // } while (num_scr_reads < 2);
    // if (num_scr_reads < 2) {
    //     *sdhci_result = SD_TIMEOUT;
    //     return result_err("Failed to read SCR in sdhci_card_init_and_id().");
    // }

    // // Set the block length to be equal to 512
    // sel4cp_dbg_puts("Sending SET_BLOCKLEN (CMD16)...");
    // res = sdhci_send_cmd(
    //         NULL,
    //         sdhci_regs,
    //         IDX_SET_BLOCKLEN,
    //         512,
    //         sdcard,
    //         sdhci_result
    // );
    // if (result_is_err(res)) {
    //     return result_err_chain(res, "Failed to send `SET_BLOCKLEN` in sdhci_card_init_and_id().");
    // }
    // sel4cp_dbg_puts("\nSet the Blocklen to 512 successfully.\n");


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
                NULL,
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
    sel4cp_dbg_puts("\nSDHCI write block checks finished successfully.\n");

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
    sel4cp_dbg_puts("\nBeginning blocks transfer\n");

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
    sel4cp_dbg_puts("\nSDHCI transfer blocks check complete\n");

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
    sel4cp_dbg_puts("\nBuffer length is valid\n");

    result_t res;
    /* Check the SD card type. */
    bool is_sdcard_type_unknown = true;
    res = sdcard_is_type_unknown(sdcard, &is_sdcard_type_unknown);
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to check if SD Card type is unknown in sdhci_transfer_blocks().");
    }
    sel4cp_dbg_puts("\nSD card type is known1\n");

    if (is_sdcard_type_unknown) {
        *sdhci_result = SD_NO_RESP;
        return result_err("SD Card type is unknown in sdhci_transfer_blocks().");
    }

    sel4cp_dbg_puts("\nSD card type is known2\n");

    /* Wait for any data command in progress. */
    res = sdhci_wait_for_data_in_progress(sdhci_regs, sdhci_result);
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to wait for data in progress in sdhci_transfer_blocks().");
    }

    sel4cp_dbg_puts("\nSD card can wait for data in progress\n");

    /* Determine the interrupt and command values for the transfer. */
    // int ready_interrupt;
    int read_or_write_cmd;
    if (is_write) {
        // ready_interrupt = INT_WRITE_RDY;
        if (num_blocks == 1) {
            read_or_write_cmd = IDX_WRITE_SINGLE;
        } else {
            read_or_write_cmd = IDX_WRITE_MULTI;
        }
    } else {
        // ready_interrupt = INT_READ_RDY;
        if (num_blocks == 1) {
            read_or_write_cmd = IDX_READ_SINGLE;
        } else {
            read_or_write_cmd = IDX_READ_MULTI;
        }
    }
    /* Check to see if the card supports multi block transfers. */
    bool is_set_block_count_cmd_supported = false;
    res = sdcard_is_set_block_count_cmd_supported(sdcard, &is_set_block_count_cmd_supported);
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to check if SD Card supports multi block transfers in sdhci_transfer_blocks().");
    }

    sel4cp_dbg_puts("\nSD card can support multi block transfers\n");

    /* If more than one block to transfer, and the card supports it, send
     * SET_BLOCK_COUNT command to indicate the number of blocks to transfer. */
    if (num_blocks > 1 && is_set_block_count_cmd_supported) {
        res = sdhci_send_cmd(
                NULL,
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

    sel4cp_dbg_puts("\nSent block count command\n");

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

    sel4cp_dbg_puts("\nSDHCI card is indeed standard capacity\n");

    /* Set BLKSIZECNT to number of blocks * 512 bytes, send the read or write command.
     * Once the data transfer has started and the TM_BLKCNT_EN bit in the CMDTM
     * register is set the EMMC module automatically decreases the BLKCNT value
     * as the data blocks are transferred and stops the transfer once BLKCNT
     * reaches 0.
     * TODO: TM_AUTO_CMD12 - is this needed?  What effect does it have? */

    sdhci_data_t sdhci_data = {0};

    sdhci_data_t* sdhci_data_ptr = &sdhci_data;

    sdhci_data_ptr->blocks = num_blocks;

    sdhci_data_ptr->blocksize = block_size;

    if (is_write) {
        sel4cp_dbg_puts("This is the pre-buffer talking:\n");
        for (int i = 0; i < buffer_len; i++) {
            sel4cp_dbg_putc(buffer[i]);
            // fake_buf[i] = buffer[i];
        }
    }

    char fake_buf[buffer_len];
    for (int i = 0; i < buffer_len; i++) {
        // sel4cp_dbg_puts("adsf: ");
        // sel4cp_dbg_putc(buffer[i]);
        fake_buf[i] = buffer[i];
    }

    // If it's a write command...
    if (is_write) {
        sdhci_data_ptr->flags |= MMC_DATA_WRITE;

        sdhci_data_ptr->src = fake_buf;
        // sdhci_data_ptr->buffer = buffer;

    } else {
        sdhci_data_ptr->flags |= MMC_DATA_READ;

        // set the destination of the read to be equal to the buffer
        sdhci_data_ptr->dest = buffer;
        // sel4cp_dbg_puts("\nqwerty\n");
        // puthex32(buffer);
        // puthex32(&buffer);

        // sdhci_data_ptr->buffer = buffer;

    }

    //! This is RPi specific.
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
            sdhci_data_ptr,
            sdhci_regs,
            read_or_write_cmd,
            block_addr,
            sdcard,
            sdhci_result
    );
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to send transfer command in sdhci_transfer_blocks().");
    }

    sel4cp_dbg_puts("\nSent write type commands done.\n");

    //? The issue with transferring blocks in the Odroid C4 is super different to the RPi3.
    //? You would load blocks one by one in the RPi, but you need to set up some other DMA addresses otherwise.

    // /* Transfer all blocks. */
    // size_t blocks_done = 0;
    // while (blocks_done < num_blocks) {
    //     /* Wait for ready interrupt for the next block. */
    //     res = sdhci_wait_for_interrupt(
    //             sdhci_regs,
    //             ready_interrupt,
    //             sdhci_result
    //     );
    //     if (result_is_err(res)) {
    //         return result_err_chain(res, "Failed to wait for ready interrupt in sdhci_transfer_blocks().");
    //     }

    //     //! This is RPi specific.
    //     /* Loop through the block 4 bytes (32 bits) at a time. */
    //     for (size_t i = 0; i < (block_size / sizeof(uint32_t)); i++) {
    //         if (is_write) {
    //             res = sdhci_regs_set_data(sdhci_regs, ((uint32_t *) buffer)[i]);
    //             if (result_is_err(res)) {
    //                 return result_err_chain(res, "Failed to set data in sdhci_transfer_blocks().");
    //             }
    //         } else {
                // res = sdhci_regs_get_data(sdhci_regs, &((uint32_t *) buffer)[i]);
    //             if (result_is_err(res)) {
    //                 return result_err_chain(res, "Failed to get data in sdhci_transfer_blocks().");
    //             }
    //         }
    //     }
    //     blocks_done++;
    //     buffer += block_size;
    // }

    // /* If not all bytes were read/written, the operation timed out. */
    // if (blocks_done != num_blocks) {
    //     *sdhci_result = SD_TIMEOUT;
    //     if (!is_write && num_blocks > 1) {
    //         res = sdhci_send_cmd(
    //                 NULL,
    //                 sdhci_regs,
    //                 IDX_STOP_TRANS,
    //                 0,
    //                 sdcard,
    //                 sdhci_result
    //         );
    //         if (result_is_err(res)) {
    //             return result_err_chain(res, "Failed to send STOP_TRANSMISSION command in sdhci_transfer_blocks().");
    //         }
    //     }
    // }

            // sel4cp_dbg_puts("adsf: ");
        // sel4cp_dbg_putc(buffer[i]);

    // char fake_buf[buffer_len];
    if (!is_write) {
        sel4cp_dbg_puts("This is the post-buffer talking:\n");
        for (int i = 0; i < buffer_len; i++) {
            sel4cp_dbg_putc(buffer[i]);
            // fake_buf[i] = buffer[i];
        }
    }

    sel4cp_dbg_puts("\nAll blocks transferred with no timeout.\n");

    //! This is also RPi3 specific and will do nothing. There is probably some equivalent on the Odroid C4 though.
    //TODO: Fix this.
    // /* For a write operation, ensure DATA_DONE interrupt before we stop transmission. */
    // if (is_write) {
    //     res = sdhci_wait_for_interrupt(
    //             sdhci_regs,
    //             INT_DATA_DONE,
    //             sdhci_result
    //     );
    //     if (result_is_err(res)) {
    //         return result_err_chain(res, "Failed to wait for data done interrupt in sdhci_transfer_blocks().");
    //     }
    // }
    // sel4cp_dbg_puts("\nData done interrupt correctly set\n");

    /* For a multi-block operation, if SET_BLOCKCNT is not supported, we need to indicate
     * that there are no more blocks to be transferred. */
    if (num_blocks > 1 && !is_set_block_count_cmd_supported) {
        res = sdhci_send_cmd(
                NULL,
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
    sel4cp_dbg_puts("\nTransmission stopped successfully. \n");

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
    size_t retries_busy = 16000;
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
    usleep(16);
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
    usleep(16);
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
        usleep(16);
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
//TODO: Fix this.
result_t sdhci_wait_for_interrupt(
        sdhci_regs_t *sdhci_regs,
        uint32_t interrupt_mask,
        sdhci_result_t *sdhci_result
) {
    // if (sdhci_regs == NULL) {
    //     return result_err("NULL `sdhci_regs` passed to sdhci_wait_for_interrupt().");
    // }
    // if (sdhci_result == NULL) {
    //     return result_err("NULL `sdhci_result` passed to sdhci_wait_for_interrupt().");
    // }
    // *sdhci_result = SD_ERROR;
    // uint32_t mask_with_error = interrupt_mask | INT_ERROR_MASK;
    // /* Wait for the interrupt. We specify a timeout of 1 second. */
    // size_t retries = 160000;
    // bool is_finished_or_error = false;
    // do {
    //     usleep(16);
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
    size_t retries = 1600000;
    do {
        usleep(16);
        result_t res_cmd = sdhci_regs_is_cmd_in_progress(
                sdhci_regs,
                &cmd_in_progress
        );
        // if (cmd_in_progress) {
        //     sel4cp_dbg_puts("cmd in progress\n");
        //     puthex32(sdhci_regs->regs->sd_emmc_status);
        //     sel4cp_dbg_puts("\n");
        //     puthex32(sdhci_regs->regs->sd_emmc_status & STATUS_END_OF_CHAIN);
        //     if (sdhci_regs->regs->sd_emmc_status & STATUS_END_OF_CHAIN) sel4cp_dbg_puts("what the hell\n");
        //     // puthex32(STATUS_END_OF_CHAIN);

        // }
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
        sel4cp_dbg_puts("\nError occurred in sdhci_wait_for_cmd_in_progress().\n");

        return result_err("Error occurred in sdhci_wait_for_cmd_in_progress().");
    }
    if (cmd_in_progress) {
        *sdhci_result = SD_BUSY;
        sel4cp_dbg_puts("\nTimed out waiting for command in sdhci_wait_for_cmd_in_progress().\n");
        return result_err("Timed out waiting for command in sdhci_wait_for_cmd_in_progress().");
    }
    *sdhci_result = SD_OK;
    return result_ok();
}

void print_cmd_registers(sdhci_regs_t* sdhci_regs) {

    sel4cp_dbg_puts("\nPrinting Everything\n");


    sel4cp_dbg_puts("\n\n");
    sel4cp_dbg_puts("sd_emmc_clock:\n\n");
    puthex32(sdhci_regs->regs->sd_emmc_clock);		    // 0x00 SD_EMMC_CLOCK : // TODO: make own type later
	sel4cp_dbg_puts("\n\n");
    sel4cp_dbg_puts("sd_emmc_delay1:\n\n");
    puthex32(sdhci_regs->regs->sd_emmc_delay1);		// 0x04 DELAY1 = 0xffe05010 FINAL
	sel4cp_dbg_puts("\n\n");
    sel4cp_dbg_puts("sd_emmc_delay2:\n\n");
    puthex32(sdhci_regs->regs->sd_emmc_delay2);		// 0x08 DELAY1 = 0xffe05020 FINAL
	sel4cp_dbg_puts("\n\n");
    sel4cp_dbg_puts("sd_emmc_adjust:\n\n");
    puthex32(sdhci_regs->regs->sd_emmc_adjust);		// 0x0c DELAY1
	sel4cp_dbg_puts("\n\n");
    sel4cp_dbg_puts("sd_emmc_calout:\n\n");
    puthex32(sdhci_regs->regs->sd_emmc_calout);		// 0x10 DELAY1
	sel4cp_dbg_puts("\n\n");
    sel4cp_dbg_puts("undocumented0:\n\n");
    puthex32(sdhci_regs->regs->undocumented0);		    // 0x14 EMPTY
	sel4cp_dbg_puts("\n\n");
    sel4cp_dbg_puts("undocumented1:\n\n");
    puthex32(sdhci_regs->regs->undocumented1);		    // 0x18 EMPTY
	sel4cp_dbg_puts("\n\n");
    sel4cp_dbg_puts("undocumented2:\n\n");
    puthex32(sdhci_regs->regs->undocumented2);		    // 0x1c EMPTY
	sel4cp_dbg_puts("\n\n");
    sel4cp_dbg_puts("sd_emmc_adj_idx_log:\n\n");
    puthex32(sdhci_regs->regs->sd_emmc_adj_idx_log);   // 0x20 ADJ IDX LOG
	sel4cp_dbg_puts("\n\n");
    sel4cp_dbg_puts("sd_emmc_clk_test_log:\n\n");
    puthex32(sdhci_regs->regs->sd_emmc_clk_test_log);	// 0x24 CLK TEST LOG
	sel4cp_dbg_puts("\n\n");
    sel4cp_dbg_puts("sd_emmc_clk_test_out:\n\n");
    puthex32(sdhci_regs->regs->sd_emmc_clk_test_out);	// 0x28 CLK TEST OUT
	sel4cp_dbg_puts("\n\n");
    sel4cp_dbg_puts("sd_emmc_eyetest_log:\n\n");
    puthex32(sdhci_regs->regs->sd_emmc_eyetest_log);	// 0x2c EYETEST LOG
	sel4cp_dbg_puts("\n\n");
    sel4cp_dbg_puts("sd_emmc_eyetest_out0:\n\n");
    puthex32(sdhci_regs->regs->sd_emmc_eyetest_out0);	// 0x30 EYETEST OUT0
	sel4cp_dbg_puts("\n\n");
    sel4cp_dbg_puts("sd_emmc_eyetest_out1:\n\n");
    puthex32(sdhci_regs->regs->sd_emmc_eyetest_out1);	// 0x34 EYETEST OUT0
	sel4cp_dbg_puts("\n\n");
    sel4cp_dbg_puts("sd_emmc_intf3:\n\n");
    puthex32(sdhci_regs->regs->sd_emmc_intf3);	        // 0x38 INTF3
	sel4cp_dbg_puts("\n\n");
    sel4cp_dbg_puts("undocumented3:\n\n");
    puthex32(sdhci_regs->regs->undocumented3);	        // 0x3c EMPTY
	sel4cp_dbg_puts("\n\n");
    sel4cp_dbg_puts("sd_emmc_start:\n\n");
    puthex32(sdhci_regs->regs->sd_emmc_start);	        // 0x40 START
	sel4cp_dbg_puts("\n\n");
    sel4cp_dbg_puts("sd_emmc_cfg:\n\n");
    puthex32(sdhci_regs->regs->sd_emmc_cfg);	        // 0x44 CFG
	sel4cp_dbg_puts("\n\n");
    sel4cp_dbg_puts("sd_emmc_status:\n\n");
    puthex32(sdhci_regs->regs->sd_emmc_status);	    // 0x48 STATUS
	sel4cp_dbg_puts("\n\n");
    sel4cp_dbg_puts("sd_emmc_irq_en:\n\n");
    puthex32(sdhci_regs->regs->sd_emmc_irq_en);	    // 0x4c IRQ EN
	sel4cp_dbg_puts("\n\n");
    sel4cp_dbg_puts("sd_emmc_cmd_cfg:\n\n");
    puthex32(sdhci_regs->regs->sd_emmc_cmd_cfg);	    // 0x50 COMMAND CFG
	sel4cp_dbg_puts("\n\n");
    sel4cp_dbg_puts("sd_emmc_cmd_arg:\n\n");
    puthex32(sdhci_regs->regs->sd_emmc_cmd_arg);	    // 0x54 COMMAND ARG
	sel4cp_dbg_puts("\n\n");
    sel4cp_dbg_puts("sd_emmc_cmd_dat:\n\n");
    puthex32(sdhci_regs->regs->sd_emmc_cmd_dat);	    // 0x58 COMMAND DAT
	sel4cp_dbg_puts("\n\n");
    sel4cp_dbg_puts("sd_emmc_cmd_rsp:\n\n");
    puthex32(sdhci_regs->regs->sd_emmc_cmd_rsp);	    // 0x5c COMMAND RSP
	sel4cp_dbg_puts("\n\n");
    sel4cp_dbg_puts("sd_emmc_cmd_rsp1:\n\n");
    puthex32(sdhci_regs->regs->sd_emmc_cmd_rsp1);	    // 0x60 COMMAND RSP1
	sel4cp_dbg_puts("\n\n");
    sel4cp_dbg_puts("sd_emmc_cmd_rsp2:\n\n");
    puthex32(sdhci_regs->regs->sd_emmc_cmd_rsp2);	    // 0x64 COMMAND RSP2
	sel4cp_dbg_puts("\n\n");
    sel4cp_dbg_puts("sd_emmc_cmd_rsp3:\n\n");
    puthex32(sdhci_regs->regs->sd_emmc_cmd_rsp3);	    // 0x68 COMMAND RSP3
	sel4cp_dbg_puts("\n\n");
    sel4cp_dbg_puts("undocumented4:\n\n");
    puthex32(sdhci_regs->regs->undocumented4);	        // 0x6c RESERVED
	sel4cp_dbg_puts("\n\n");
    sel4cp_dbg_puts("sd_emmc_curr_cfg:\n\n");
    puthex32(sdhci_regs->regs->sd_emmc_curr_cfg);	    // 0x70 CURR CFG
	sel4cp_dbg_puts("\n\n");
    sel4cp_dbg_puts("sd_emmc_curr_arg:\n\n");
    puthex32(sdhci_regs->regs->sd_emmc_curr_arg);	    // 0x74 CURR ARG
	sel4cp_dbg_puts("\n\n");
    sel4cp_dbg_puts("sd_emmc_curr_dat:\n\n");
    puthex32(sdhci_regs->regs->sd_emmc_curr_dat);	    // 0x78 CURR DAT
	sel4cp_dbg_puts("\n\n");
    sel4cp_dbg_puts("sd_emmc_curr_rsp:\n\n");
    puthex32(sdhci_regs->regs->sd_emmc_curr_rsp);	    // 0x7c CURR RSP
	sel4cp_dbg_puts("\n\n");
    sel4cp_dbg_puts("sd_emmc_next_cfg:\n\n");
    puthex32(sdhci_regs->regs->sd_emmc_next_cfg);	    // 0x80 NEXT CFG
	sel4cp_dbg_puts("\n\n");
    sel4cp_dbg_puts("sd_emmc_next_arg:\n\n");
    puthex32(sdhci_regs->regs->sd_emmc_next_arg);	    // 0x84 NEXT ARG
	sel4cp_dbg_puts("\n\n");
    sel4cp_dbg_puts("sd_emmc_next_dat:\n\n");
    puthex32(sdhci_regs->regs->sd_emmc_next_dat);	    // 0x88 NEXT DAT
	sel4cp_dbg_puts("\n\n");
    sel4cp_dbg_puts("sd_emmc_next_rsp:\n\n");
    puthex32(sdhci_regs->regs->sd_emmc_next_rsp);	    // 0x8c NEXT RSP
	sel4cp_dbg_puts("\n\n");
    sel4cp_dbg_puts("sd_emmc_rxd:\n\n");
    puthex32(sdhci_regs->regs->sd_emmc_rxd);	        // 0x90 RXD
	sel4cp_dbg_puts("\n\n");
    sel4cp_dbg_puts("sd_emmc_txd:\n\n");
    puthex32(sdhci_regs->regs->sd_emmc_txd);	        // 0x94 TXD
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
    size_t retries = 160000;
    do {
        usleep(16);
        // result_t res_cmd = result_ok();
        // data_in_progress = true;
        result_t res_cmd = sdhci_regs_is_data_in_progress(
                sdhci_regs,
                &data_in_progress
        );
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
        sdhci_data_t *data,
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

    sel4cp_dbg_puts("\nSDHCI send commands checks finished\n");
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
    sel4cp_dbg_puts("\nRCA checks done. \n");


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

        sel4cp_dbg_puts("\nIn an app command. \n");
        
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
                NULL,
                sdhci_regs,
                app_cmd_index, /* This will change depending on if RCA exists or not. */
                app_cmd_arg,
                sdcard,
                sdhci_result
        );
        if (result_is_err(res)) {
            sel4cp_dbg_puts("\nError in app recursion. \n");

            return result_err_chain(res, "Failed to send app command in sdhci_send_cmd().");
        }

        sel4cp_dbg_puts("\nApp recursion executed fine. \n");

        /* When there is an RCA, we should check the status indicates APP_CMD accepted. */
        if (app_cmd_index == IDX_APP_CMD_RCA) {
            bool is_app_cmd_accepted;
            res = sdcard_is_app_cmd_accepted(sdcard, &is_app_cmd_accepted);
            if (result_is_err(res)) {
                return result_err_chain(res, "Failed to check if app command was accepted in sdhci_send_cmd().");
            }
            if (!is_app_cmd_accepted) {
                *sdhci_result = SD_ERROR;
                sel4cp_dbg_puts("\nApp command not accepted... \n");

                return result_err("App command was not accepted in sdhci_send_cmd().");
            }
        }
    }

    sel4cp_dbg_puts("\nApp commands check done\n");

    // print_cmd_registers(sdhci_regs);

    sel4cp_dbg_puts("\nGetting status: before checking completion.\n");

    // sel4cp_dbg_puts(itoa(sdhci_regs->regs->sd_emmc_status, _num, 16));
    puthex32(sdhci_regs->regs->sd_emmc_status);
    sel4cp_dbg_puts("\n");

    // /* Wait for command in progress. */
    // res = sdhci_wait_for_cmd_in_progress(sdhci_regs, sdhci_result);
    // if (result_is_err(res)) {
    //     return result_err_chain(res, "Failed to wait for command in progress in sdhci_send_cmd().");
    // }

    // /* Clear interrupt flags. */
    // res = sdhci_regs_clear_interrupt(sdhci_regs);
    // if (sdhci_regs->regs == NULL) {
    //     sel4cp_dbg_puts("\nNo inner OC4 regs....\n");
    // }
    // if (result_is_err(res)) {
    //     return result_err_chain(res, "Failed to clear interrupt flags in sdhci_send_cmd().");
    // }

    sel4cp_dbg_puts("\nReady to get SDHCI commands\n");

    // char _num[16];
    sel4cp_dbg_puts("Getting command argument: \n");

    // sel4cp_dbg_puts(itoa(sdhci_regs->regs->sd_emmc_cmd_arg, _num, 16));

    // sel4cp_dbg_puts("getting command cfg INITIALLY: ");

    // sel4cp_dbg_puts(itoa(sdhci_regs->regs->sd_emmc_cmd_cfg, _num, 16));
    // puthex32(sdhci_regs->regs->sd_emmc_cmd_cfg);

    // /* Set the argument register first. */
    // res = sdhci_regs_set_arg1(
    //         sdhci_regs,
    //         arg
    // );
    // if (result_is_err(res)) {
    //     return result_err_chain(res, "Failed to set argument in sdhci_send_cmd().");
    // }

    //TODO This could be the breaking code
    /* Get the command register value stored in `sdhci_cmd`. */
    cmdtm_t cmdtm;
    res = sdhci_cmd_get_cmdtm(
            sdhci_cmd,
            &cmdtm
    );
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to get cmdtm in sdhci_send_cmd().");
    }

    // //! Raspberry Pi specific, but still need!

    // char xxnum[16];
    // sel4cp_dbg_puts("\nTHIS IS WHAT THE CMDTM IS RN\n");
    // sel4cp_dbg_puts(itoa(cmdtm.raw32, xxnum, 16));
    // puthex32(cmdtm.raw32);
    // sel4cp_dbg_puts("\nTHIS IS WHAT THE CMDTM IS RN\n");

    // sel4cp_dbg_puts("\nTHIS IS WHAT THE dat IS RN\n");
    // puthex32(sdhci_regs->regs->sd_emmc_cmd_dat);
    // sel4cp_dbg_puts("\nTHIS IS WHAT THE resp IS RN\n");
    // puthex32(sdhci_regs->regs->sd_emmc_cmd_rsp);


    /* Set the command register to the value obtained from `sdhci_cmd`. */
    // res = sdhci_regs_set_cmdtm(
    //         sdhci_regs,
    //         cmdtm
    // );
    // if (result_is_err(res)) {
    //     return result_err_chain(res, "Failed to set cmdtm in sdhci_send_cmd().");
    // }

    // oc4_emmc_regs_mmc_setup_cmd(mmc, data, cmd);
    //     //! These won't work rn
	// 	// sdhci_regs->regs->sd_emmc_cfg |= ilog2(data->blocksize) << CFG_BL_LEN_SHIFT;

	// 	// if (sdhci_regs->regs->sd_emmc_cmd_dat == MMC_DATA_WRITE)
	// 	// 	meson_mmc_cmd |= CMD_CFG_DATA_WR;

	// 	sdhci_regs->regs->sd_emmc_cmd_cfg |= CMD_CFG_DATA_IO | CMD_CFG_BLOCK_MODE;
    // }

    // char snumpp[16];

    // sdhci_regs->regs->sd_emmc_cmd_cfg = (uint32_t) 7;

    // sel4cp_dbg_puts("getting cmd cfg pre: ");

    // sel4cp_dbg_puts(itoa(sdhci_regs->regs->sd_emmc_cmd_cfg, snumpp, 16));

    // char snumppp[16];


    // sdhci_regs->regs->sd_emmc_cmd_cfg |= (CMD_CFG_OWNER | CMD_CFG_TIMEOUT_4S | CMD_CFG_END_OF_CHAIN);

    // sel4cp_dbg_puts("getting cmd cfg pro: ");

    // sel4cp_dbg_puts(itoa(sdhci_regs->regs->sd_emmc_cmd_cfg, snumppp, 16));

    // //! This won't work rn
	// // oc4_emmc_regs_mmc_setup_addr(mmc, data);


    // //TODO Refactor into a function later: Read response

    // sel4cp_dbg_puts("getting arg: ");

    // sel4cp_dbg_puts(itoa(arg, snum, 16));

    // sel4cp_dbg_puts("getting stored arg: ");

    // sel4cp_dbg_puts(itoa(sdhci_regs->regs->sd_emmc_cmd_arg, snum, 16));

    // sdhci_regs->regs->sd_emmc_cmd_arg = 79;
    // oc4_emmc_regs_set_arg1(sdhci_regs->regs, 2);

    // sel4cp_dbg_puts("getting new stored arg: ");

    // sel4cp_dbg_puts(itoa(sdhci_regs->regs->sd_emmc_cmd_arg, snum, 16));

    //     // sdhci_regs->regs->sd_emmc_cmd_arg = 1;

    sel4cp_dbg_puts("\n========================================================\n");
    sel4cp_dbg_puts("\nBeginning send command\n");
    sel4cp_dbg_puts("\n========================================================\n");


    sel4cp_dbg_puts("\nCMD index is:\n");
    puthex32(cmdtm.CMD_INDEX);

    /* Get the SDHCI command's response type. */
    cmd_rspns_type_t cmd_rspns_type;
    res = sdhci_cmd_get_cmd_rspns_type(
            sdhci_cmd,
            &cmd_rspns_type
    );
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to get cmd_rspns_type in sdhci_send_cmd().");
    }

    sel4cp_dbg_puts("\nCMD response type:\n");
    puthex32(cmd_rspns_type);

    uint32_t meson_mmc_cmd = 0;
    uint32_t meson_mmc_dat = 0;

    sel4cp_dbg_puts("\nPre CMD Status:\n");
    // puthex32(sdhci_regs->regs->sd_emmc_status);
    puthex32(sdhci_regs->regs->sd_emmc_status);
    sel4cp_dbg_puts("\n\n");

	meson_mmc_cmd |= cmdtm.CMD_INDEX << CMD_CFG_CMD_INDEX_SHIFT;

	if (cmdtm.CMD_RSPNS_TYPE) {
        switch (cmdtm.CMD_RSPNS_TYPE) {
            case CMD_136BIT_RESP:
                sel4cp_dbg_puts("\nThe command is a 136BIT resp command.\n");

                meson_mmc_cmd |= CMD_CFG_RESP_128;  
                break;
            case CMD_BUSY48BIT_RESP:
                sel4cp_dbg_puts("\nThe command is a Busy48BIT resp command.\n");

                meson_mmc_cmd |= CMD_CFG_R1B;
                break;
            case CMD_48BIT_RESP:
                sel4cp_dbg_puts("\nThe command is a normal 48BIT resp command.\n");

                // meson_mmc_cmd |= CMD_CFG_R1B;

                // meson_mmc_cmd |= CMD_CFG_RESP_128;
                break;
            default:
                // Default case should fall into while loop below
                sel4cp_dbg_puts("Invalid response type: We should never reach here\n\n");
                break;
        }
			
		if (!(cmdtm.CMD_CRCCHK_EN))
			meson_mmc_cmd |= CMD_CFG_RESP_NOCRC;
	} else {
		meson_mmc_cmd |= CMD_CFG_NO_RESP;
	}
    // meson_mmc_cmd |= CMD_CFG_RESP_128;

    //: Need to deal with data at a later time, aka now. This is the Odroid C4 specific code.
	if (data) {
		uint32_t cfg = sdhci_regs->regs->sd_emmc_cfg;
		cfg &= ~CFG_BL_LEN_MASK;

		cfg |= ilog2(data->blocksize) << CFG_BL_LEN_SHIFT;

        // puthex32(ilog2(data->blocksize));
        sel4cp_dbg_puts("In handle data code...");

        sdhci_regs->regs->sd_emmc_cfg = cfg;

		if (data->flags == MMC_DATA_WRITE) 
            meson_mmc_cmd |= CMD_CFG_DATA_WR;

		meson_mmc_cmd |= CMD_CFG_DATA_IO | CMD_CFG_BLOCK_MODE | data->blocks;
		// meson_mmc_cmd |= BIT(23);

        
	}

	meson_mmc_cmd |= CMD_CFG_TIMEOUT_4S | CMD_CFG_OWNER | CMD_CFG_END_OF_CHAIN;

    // puthex32(sdhci_regs->regs->sd_emmc_cmd_cfg);

    // puthex32(sdhci_regs->regs->sd_emmc_cmd_cfg);

    //? Following U-Boot spec here
    uint32_t data_addr = 0;


    /*
        ...read...                
        buff ====> data->dest
        ...write...
        data->src =====> buff
    */
    sel4cp_dbg_puts("hello!\n\n");

	if (data) {
		// uint32_t data_size = data->blocks * data->blocksize;

        // puthex32(data->flags);
        // puthex32(MMC_DATA_READ);

		if (data->flags == MMC_DATA_READ) {
            // if read, set the address to be the data destination
            sel4cp_dbg_puts("I am a read!\n\n");

            // READ INTO THIS BUFFER
			data_addr = (long) data->dest;

            //TODO: fix
			// invalidate_dcache_range(data_addr, data_addr + data_size);
		} else {
            sel4cp_dbg_puts("I am a write!\n\n");

            // else, copy the data into the buffer from the data source
			// data->buffer = malloc(data_size * sizeof(char));
            // void * shared_sd_data_block = data->buffer;
            
            // sel4cp_dbg_puts("malloc was ok!\n\n");

            // bzero(data->buffer, data_size);
            // sel4cp_dbg_puts("bzero was ok!\n\n");

            // WRITE FROM THIS BUFFER
			data_addr = (long) data->src;

			// memcpy((void *) data->buffer, data->src, data_size);

            //TODO: fix
			// flush_dcache_range(data_addr, data_addr + data_size);
		}
	}

    meson_mmc_dat = data_addr;



    //? Following Linux spec here
    sdhci_regs->regs->sd_emmc_cmd_cfg = meson_mmc_cmd;
    sdhci_regs->regs->sd_emmc_cmd_dat = meson_mmc_dat;
    // sdhci_regs->regs->sd_emmc_cmd_dat = 1;
    sel4cp_dbg_puts("\n\nData is: \n");
    // sdhci_regs->regs->sd_emmc_cmd_dat = meson_mmc_dat;

    puthex32(sdhci_regs->regs->sd_emmc_cmd_dat);
    sel4cp_dbg_puts("\n");
    puthex32(meson_mmc_dat);
    sel4cp_dbg_puts("\n");
    puthex32(sdhci_regs->regs->sd_emmc_cmd_dat);
    sel4cp_dbg_puts("\n");

    sdhci_regs->regs->sd_emmc_cmd_rsp = 0;
    sdhci_regs->regs->sd_emmc_cmd_arg = arg;

    //TODO Refactor into a function later

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

    sel4cp_dbg_puts("\nFinished delaying. \n");

    /* Wait until command complete interrupt */
    res = sdhci_wait_for_interrupt(
            sdhci_regs,
            INT_CMD_DONE,
            sdhci_result
    );
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to wait for command complete interrupt in sdhci_send_cmd().");
    }

    // //! This is never loading anything in because of the data register not writing anything in buasdnfasdmfkjsnzxcbjknv
    // if (data) {
    //     while (!((char *)data->dest)[0]) {
    //         sel4cp_dbg_putc('.');
    //         sel4cp_dbg_putc(data->dest[0]);

    //     }
    // }

    // sel4cp_dbg_puts("\nGotten interrupt\n");
    sel4cp_dbg_puts("\n\nWaiting for CMD to finish...\n\n");

    /* Wait for command in progress (this is actually the interrupt we are waiting for). */
    sel4cp_dbg_puts("Status before waiting for command:\n");
    puthex32(sdhci_regs->regs->sd_emmc_status);
    sel4cp_dbg_puts("\n\n");

        /* Wait for command in progress (this is actually the interrupt we are waiting for). */
    sel4cp_dbg_puts("Data before waiting for command:\n");
    puthex32(sdhci_regs->regs->sd_emmc_cmd_dat);
    sel4cp_dbg_puts("\n\n");


    res = sdhci_wait_for_cmd_in_progress(sdhci_regs, sdhci_result);


    sel4cp_dbg_puts("Status after waiting for command:\n");
    puthex32(sdhci_regs->regs->sd_emmc_status);
    sel4cp_dbg_puts("\n\n");

    // puthex32(sdhci_regs->regs->sd_emmc_cmd_dat);

    // sel4cp_dbg_puts("Resp:\n\n");
    // puthex32(sdhci_regs->regs->sd_emmc_cmd_rsp);
    // puthex32(sdhci_regs->regs->sd_emmc_cmd_rsp1);
    // puthex32(sdhci_regs->regs->sd_emmc_cmd_rsp2);
    // puthex32(sdhci_regs->regs->sd_emmc_cmd_rsp3);
    // sel4cp_dbg_puts("\n\n");

    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to wait for command in progress in sdhci_send_cmd().");
    }

    sel4cp_dbg_puts("\nCMD finished successfully. The current status is\n");

    sel4cp_dbg_puts("Status:\n");
    puthex32(sdhci_regs->regs->sd_emmc_status);
    sel4cp_dbg_puts("\n\n");

    sel4cp_dbg_puts("Data:\n");
    puthex32(sdhci_regs->regs->sd_emmc_cmd_dat);
    sel4cp_dbg_puts("\n\n");

    // sel4cp_dbg_puts("Resp0:\n\n");
    // puthex32(sdhci_regs->regs->sd_emmc_cmd_dat);
    // puthex32(sdhci_regs->regs->sd_emmc_cmd_dat);
    // sel4cp_dbg_puts("\n\n");

    if (data && data->flags == MMC_DATA_READ) {
        sel4cp_dbg_puts("Reading the first byte from destination...: ");
        sel4cp_dbg_putc(data->dest[0]);
    }

    // print_cmd_registers(sdhci_regs);



    // sel4cp_dbg_puts("Command Config:\n\n");
    // puthex32(sdhci_regs->regs->sd_emmc_cmd_cfg);
    // sel4cp_dbg_puts("\n\n");

    // sel4cp_dbg_puts("Arg:\n\n");
    // puthex32(sdhci_regs->regs->sd_emmc_cmd_arg);
    // sel4cp_dbg_puts("\n\n");
    // puthex32(arg);
    // sel4cp_dbg_puts("\n\n");


    /* Reset status bits */
    sdhci_regs->regs->sd_emmc_status = STATUS_MASK;

    /* Get the response from `resp0`. */
    uint32_t resp0 = 0;
    resp0 = sdhci_regs->regs->sd_emmc_cmd_rsp;

    sel4cp_dbg_puts("Resp0:\n\n");
    puthex32(sdhci_regs->regs->sd_emmc_cmd_rsp);
    sel4cp_dbg_puts("\n\n");

    /* Handle response depending on the SDHCI command's response type. */
    //! Be careful with the following response types. Adding more will require you to change these.
    switch (cmd_rspns_type) {
        case CMD_NO_RESP: {
            sel4cp_dbg_puts("\nGotten cmd response type, it's CMD_NO_RESP\n");

            *sdhci_result = SD_OK;
            return result_ok();
        }
        case CMD_BUSY48BIT_RESP: {
            /* `IDX_CARD_SELECT` will enter here. */
            sel4cp_dbg_puts("\nGotten cmd response type, it's CMD_BUSY48BIT_RESP\n");

            res = sdcard_set_status(sdcard, resp0);
            if (result_is_err(res)) {
                return result_err_chain(res, "Failed to set status in sdhci_send_cmd().");
            }
            sel4cp_dbg_puts("\nSet status successfully.\n");

            bool resp_r1_has_err = true;
            res = sdhci_resp_r1_has_error(resp0, &resp_r1_has_err);

            if (result_is_err(res)) {
                return result_err_chain(res, "Failed to check if resp_r1 has error in sdhci_send_cmd().");
            }
            sel4cp_dbg_puts("\nThere is no error.\n");
            // puthex32(resp_r1_has_err);

            if (resp_r1_has_err) {
                return result_err("Response from SD card indicates error in `CMD_BUSY48BIT_RESP` case in sdhci_send_cmd().");
            }
            sel4cp_dbg_puts("\nThe response from SD card indicates that there is no error.\n");
            *sdhci_result = SD_OK;
            return result_ok();
        }
        case CMD_48BIT_RESP: {
            sel4cp_dbg_puts("\nGotten cmd response type, it's CMD_48BIT_RESP\n");

            /* Obtain the command index. */
            // cmd_index_t cmd_index = cmdtm.CMD_INDEX;

            cmd_index_t cmd_index = 0;
            result_t res_get_cmd_index = sdhci_cmd_get_cmd_index(
                    sdhci_cmd,
                    &cmd_index
            );
            
            sel4cp_dbg_puts("\nThe command index is: ");
            
            puthex32(cmd_index);
            if (result_is_err(result_ok())) {
                sel4cp_dbg_puts("\nuhoh, couldn't get cmd index \n");
                return result_err_chain(res_get_cmd_index, "Failed to get cmd_index in sdhci_send_cmd().");
            }
            switch (cmd_index) {
                case 0x03: {
                    /* This is the switch-case for `IDX_SEND_REL_ADDR`.
                     * RESP0 contains the RCA and status bits 23,22,19,12:0. */
                    /* Get the RCA from `resp0`. */
                    sel4cp_dbg_puts("getting resp0: xxxxxxxx");

                    char xnum[16];
                    sel4cp_dbg_puts(itoa(resp0, xnum, 16));
                    sel4cp_dbg_puts("\nGotten cmd response type, it's CMD_48BIT_RESP, 0x03\n");

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
                    sel4cp_dbg_puts("\nGotten cmd response type, it's CMD_48BIT_RESP, 0x08\n");

                    if (resp0 == arg) {
                        *sdhci_result = SD_OK;
                        return result_ok();
                    } else {
                        *sdhci_result = SD_ERROR;
                        sel4cp_dbg_puts("\nGotten cmd response type, not match\n");
                        puthex32(sdcard->ocr.raw32);

                        return result_err("Response from SD card does not match argument in sdhci_send_cmd().");
                    }
                }
                case 0x29: {
                    /* Response handling for `IDX_APP_SEND_OP_COND`. Save the
                     * RESP0 register as the Operation Conditions Register (OCR)
                     * for the `sdcard`. */
                    sel4cp_dbg_puts("\nGotten cmd response type, it's CMD_48BIT_RESP, 0x29\n");

                    sel4cp_dbg_puts("getting resp0: ");

                    char snum[16];
                    sel4cp_dbg_puts(itoa(resp0, snum, 16));
                    sel4cp_dbg_puts("\n");

                    res = sdcard_set_ocr_raw32(sdcard, resp0);
                    while (result_is_err(res)) {
                        res = sdcard_set_ocr_raw32(sdcard, resp0);
                        sel4cp_dbg_puts("trying 0x29 and ");
                        return result_err_chain(res, "Failed to set OCR in sdhci_send_cmd().");
                    }
                    *sdhci_result = SD_OK;
                    return result_ok();
                }
                default: {
                    /* Save the response as the SD card's status. */
                    sel4cp_dbg_puts("\nGotten cmd response type, it's CMD_48BIT_RESP, default\n");

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
            sel4cp_dbg_puts("\nGotten cmd response type, it's CMD_136BIT_RESP\n");

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

                *sdhci_result = SD_OK;
                return result_ok();
            }
        }
    }

    return result_err("Response handling for command has not been implemented in sdhci_send_cmd().");
}




