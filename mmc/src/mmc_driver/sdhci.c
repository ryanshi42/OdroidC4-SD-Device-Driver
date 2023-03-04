#include "sdhci.h"

/*--------------------------------------------------------------------------}
{							  SD CARD COMMAND TABLE						    }
{--------------------------------------------------------------------------*/
EMMCCommand sdCommandTable[IX_SEND_SCR + 1] = {
        [IX_GO_IDLE_STATE] =    {
                "GO_IDLE_STATE", .code.CMD_INDEX = 0x00, .code.CMD_RSPNS_TYPE = CMD_NO_RESP, .use_rca = 0, .delay = 0},
        [IX_ALL_SEND_CID] =        {
                "ALL_SEND_CID", .code.CMD_INDEX = 0x02, .code.CMD_RSPNS_TYPE = CMD_136BIT_RESP, .use_rca = 0, .delay = 0},
        [IX_SEND_REL_ADDR] =    {
                "SEND_REL_ADDR", .code.CMD_INDEX = 0x03, .code.CMD_RSPNS_TYPE = CMD_48BIT_RESP, .use_rca = 0, .delay = 0},
        [IX_SET_DSR] =            {
                "SET_DSR", .code.CMD_INDEX = 0x04, .code.CMD_RSPNS_TYPE = CMD_NO_RESP, .use_rca = 0, .delay = 0},
        [IX_SWITCH_FUNC] =        {
                "SWITCH_FUNC", .code.CMD_INDEX = 0x06, .code.CMD_RSPNS_TYPE = CMD_48BIT_RESP, .use_rca = 0, .delay = 0},
        [IX_CARD_SELECT] =        {
                "CARD_SELECT", .code.CMD_INDEX = 0x07, .code.CMD_RSPNS_TYPE = CMD_BUSY48BIT_RESP, .use_rca = 1, .delay = 0},
        [IX_SEND_IF_COND] =    {
                "SEND_IF_COND", .code.CMD_INDEX = 0x08, .code.CMD_RSPNS_TYPE = CMD_48BIT_RESP, .use_rca = 0, .delay = 100},
        [IX_SEND_CSD] =            {
                "SEND_CSD", .code.CMD_INDEX = 0x09, .code.CMD_RSPNS_TYPE = CMD_136BIT_RESP, .use_rca = 1, .delay = 0},
        [IX_SEND_CID] =            {
                "SEND_CID", .code.CMD_INDEX = 0x0A, .code.CMD_RSPNS_TYPE = CMD_136BIT_RESP, .use_rca = 1, .delay = 0},
        [IX_VOLTAGE_SWITCH] =    {
                "VOLT_SWITCH", .code.CMD_INDEX = 0x0B, .code.CMD_RSPNS_TYPE = CMD_48BIT_RESP, .use_rca = 0, .delay = 0},
        [IX_STOP_TRANS] =        {
                "STOP_TRANS", .code.CMD_INDEX = 0x0C, .code.CMD_RSPNS_TYPE = CMD_BUSY48BIT_RESP, .use_rca = 0, .delay = 0},
        [IX_SEND_STATUS] =        {
                "SEND_STATUS", .code.CMD_INDEX = 0x0D, .code.CMD_RSPNS_TYPE = CMD_48BIT_RESP, .use_rca = 1, .delay = 0},
        [IX_GO_INACTIVE] =        {
                "GO_INACTIVE", .code.CMD_INDEX = 0x0F, .code.CMD_RSPNS_TYPE = CMD_NO_RESP, .use_rca = 1, .delay = 0},
        [IX_SET_BLOCKLEN] =        {
                "SET_BLOCKLEN", .code.CMD_INDEX = 0x10, .code.CMD_RSPNS_TYPE = CMD_48BIT_RESP, .use_rca = 0, .delay = 0},
        [IX_READ_SINGLE] =        {"READ_SINGLE", .code.CMD_INDEX = 0x11, .code.CMD_RSPNS_TYPE = CMD_48BIT_RESP,
                .code.CMD_ISDATA = 1, .code.TM_DAT_DIR = 1, .use_rca = 0, .delay = 0},
        [IX_READ_MULTI] =        {"READ_MULTI", .code.CMD_INDEX = 0x12, .code.CMD_RSPNS_TYPE = CMD_48BIT_RESP,
                .code.CMD_ISDATA = 1, .code.TM_DAT_DIR = 1,
                .code.TM_BLKCNT_EN =1, .code.TM_MULTI_BLOCK = 1, .use_rca = 0, .delay = 0},
        [IX_SEND_TUNING] =        {
                "SEND_TUNING", .code.CMD_INDEX = 0x13, .code.CMD_RSPNS_TYPE = CMD_48BIT_RESP, .use_rca = 0, .delay = 0},
        [IX_SPEED_CLASS] =        {
                "SPEED_CLASS", .code.CMD_INDEX = 0x14, .code.CMD_RSPNS_TYPE = CMD_BUSY48BIT_RESP, .use_rca = 0, .delay = 0},
        [IX_SET_BLOCKCNT] =        {
                "SET_BLOCKCNT", .code.CMD_INDEX = 0x17, .code.CMD_RSPNS_TYPE = CMD_48BIT_RESP, .use_rca = 0, .delay = 0},
        [IX_WRITE_SINGLE] =        {"WRITE_SINGLE", .code.CMD_INDEX = 0x18, .code.CMD_RSPNS_TYPE = CMD_48BIT_RESP,
                .code.CMD_ISDATA = 1, .use_rca = 0, .delay = 0},
        [IX_WRITE_MULTI] =        {"WRITE_MULTI", .code.CMD_INDEX = 0x19, .code.CMD_RSPNS_TYPE = CMD_48BIT_RESP,
                .code.CMD_ISDATA = 1,
                .code.TM_BLKCNT_EN = 1, .code.TM_MULTI_BLOCK = 1, .use_rca = 0, .delay = 0},
        [IX_PROGRAM_CSD] =        {
                "PROGRAM_CSD", .code.CMD_INDEX = 0x1B, .code.CMD_RSPNS_TYPE = CMD_48BIT_RESP, .use_rca = 0, .delay = 0},
        [IX_SET_WRITE_PR] =        {
                "SET_WRITE_PR", .code.CMD_INDEX = 0x1C, .code.CMD_RSPNS_TYPE = CMD_BUSY48BIT_RESP, .use_rca = 0, .delay = 0},
        [IX_CLR_WRITE_PR] =        {
                "CLR_WRITE_PR", .code.CMD_INDEX = 0x1D, .code.CMD_RSPNS_TYPE = CMD_BUSY48BIT_RESP, .use_rca = 0, .delay = 0},
        [IX_SND_WRITE_PR] =        {
                "SND_WRITE_PR", .code.CMD_INDEX = 0x1E, .code.CMD_RSPNS_TYPE = CMD_48BIT_RESP, .use_rca = 0, .delay = 0},
        [IX_ERASE_WR_ST] =        {
                "ERASE_WR_ST", .code.CMD_INDEX = 0x20, .code.CMD_RSPNS_TYPE = CMD_48BIT_RESP, .use_rca = 0, .delay = 0},
        [IX_ERASE_WR_END] =        {
                "ERASE_WR_END", .code.CMD_INDEX = 0x21, .code.CMD_RSPNS_TYPE = CMD_48BIT_RESP, .use_rca = 0, .delay = 0},
        [IX_ERASE] =            {
                "ERASE", .code.CMD_INDEX = 0x26, .code.CMD_RSPNS_TYPE = CMD_BUSY48BIT_RESP, .use_rca = 0, .delay = 0},
        [IX_LOCK_UNLOCK] =        {
                "LOCK_UNLOCK", .code.CMD_INDEX = 0x2A, .code.CMD_RSPNS_TYPE = CMD_48BIT_RESP, .use_rca = 0, .delay = 0},
        [IX_APP_CMD] =            {
                "APP_CMD", .code.CMD_INDEX = 0x37, .code.CMD_RSPNS_TYPE = CMD_NO_RESP, .use_rca = 0, .delay = 100},
        [IX_APP_CMD_RCA] =        {
                "APP_CMD", .code.CMD_INDEX = 0x37, .code.CMD_RSPNS_TYPE = CMD_48BIT_RESP, .use_rca = 1, .delay = 0},
        [IX_GEN_CMD] =            {
                "GEN_CMD", .code.CMD_INDEX = 0x38, .code.CMD_RSPNS_TYPE = CMD_48BIT_RESP, .use_rca = 0, .delay = 0},

        // APP commands must be prefixed by an APP_CMD.
        [IX_SET_BUS_WIDTH] =    {
                "SET_BUS_WIDTH", .code.CMD_INDEX = 0x06, .code.CMD_RSPNS_TYPE = CMD_48BIT_RESP, .use_rca = 0, .delay = 0},
        [IX_SD_STATUS] =        {
                "SD_STATUS", .code.CMD_INDEX = 0x0D, .code.CMD_RSPNS_TYPE = CMD_48BIT_RESP, .use_rca = 1, .delay = 0},
        [IX_SEND_NUM_WRBL] =    {
                "SEND_NUM_WRBL", .code.CMD_INDEX = 0x16, .code.CMD_RSPNS_TYPE = CMD_48BIT_RESP, .use_rca = 0, .delay = 0},
        [IX_SEND_NUM_ERS] =        {
                "SEND_NUM_ERS", .code.CMD_INDEX = 0x17, .code.CMD_RSPNS_TYPE = CMD_48BIT_RESP, .use_rca = 0, .delay = 0},
        [IX_APP_SEND_OP_COND] =    {
                "SD_SENDOPCOND", .code.CMD_INDEX = 0x29, .code.CMD_RSPNS_TYPE = CMD_48BIT_RESP, .use_rca = 0, .delay = 1000},
        [IX_SET_CLR_DET] =        {
                "SET_CLR_DET", .code.CMD_INDEX = 0x2A, .code.CMD_RSPNS_TYPE = CMD_48BIT_RESP, .use_rca = 0, .delay = 0},
        [IX_SEND_SCR] =            {"SEND_SCR", .code.CMD_INDEX = 0x33, .code.CMD_RSPNS_TYPE = CMD_48BIT_RESP,
                .code.CMD_ISDATA = 1, .code.TM_DAT_DIR = 1, .use_rca = 0, .delay = 0},
};

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
        bool *has_timed_out
) {
    if (bcm_emmc_regs == NULL) {
        return result_err("NULL `bcm_emmc_regs` passed to sdhci_wait_for_interrupt().");
    }
    uint32_t mask_with_error = interrupt_mask | INT_ERROR_MASK;
    /* Wait for the interrupt. We specify a timeout of 1 second. */
    size_t retries = 1000000;
    bool is_finished_or_error = false;
    do {
        usleep(1);
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
        *has_timed_out = true;
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
    /* Success case. */
    return result_ok();
}
