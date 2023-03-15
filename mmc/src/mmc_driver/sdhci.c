#include "sdhci.h"

result_t sdhci_card_init_and_id(
        bcm_emmc_regs_t *bcm_emmc_regs,
        sdcard_t *sdcard,
        sdhci_result_t *sdhci_result
) {
    if (bcm_emmc_regs == NULL) {
        return result_err("bcm_emmc_regs is NULL in sdhci_card_init_and_id().");
    }
    if (sdcard == NULL) {
        return result_err("sdcard is NULL in sdhci_card_init_and_id().");
    }
    if (sdhci_result == NULL) {
        return result_err("sdhci_result is NULL in sdhci_card_init_and_id().");
    }
    *sdhci_result = SD_ERROR;
    result_t res;

    /* Sending GO_IDLE command. */
    log_trace("Sending GO_IDLE (CMD0) command...");
    sdhci_result_t sdhci_res_go_idle;
    res = sdhci_send_cmd(
            bcm_emmc_regs,
            IX_GO_IDLE_STATE,
            0,
            sdcard,
            &sdhci_res_go_idle
    );
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to send `IX_GO_IDLE_STATE` in sdhci_card_init_and_id().");
    }

    /* Sending SEND_IF_COND,0x000001AA (CMD8) voltage range 0x1 check pattern 0xAA.
     * TODO: If voltage range and check pattern don't match, look for older card. */
    log_trace("Sending IF_COND (CMD8) command...");
    sdhci_result_t sdhci_res_if_cond;
    res = sdhci_send_cmd(
            bcm_emmc_regs,
            IX_SEND_IF_COND,
            0x000001AA,
            sdcard,
            &sdhci_res_if_cond
    );
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to send `IX_SEND_IF_COND` in sdhci_card_init_and_id().");
    }

    /* If card responded with voltage and check pattern, resolve voltage and
     * check for high capacity card with IX_APP_SEND_OP_COND. */
    size_t retries = 7;
    bool has_powered_up = false;
    do {
        usleep(400000);
        res = sdhci_send_cmd(
                bcm_emmc_regs,
                IX_APP_SEND_OP_COND,
                ACMD41_ARG_HC,
                sdcard,
                sdhci_result
        );
        if (result_is_err(res)) {
            return result_err_chain(res, "Failed to send `IX_APP_SEND_OP_COND` in sdhci_card_init_and_id().");
        }
        res = sdcard_has_powered_up(sdcard, &has_powered_up);
        if (result_is_err(res)) {
            return result_err_chain(res, "Failed to check if card has powered up in sdhci_card_init_and_id().");
        }
    } while (!has_powered_up && (retries-- > 0));
    /* If the card still hasn't powered up, we timeout. */
    if (!has_powered_up) {
        *sdhci_result = SD_TIMEOUT;
        return result_err("EMMC card did not power up in sdhci_card_init_and_id().");
    }

    /* Check voltage */
    log_trace("Checking card voltage is 3v3...");
    bool has_correct_voltage = false;
    res = sdcard_is_voltage_3v3(sdcard, &has_correct_voltage);
    if (result_is_err(res)) {
        return result_err("Failed to check if card has correct voltage in sdhci_card_init_and_id().");
    }
    if (!has_correct_voltage) {
        *sdhci_result = SD_ERROR_VOLTAGE;
        return result_err("EMMC card does not have correct voltage in sdhci_card_init_and_id().");
    }
    log_trace("Voltage good.");

    /* Check card capacity. */
    log_trace("Checking card type...");
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
        log_trace("Card high capacity (HC).");
    } else {
        res = sdcard_set_type(sdcard, SD_TYPE_2_SC);
        if (result_is_err(res)) {
            return result_err_chain(res, "Failed to set card type to SD_TYPE_2_SC in sdhci_card_init_and_id().");
        }
        log_trace("Card standard capacity (SC).");
    }

    /* Send ALL_SEND_CID (CMD2). */
    log_trace("Sending ALL_SEND_CID (CMD2) command...");
    res = sdhci_send_cmd(
            bcm_emmc_regs,
            IX_ALL_SEND_CID,
            0,
            sdcard,
            sdhci_result
    );
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to send `IX_ALL_SEND_CID` in sdhci_card_init_and_id().");
    }

    /* Send SEND_REL_ADDR (CMD3). */
    /* TODO: In theory, loop back to SEND_IF_COND to find additional cards. */
    log_trace("Sending SEND_REL_ADDR (CMD3) command...");
    res = sdhci_send_cmd(
            bcm_emmc_regs,
            IX_SEND_REL_ADDR,
            0,
            sdcard,
            sdhci_result
    );
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to send `IX_SEND_REL_ADDR` in sdhci_card_init_and_id().");
    }
    /* SD card will have an RCA at this point. */

    /* Set the clock to full speed. */
    log_trace("Setting clock to full-speed frequency (25GHz)...");
    res = sdhci_set_sd_clock(bcm_emmc_regs, 25000000);
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to set clock to full speed in sdhci_card_init_and_id().");
    }

    /* Get the sdcard's RCA. */
    log_trace("Obtaining SD card's Relative Card Address (RCA)...");
    uint32_t rca = 0;
    res = sdcard_get_rca(sdcard, &rca);
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to get card's RCA in sdhci_card_init_and_id().");
    }
    /* Call the `IX_CARD_SELECT` CMD using the RCA. */
    /* TODO: Check card_is_locked status in the R1 response from CMD7 [bit 25],
     * if so, use CMD42 to unlock CMD42 structure [4.3.7] same as a single block
     * write; data block includes PWD setting mode, PWD len, PWD data.*/
    log_trace("Sending CARD_SELECT (CMD7)...");
    res = sdhci_send_cmd(
            bcm_emmc_regs,
            IX_CARD_SELECT,
            rca,
            sdcard,
            sdhci_result
    );
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to send `IX_CARD_SELECT` in sdhci_card_init_and_id().");
    }

    /* Reading from the SD Card Configuration Register (SCR).
     * SEND_SCR command is like a READ_SINGLE but for a block of 8 bytes.*/
    log_trace("Reading from the SD Card Configuration Register (SCR)...");
    /* Wait for any data operation that might be in progress before reading the block. */
    res = sdhci_wait_for_data_in_progress(bcm_emmc_regs, sdhci_result);
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to wait for data in progress in sdhci_card_init_and_id().");
    }
    /* Set BLKSIZECNT to 1 block of 8 bytes. */
    log_trace("Setting BLKSIZECNT to 1 block of 8 bytes...");
    res = bcm_emmc_regs_set_block_count(bcm_emmc_regs, 1);
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to set block count to 1 in sdhci_card_init_and_id().");
    }
    res = bcm_emmc_regs_set_block_size(bcm_emmc_regs, 8);
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to set block size to 8 in sdhci_card_init_and_id().");
    }
    /* Get the SCR by sending the `SEND_SCR` command. We need the SCR to figure
     * out what the allowed bus widths are. */
    log_trace("Sending SEND_SCR (CMD51) command...");
    res = sdhci_send_cmd(
            bcm_emmc_regs,
            IX_SEND_SCR,
            0,
            sdcard,
            sdhci_result
    );
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to send `IX_SEND_SCR` in sdhci_card_init_and_id().");
    }
    /* Wait until we're ready to read. */
    res = sdhci_wait_for_interrupt(
            bcm_emmc_regs,
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
        res = bcm_emmc_regs_is_read_ready(bcm_emmc_regs, &is_read_ready);
        if (result_is_err(res)) {
            return result_err_chain(res, "Failed to check if read ready in sdhci_card_init_and_id().");
        }
        if (is_read_ready) {
            /* Read the SCR from the data register. */
            uint32_t data = 0;
            res = bcm_emmc_regs_get_data(bcm_emmc_regs, &data);
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
    /* Check the bus width. */
    bool is_bus_width_4_bit = false;
    res = sdcard_is_bus_width_4(sdcard, &is_bus_width_4_bit);
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to check if bus width is 4 bit in sdhci_card_init_and_id().");
    }
    /* Send APP_SET_BUS_WIDTH (ACMD6) command. If supported, set 4 bit bus width
     * and update the CONTROL0 register. */
    if (is_bus_width_4_bit) {
        log_trace("Setting bus width to 4 bit...");
        res = sdhci_send_cmd(
                bcm_emmc_regs,
                IX_SET_BUS_WIDTH,
                rca | 2,
                sdcard,
                sdhci_result
        );
        if (result_is_err(res)) {
            return result_err_chain(res, "Failed to send `IX_SET_BUS_WIDTH` in sdhci_card_init_and_id().");
        }
        /* Sets the bus width to 4 bit. */
        res = bcm_emmc_regs_set_bus_width_4(
                bcm_emmc_regs,
                true
        );
        if (result_is_err(res)) {
            return result_err_chain(res, "Failed to use 4 data lines in sdhci_card_init_and_id().");
        }
    }

    return result_ok();
}

result_t sdhci_read_blocks(
        bcm_emmc_regs_t *bcm_emmc_regs,
        sdcard_t *sdcard,
        size_t lba,
        size_t num_blocks,
        char *dst_buffer,
        sdhci_result_t *sdhci_result
) {
    if (bcm_emmc_regs == NULL) {
        return result_err("NULL `bcm_emmc_regs` passed to sdhci_read_blocks().");
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
            bcm_emmc_regs,
            sdcard,
            lba,
            num_blocks,
            false,
            dst_buffer,
            sdhci_result
    );
}

result_t sdhci_write_blocks(
        bcm_emmc_regs_t *bcm_emmc_regs,
        sdcard_t *sdcard,
        size_t lba,
        size_t num_blocks,
        char *src_buffer,
        sdhci_result_t *sdhci_result
) {
    if (bcm_emmc_regs == NULL) {
        return result_err("NULL `bcm_emmc_regs` passed to sdhci_write_blocks().");
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
    /* Transfer blocks for writing to the SD Card. */
    return sdhci_transfer_blocks(
            bcm_emmc_regs,
            sdcard,
            lba,
            num_blocks,
            true,
            src_buffer,
            sdhci_result
    );
}

result_t sdhci_transfer_blocks(
        bcm_emmc_regs_t *bcm_emmc_regs,
        sdcard_t *sdcard,
        size_t lba,
        size_t num_blocks,
        bool is_write,
        char *buffer,
        sdhci_result_t *sdhci_result
) {
    if (bcm_emmc_regs == NULL) {
        return result_err("NULL `bcm_emmc_regs` passed to sdhci_transfer_blocks().");
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
    result_t res;
    bool is_sdcard_type_unknown = true;
    res = sdcard_is_type_unknown(sdcard, &is_sdcard_type_unknown);
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to check if SD Card type is unknown in sdhci_transfer_blocks().");
    }
    if (is_sdcard_type_unknown) {
        *sdhci_result = SD_NO_RESP;
        return result_err("SD Card type is unknown in sdhci_transfer_blocks().");
    }
    res = sdhci_wait_for_data_in_progress(bcm_emmc_regs, sdhci_result);

    return result_ok();
}

result_t sdhci_get_sd_clock_divisor(
        bcm_emmc_regs_t *bcm_emmc_regs,
        uint32_t freq,
        uint32_t *ret_val
) {
    if (bcm_emmc_regs == NULL) {
        return result_err("NULL `bcm_emmc_regs` passed to sdhci_get_sd_clock_divisor().");
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
    /* Obtain the controller's Host Controller Spec Version. */
    uint8_t host_controller_spec_version = 0;
    result_t res = bcm_emmc_regs_get_host_controller_spec_version(
            bcm_emmc_regs,
            &host_controller_spec_version
    );
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to get host controller spec version in sdhci_get_sd_clock_divisor().");
    }
    /* If the Host Controller Spec Version is 1 or 2, we're in "8-bit Divided
     * Clock Mode". */
    if (host_controller_spec_version < 2) {
        uint_fast8_t shiftcount = arith_find_most_sig_bit_set(divisor);
        /* Note the offset of shift by 1 (look at the spec) */
        if (shiftcount > 0) shiftcount--;
        /* It's only 8 bits maximum on HOST_SPEC_V2 */
        if (shiftcount > 7) shiftcount = 7;
        /* Version 1 and 2 take power 2 */
        divisor = ((uint32_t) 1 << shiftcount);
    }
    /* TODO: Check this. */
    else if (divisor < 3) {
        divisor = 4;
    }
    *ret_val = divisor;
    return result_ok();
}

result_t sdhci_set_sd_clock(bcm_emmc_regs_t *bcm_emmc_regs, uint32_t freq) {
    if (bcm_emmc_regs == NULL) {
        return result_err("NULL `bcm_emmc_regs` passed to sdhci_set_sd_clock().");
    }
    size_t retries_busy = 10000;
    bool cmd_or_data_lines_busy = false;
    do {
        usleep(1);
        bool data_lines_busy = false;
        result_t res = bcm_emmc_regs_is_data_lines_busy(
                bcm_emmc_regs,
                &data_lines_busy
        );
        if (result_is_err(res)) {
            return result_err_chain(res, "Failed to check if data lines are busy in sdhci_set_sd_clock().");
        }
        bool cmd_line_busy = false;
        res = bcm_emmc_regs_is_cmd_line_busy(
                bcm_emmc_regs,
                &cmd_line_busy
        );
        if (result_is_err(res)) {
            return result_err_chain(res, "Failed to check if cmd line is busy in sdhci_set_sd_clock().");
        }
        cmd_or_data_lines_busy = data_lines_busy || cmd_line_busy;
    } while(cmd_or_data_lines_busy && (retries_busy-- > 0));
    if (cmd_or_data_lines_busy) {
        return result_err("Timed out waiting for data/cmd lines to be free in sdhci_set_sd_clock().");
    }
    /* Disable SD clock. */
    result_t res = bcm_emmc_regs_disable_sd_clock(bcm_emmc_regs);
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to disable SD clock in sdhci_set_sd_clock().");
    }
    usleep(10);
    /* Get the SD clock divisor we want to set. */
    uint32_t sd_clock_divisor = 0;
    res = sdhci_get_sd_clock_divisor(
            bcm_emmc_regs,
            freq,
            &sd_clock_divisor
    );
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to get SD clock divisor in sdhci_set_sd_clock().");
    }
    /* Set SD Clock Mode to "Divided". */
    res = bcm_emmc_regs_set_sd_clock_mode_as_divided(bcm_emmc_regs);
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to set SD clock mode to \"Divided\" in sdhci_set_sd_clock().");
    }
    /* Set SD Clock Divisor. */
    res = bcm_emmc_regs_set_sd_clock_divisor(
            bcm_emmc_regs,
            sd_clock_divisor
    );
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to set SD clock divisor in sdhci_set_sd_clock().");
    }
    usleep(10);
    /* Enable the SD Clock. */
    res = bcm_emmc_regs_enable_sd_clock(bcm_emmc_regs);
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to enable SD clock in sdhci_set_sd_clock().");
    }
    log_trace("Setting SD clock to %uHz (divisor: %u).", freq, sd_clock_divisor);
    /* Wait for the SD Clock to stabilise. The SDHCI specifies timeout as 150
     * milliseconds (150,000 microseconds) */
    size_t retries_stable = 15000;
    bool is_sd_clock_stable = false;
    do {
        usleep(10);
        res = bcm_emmc_regs_is_sd_clock_stable(
                bcm_emmc_regs,
                &is_sd_clock_stable
        );
        if (result_is_err(res)) {
            return result_err_chain(res, "Failed to check if SD clock is stable in sdhci_set_sd_clock().");
        }
    } while (!is_sd_clock_stable && (retries_stable-- > 0));
    if (!is_sd_clock_stable) {
        return result_err("Timed out waiting for SD clock to be stable in sdhci_set_sd_clock().");
    }
    log_trace("SD clock is stable.");
    return result_ok();
}

result_t sdhci_wait_for_interrupt(
        bcm_emmc_regs_t *bcm_emmc_regs,
        uint32_t interrupt_mask,
        sdhci_result_t *sdhci_result
) {
    if (bcm_emmc_regs == NULL) {
        return result_err("NULL `bcm_emmc_regs` passed to sdhci_wait_for_interrupt().");
    }
    if (sdhci_result == NULL) {
        return result_err("NULL `sdhci_result` passed to sdhci_wait_for_interrupt().");
    }
    *sdhci_result = SD_ERROR;
    uint32_t mask_with_error = interrupt_mask | INT_ERROR_MASK;
    /* Wait for the interrupt. We specify a timeout of 1 second. */
    size_t retries = 100000;
    bool is_finished_or_error = false;
    do {
        usleep(10);
        result_t res = bcm_emmc_regs_mask_interrupt(
                bcm_emmc_regs,
                mask_with_error,
                &is_finished_or_error
        );
        if (result_is_err(res)) {
            return result_err_chain(res, "Failed to wait for interrupt in sdhci_wait_for_interrupt().");
        }
    } while (!is_finished_or_error && (retries-- > 0));
    /* Read interrupt. */
    uint32_t interrupt_raw32 = 0;
    result_t res_get_raw32 = bcm_emmc_regs_get_interrupt_raw32(
            bcm_emmc_regs,
            &interrupt_raw32
    );
    if (result_is_err(res_get_raw32)) {
        return result_err_chain(res_get_raw32, "Failed to get `interrupt` in sdhci_wait_for_interrupt().");
    }
    /* Timeout case. */
    if (!is_finished_or_error) {
        *sdhci_result = SD_TIMEOUT;
        bool is_cmd_timeout = false;
        result_t res = bcm_emmc_regs_is_cmd_timeout_err(
                bcm_emmc_regs,
                &is_cmd_timeout
        );
        if (result_is_err(res)) {
            return result_err_chain(res, "Failed to get `interrupt.CTO_ERR` in sdhci_wait_for_interrupt().");
        }
        bool is_data_timeout = false;
        res = bcm_emmc_regs_is_data_timeout_err(
                bcm_emmc_regs,
                &is_data_timeout
        );
        if (result_is_err(res)) {
            return result_err_chain(res, "Failed to get `interrupt.DTO_ERR` in sdhci_wait_for_interrupt().");
        }
        /* Clear the interrupt register. */
        res = bcm_emmc_regs_set_interrupt_raw32(
                bcm_emmc_regs,
                interrupt_raw32
        );
        if (result_is_err(res)) {
            return result_err_chain(res, "Failed to clear `interrupt` in sdhci_wait_for_interrupt().");
        }
        /* Return error depending on the type of timeout. */
        if (is_cmd_timeout) {
            return result_err("Command timeout error in sdhci_wait_for_interrupt().");
        }
        if (is_data_timeout) {
            return result_err("Data timeout error in sdhci_wait_for_interrupt().");
        }
        return result_err("Timed out waiting for interrupt in sdhci_wait_for_interrupt().");
    }
    /* Error case. */
    bool is_error = false;
    result_t res = bcm_emmc_regs_mask_interrupt(bcm_emmc_regs, INT_ERROR_MASK, &is_error);
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to check for error in sdhci_wait_for_interrupt().");
    }
    if (is_error) {
        /* Clear the interrupt register. */
        res = bcm_emmc_regs_set_interrupt_raw32(bcm_emmc_regs, interrupt_raw32);
        if (result_is_err(res)) {
            return result_err_chain(res, "Failed to clear `interrupt` in sdhci_wait_for_interrupt().");
        }
        return result_err("Error interrupt in sdhci_wait_for_interrupt().");
    }
    /* Clear the interrupt register. */
    res = bcm_emmc_regs_set_interrupt_raw32(bcm_emmc_regs, interrupt_mask);
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to clear `interrupt` in sdhci_wait_for_interrupt().");
    }
    *sdhci_result = SD_OK;
    /* Success case. */
    return result_ok();
}

result_t sdhci_wait_for_cmd_in_progress(
        bcm_emmc_regs_t *bcm_emmc_regs,
        sdhci_result_t *sdhci_result
) {
    if (bcm_emmc_regs == NULL) {
        return result_err("NULL `bcm_emmc_regs` passed to sdhci_wait_for_cmd_in_progress().");
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
        result_t res_cmd = bcm_emmc_regs_is_cmd_in_progress(
                bcm_emmc_regs,
                &cmd_in_progress
        );
        if (result_is_err(res_cmd)) {
            return result_err_chain(res_cmd, "Failed to check if command is in progress in sdhci_wait_for_cmd_in_progress().");
        }
        result_t res_err = bcm_emmc_regs_is_any_err(
                bcm_emmc_regs,
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
        bcm_emmc_regs_t *bcm_emmc_regs,
        sdhci_result_t *sdhci_result
) {
    if (bcm_emmc_regs == NULL) {
        return result_err("NULL `bcm_emmc_regs` passed to sdhci_wait_for_data_in_progress().");
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
        result_t res_cmd = bcm_emmc_regs_is_data_in_progress(
                bcm_emmc_regs,
                &data_in_progress
        );
        if (result_is_err(res_cmd)) {
            return result_err_chain(res_cmd, "Failed to check if data is in progress in sdhci_wait_for_data_in_progress().");
        }
        result_t res_err = bcm_emmc_regs_is_any_err(
                bcm_emmc_regs,
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

#define R1_ERRORS_MASK       0xfff9c004

result_t sdhci_send_cmd(
        bcm_emmc_regs_t *bcm_emmc_regs,
        size_t sdhci_cmd_index,
        uint32_t arg,
        sdcard_t *sdcard,
        sdhci_result_t *sdhci_result
) {
    if (bcm_emmc_regs == NULL) {
        return result_err("NULL `bcm_emmc_regs` passed to sdhci_send_cmd().");
    }
    if (sdcard == NULL) {
        return result_err("NULL `sdcard` passed to sdhci_send_cmd().");
    }
    if (sdhci_result == NULL) {
        return result_err("NULL `sdhci_result` passed to sdhci_send_cmd().");
    }
    *sdhci_result = SD_ERROR;
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
        size_t app_cmd_index = IX_APP_CMD;
        uint32_t app_cmd_arg = 0;
        /* If yes, use the RCA as the `app_cmd_arg`. */
        if (has_rca) {
            app_cmd_index = IX_APP_CMD_RCA;
            res = sdcard_get_rca(sdcard, &app_cmd_arg);
            if (result_is_err(res)) {
                return result_err_chain(res, "Failed to get RCA in sdhci_send_cmd().");
            }
        }
        /* `app_cmd_arg` will be 0 if no RCA and will be RCA otherwise. This is
         * a recursive call to ourselves. */
        res = sdhci_send_cmd(
                bcm_emmc_regs,
                app_cmd_index, /* This will change depending on if RCA exists or not. */
                app_cmd_arg,
                sdcard,
                sdhci_result
        );
        if (result_is_err(res)) {
            return result_err_chain(res, "Failed to send app command in sdhci_send_cmd().");
        }
        /* When there is an RCA, we should check the status indicates APP_CMD accepted. */
        if (app_cmd_index == IX_APP_CMD_RCA) {
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

    /* Wait for command in progress. */
    res = sdhci_wait_for_cmd_in_progress(bcm_emmc_regs, sdhci_result);
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to wait for command in progress in sdhci_send_cmd().");
    }

    /* Clear interrupt flags. */
    res = bcm_emmc_regs_clear_interrupt(bcm_emmc_regs);
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to clear interrupt flags in sdhci_send_cmd().");
    }

    /* Set the argument register first. */
    res = bcm_emmc_regs_set_arg1(
            bcm_emmc_regs,
            arg
    );
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to set argument in sdhci_send_cmd().");
    }
    /* Get the command register value stored in `sdhci_cmd`. */
    cmdtm_t cmdtm;
    res = sdhci_cmd_get_cmdtm(
            sdhci_cmd,
            &cmdtm
    );
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to get cmdtm in sdhci_send_cmd().");
    }
    /* Set the command register to the value obtained from `sdhci_cmd`. */
    res = bcm_emmc_regs_set_cmdtm(
            bcm_emmc_regs,
            cmdtm
    );
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to set cmdtm in sdhci_send_cmd().");
    }
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

    /* Wait until command complete interrupt */
    res = sdhci_wait_for_interrupt(
            bcm_emmc_regs,
            INT_CMD_DONE,
            sdhci_result
    );
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to wait for command complete interrupt in sdhci_send_cmd().");
    }

    /* Get the response from `resp0`. */
    uint32_t resp0;
    res = bcm_emmc_regs_get_resp0(
            bcm_emmc_regs,
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
    /* Handle response depending on the SDHCI command's response type. */
    switch (cmd_rspns_type) {
        case CMD_NO_RESP: {
            *sdhci_result = SD_OK;
            return result_ok();
        }
        case CMD_BUSY48BIT_RESP: {
            /* `IX_CARD_SELECT` will enter here. */
            res = sdcard_set_status(sdcard, resp0);
            if (result_is_err(res)) {
                return result_err_chain(res, "Failed to set status in sdhci_send_cmd().");
            }
            *sdhci_result = resp0 & R1_ERRORS_MASK;
            if (*sdhci_result != 0) {
                return result_err("Response from SD card indicates error in `CMD_BUSY48BIT_RESP` case in sdhci_send_cmd().");
            }
            return result_ok();
        }
        case CMD_48BIT_RESP: {
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
                    /* This is the switch-case for `IX_SEND_REL_ADDR`. RESP0
                     * contains the RCA. */
                    /* RESP0 contains RCA and status bits 23,22,19,12:0 */
                    uint32_t rca = resp0 & 0xffff0000; // RCA[31:16] of response
                    uint32_t status = ((resp0 & 0x00001fff)) |       // 12:0 map directly to status 12:0
                                      ((resp0 & 0x00002000) << 6) |  // 13 maps to status 19 ERROR
                                      ((resp0 & 0x00004000) << 8) |  // 14 maps to status 22 ILLEGAL_COMMAND
                                      ((resp0 & 0x00008000) << 8);
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
                    *sdhci_result = status & R1_ERRORS_MASK;
                    if (*sdhci_result != 0) {
                        return result_err("Response from SD card indicates error in sdhci_send_cmd().");
                    }
                    return result_ok();
                }
                case 0x08: {
                    /* This is the switch-case for `IX_SEND_IF_COND`. RESP0 contains
                     * voltage acceptance and check pattern, which should match
                     * the argument. */
                    if (resp0 == arg) {
                        *sdhci_result = SD_OK;
                        return result_ok();
                    } else {
                        *sdhci_result = SD_ERROR;
                        return result_err("Response from SD card does not match argument in sdhci_send_cmd().");
                    }
                }
                case 0x29: {
                    /* Response handling for `IX_APP_SEND_OP_COND`. Save the
                     * RESP0 register as the Operation Conditions Register (OCR)
                     * for the `sdcard`. */
                    res = sdcard_set_ocr_raw32(sdcard, resp0);
                    if (result_is_err(res)) {
                        return result_err_chain(res, "Failed to set OCR in sdhci_send_cmd().");
                    }
                    *sdhci_result = SD_OK;
                    return result_ok();
                }
                default: {
                    /* Save the response as the SD card's status. */
                    res = sdcard_set_status(sdcard, resp0);
                    if (result_is_err(res)) {
                        return result_err_chain(res, "Failed to set status in sdhci_send_cmd().");
                    }
                    *sdhci_result = resp0 & R1_ERRORS_MASK;
                    if (*sdhci_result != 0) {
                        return result_err("Response from SD card indicates error in sdhci_send_cmd().");
                    }
                    return result_ok();
                }
            }
        }
        case CMD_136BIT_RESP: {
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
                /* TODO: CSD Stuff here. */
                *sdhci_result = SD_OK;
                return result_ok();
            } else {
                /* `IX_ALL_SEND_CID` will enter this branch. */
                /* Get the response from `resp1`. */
                uint32_t resp1 = 0;
                res = bcm_emmc_regs_get_resp1(
                        bcm_emmc_regs,
                        &resp1
                );
                if (result_is_err(res)) {
                    return result_err_chain(res, "Failed to get resp1 in sdhci_send_cmd().");
                }
                /* Get the response from `resp2`. */
                uint32_t resp2 = 0;
                res = bcm_emmc_regs_get_resp2(
                        bcm_emmc_regs,
                        &resp2
                );
                if (result_is_err(res)) {
                    return result_err_chain(res, "Failed to get resp2 in sdhci_send_cmd().");
                }
                /* Get the response from `resp3`. */
                uint32_t resp3 = 0;
                res = bcm_emmc_regs_get_resp3(
                        bcm_emmc_regs,
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




