/**
 * Represents the EMMC Registers as documented in Section 5 of the
 *  ARM Peripherals manual.
 */

#pragma once

#include <stdint.h>

#include "result.h"
#include "control0.h"
#include "control1.h"
#include "status.h"
#include "slotisr_ver.h"
#include "irpt_mask.h"
#include "irpt_en.h"
#include "interrupt.h"
#include "cmdtm.h"
#include "blksizecnt.h"

/**
 * Some Linux-defined constants which may come in later
 */
#define SD_EMMC_CFG_BLK_SIZE 512 /* internal buffer max: 512 bytes */
#define SD_EMMC_CFG_RESP_TIMEOUT 256 /* in clock cycles */
#define SD_EMMC_CMD_TIMEOUT 1024 /* in ms */
#define SD_EMMC_CMD_TIMEOUT_DATA 4096 /* in ms */
#define SD_EMMC_CFG_CMD_GAP 16 /* in clock cycles */
#define SD_EMMC_DESC_BUF_LEN PAGE_SIZE

/**
 * Some Linux-defined constants related to the IRQ EN registers below
 */
#define   IRQ_RXD_ERR_MASK GENMASK_UNSAFE(7, 0)
#define   IRQ_TXD_ERR BIT(8)
#define   IRQ_DESC_ERR BIT(9)
#define   IRQ_RESP_ERR BIT(10)
#define   IRQ_CRC_ERR (IRQ_RXD_ERR_MASK | IRQ_TXD_ERR | IRQ_DESC_ERR | IRQ_RESP_ERR)
#define   IRQ_RESP_TIMEOUT BIT(11)
#define   IRQ_DESC_TIMEOUT BIT(12)
#define   IRQ_TIMEOUTS (IRQ_RESP_TIMEOUT | IRQ_DESC_TIMEOUT)
#define   IRQ_END_OF_CHAIN BIT(13)
#define   IRQ_RESP_STATUS BIT(14)
#define   IRQ_SDIO BIT(15)
#define   IRQ_EN_MASK (IRQ_CRC_ERR | IRQ_TIMEOUTS | IRQ_END_OF_CHAIN)

typedef struct oc4_emmc_regs oc4_emmc_regs_t;

/**
 * A descriptor struct that stores key information about transfers between SD Card and SD Controller
 */
struct __attribute__((__packed__, aligned(4))) sd_emmc_desc {
    uint32_t cmd_cfg; /* Command Config */
    uint32_t cmd_arg; /* Command Arguments */
    uint32_t cmd_data; /* Command Data */
    uint32_t cmd_resp; /* Command Response */
};

struct __attribute__((__packed__, aligned(4))) oc4_emmc_regs {
	uint32_t sd_emmc_clock;		    // 0x00 SD_EMMC_CLOCK : // TODO: make own type later
	uint32_t sd_emmc_delay1;		// 0x04 DELAY1
	uint32_t sd_emmc_delay2;		// 0x08 DELAY1
	uint32_t sd_emmc_adjust;		// 0x0c DELAY1
	uint32_t sd_emmc_calout;		// 0x10 DELAY1
	uint32_t undocumented0;		    // 0x14 EMPTY
	uint32_t undocumented1;		    // 0x18 EMPTY
	uint32_t undocumented2;		    // 0x1c EMPTY
	uint32_t sd_emmc_adj_idx_log;   // 0x20 ADJ IDX LOG
	uint32_t sd_emmc_clk_test_log;	// 0x24 CLK TEST LOG
	uint32_t sd_emmc_clk_test_out;	// 0x28 CLK TEST OUT
	uint32_t sd_emmc_eyetest_log;	// 0x2c EYETEST LOG
	uint32_t sd_emmc_eyetest_out0;	// 0x30 EYETEST OUT0
	uint32_t sd_emmc_eyetest_out1;	// 0x34 EYETEST OUT0
	uint32_t sd_emmc_intf3;	        // 0x38 INTF3
	uint32_t undocumented3;	        // 0x3c EMPTY
	uint32_t sd_emmc_start;	        // 0x40 START
	uint32_t sd_emmc_cfg;	        // 0x44 CFG
	uint32_t sd_emmc_status;	    // 0x48 STATUS
	uint32_t sd_emmc_irq_en;	    // 0x4c IRQ EN
	uint32_t sd_emmc_cmd_cfg;	    // 0x50 COMMAND CFG
	uint32_t sd_emmc_cmd_arg;	    // 0x54 COMMAND ARG
	uint32_t sd_emmc_cmd_dat;	    // 0x58 COMMAND DAT
	uint32_t sd_emmc_cmd_rsp;	    // 0x5c COMMAND RSP
	uint32_t sd_emmc_cmd_rsp1;	    // 0x60 COMMAND RSP1
	uint32_t sd_emmc_cmd_rsp2;	    // 0x64 COMMAND RSP2
	uint32_t sd_emmc_cmd_rsp3;	    // 0x68 COMMAND RSP3
	uint32_t undocumented4;	        // 0x6c RESERVED
	uint32_t sd_emmc_curr_cfg;	    // 0x70 CURR CFG
	uint32_t sd_emmc_curr_arg;	    // 0x74 CURR ARG
	uint32_t sd_emmc_curr_dat;	    // 0x78 CURR DAT
	uint32_t sd_emmc_curr_rsp;	    // 0x7c CURR RSP
    uint32_t sd_emmc_next_cfg;	    // 0x80 NEXT CFG
	uint32_t sd_emmc_next_arg;	    // 0x84 NEXT ARG
	uint32_t sd_emmc_next_dat;	    // 0x88 NEXT DAT
	uint32_t sd_emmc_next_rsp;	    // 0x8c NEXT RSP
	uint32_t sd_emmc_rxd;	        // 0x90 RXD
	uint32_t sd_emmc_txd;	        // 0x94 TXD
};

/**
 * Zeros out the `control0` register.
 * @param oc4_emmc_regs
 */
result_t oc4_emmc_regs_zero_control0(oc4_emmc_regs_t *oc4_emmc_regs);

/**
 * Zeros out the `control1` register.
 * @param oc4_emmc_regs
 */
result_t oc4_emmc_regs_zero_control1(oc4_emmc_regs_t *oc4_emmc_regs);

/**
 * Resets the complete host circuit.
 * @param oc4_emmc_regs
 * @return
 */
result_t oc4_emmc_regs_reset_host_circuit(oc4_emmc_regs_t *oc4_emmc_regs);

/**
 * Checks if the host circuit is in reset.
 * @param oc4_emmc_regs
 * @param ret_val
 * @return
 */
result_t oc4_emmc_regs_is_host_circuit_reset(oc4_emmc_regs_t *oc4_emmc_regs, bool *ret_val);

/**
 * Sets the data timeout to the maximum value.
 * @param oc4_emmc_regs
 * @return
 */
result_t oc4_emmc_regs_set_max_data_timeout(oc4_emmc_regs_t *oc4_emmc_regs);

/**
 * Enables the internal clock.
 * @param oc4_emmc_regs
 * @return
 */
result_t oc4_emmc_regs_enable_internal_clock(oc4_emmc_regs_t *oc4_emmc_regs);

/**
 * Returns True if Data Lines are in use and False otherwise.
 * @param oc4_emmc_regs
 * @param ret_val
 * @return
 */
result_t oc4_emmc_regs_is_data_lines_busy(oc4_emmc_regs_t *oc4_emmc_regs, bool *ret_val);

/**
 * Returns True if Command Line is in use and False otherwise.
 * @param oc4_emmc_regs
 * @param ret_val
 * @return
 */
result_t oc4_emmc_regs_is_cmd_line_busy(oc4_emmc_regs_t *oc4_emmc_regs, bool *ret_val);

/**
 * Disables the SD clock.
 * @param oc4_emmc_regs
 * @return
 */
result_t oc4_emmc_regs_disable_sd_clock(oc4_emmc_regs_t *oc4_emmc_regs);

/**
 * Enables the SD clock.
 * @param oc4_emmc_regs
 * @return
 */
result_t oc4_emmc_regs_enable_sd_clock(oc4_emmc_regs_t *oc4_emmc_regs);


/**
 * Returns the Host Controller specification version.
 * @param oc4_emmc_regs
 * @param ret_val
 * @return
 */
result_t oc4_emmc_regs_get_host_controller_spec_version(
        oc4_emmc_regs_t *oc4_emmc_regs,
        uint8_t *ret_val
);

/**
 * Sets the SD clock mode to "Divided".
 * @param oc4_emmc_regs
 * @return
 */
result_t oc4_emmc_regs_set_sd_clock_mode_as_divided(
        oc4_emmc_regs_t *oc4_emmc_regs
);

/**
 * Sets the SD clock's divisor.
 * @param oc4_emmc_regs
 * @param divisor
 * @return
 */
result_t oc4_emmc_regs_set_sd_clock_divisor(
        oc4_emmc_regs_t *oc4_emmc_regs,
        uint16_t divisor
);

/**
 * Returns True if the SD clock is stable and False otherwise.
 * @param oc4_emmc_regs
 * @param ret_val
 * @return
 */
result_t oc4_emmc_regs_is_sd_clock_stable(
        oc4_emmc_regs_t *oc4_emmc_regs,
        bool *ret_val
);

/**
 * Enables interrupts.
 * @param oc4_emmc_regs
 * @return
 */
result_t oc4_emmc_regs_enable_interrupts(
        oc4_emmc_regs_t *oc4_emmc_regs
);

/**
 * Masks interrupts.
 * @param oc4_emmc_regs
 * @param mask The interrupt mask.
 * @param ret_val
 * @return
 */
result_t oc4_emmc_regs_mask_interrupt(
        oc4_emmc_regs_t *oc4_emmc_regs,
        uint32_t mask,
        bool *ret_val
);

/**
 * Retrieves the interrupt as a raw 32-bit value.
 * @param oc4_emmc_regs
 * @param ret_val
 * @return
 */
result_t oc4_emmc_regs_get_interrupt_raw32(
        oc4_emmc_regs_t *oc4_emmc_regs,
        uint32_t *ret_val
);

/**
 * Sets the interrupt as a raw 32-bit value.
 * @param oc4_emmc_regs
 * @param val
 * @return
 */
result_t oc4_emmc_regs_set_interrupt_raw32(
        oc4_emmc_regs_t *oc4_emmc_regs,
        uint32_t val
);

/**
 * Returns true if there is a CMD timeout error and false otherwise.
 * @param oc4_emmc_regs
 * @param ret_val
 * @return
 */
result_t oc4_emmc_regs_is_cmd_timeout_err(
        oc4_emmc_regs_t *oc4_emmc_regs,
        bool *ret_val
);

/**
 * Returns true if there is a DATA timeout error and false otherwise.
 * @param oc4_emmc_regs
 * @param ret_val
 * @return
 */
result_t oc4_emmc_regs_is_data_timeout_err(
        oc4_emmc_regs_t *oc4_emmc_regs,
        bool *ret_val
);

/**
 * Returns true if there is any error and false otherwise.
 * @param oc4_emmc_regs
 * @param ret_val
 * @return
 */
result_t oc4_emmc_regs_is_any_err(
        oc4_emmc_regs_t *oc4_emmc_regs,
        bool *ret_val
);

/**
 * Returns true if there is a CMD in progress and false otherwise.
 * @param oc4_emmc_regs
 * @param ret_val
 * @return
 */
result_t oc4_emmc_regs_is_cmd_in_progress(
        oc4_emmc_regs_t *oc4_emmc_regs,
        bool *ret_val
);

/**
 * Returns true if there is a DATA in progress and false otherwise.
 * @param oc4_emmc_regs
 * @param ret_val
 * @return
 */
result_t oc4_emmc_regs_is_data_in_progress(
        oc4_emmc_regs_t *oc4_emmc_regs,
        bool *ret_val
);

/**
 * Clears interrupt flags.
 * @param oc4_emmc_regs
 * @return
 */
result_t oc4_emmc_regs_clear_interrupt(
        oc4_emmc_regs_t *oc4_emmc_regs
);

/**
 * Sets the `arg1` register.
 * @param oc4_emmc_regs
 * @param val
 * @return
 */
result_t oc4_emmc_regs_set_arg1(
        oc4_emmc_regs_t *oc4_emmc_regs,
        uint32_t val
);

/**
 * Sets the `cmdtm` register.
 * @param oc4_emmc_regs
 * @param val
 * @return
 */
result_t oc4_emmc_regs_set_cmdtm(
        oc4_emmc_regs_t *oc4_emmc_regs,
        cmdtm_t val
);

/**
 * Gets the `resp0` register.
 * @param oc4_emmc_regs
 * @param ret_val
 * @return
 */
result_t oc4_emmc_regs_get_resp0(
        oc4_emmc_regs_t *oc4_emmc_regs,
        uint32_t *ret_val
);

/**
 * Gets the `resp1` register.
 * @param oc4_emmc_regs
 * @param ret_val
 * @return
 */
result_t oc4_emmc_regs_get_resp1(
        oc4_emmc_regs_t *oc4_emmc_regs,
        uint32_t *ret_val
);

/**
 * Gets the `resp2` register.
 * @param oc4_emmc_regs
 * @param ret_val
 * @return
 */
result_t oc4_emmc_regs_get_resp2(
        oc4_emmc_regs_t *oc4_emmc_regs,
        uint32_t *ret_val
);

/**
 * Gets the `resp3` register.
 * @param oc4_emmc_regs
 * @param ret_val
 * @return
 */
result_t oc4_emmc_regs_get_resp3(
        oc4_emmc_regs_t *oc4_emmc_regs,
        uint32_t *ret_val
);

/**
 * Sets the block size.
 * @param oc4_emmc_regs
 * @param val
 * @return
 */
result_t oc4_emmc_regs_set_block_size(
        oc4_emmc_regs_t *oc4_emmc_regs,
        uint32_t val
);

/**
 * Sets the block count.
 * @param oc4_emmc_regs
 * @param val
 * @return
 */
result_t oc4_emmc_regs_set_block_count(
        oc4_emmc_regs_t *oc4_emmc_regs,
        uint32_t val
);

/**
 * Returns True if new data is available and ready to be read.
 * @param oc4_emmc_regs
 * @param ret_val
 * @return
 */
result_t oc4_emmc_regs_is_read_ready(
        oc4_emmc_regs_t *oc4_emmc_regs,
        bool *ret_val
);

/**
 * Returns True if new data is available and ready to be written.
 * @param oc4_emmc_regs
 * @param ret_val
 * @return
 */
result_t oc4_emmc_regs_is_write_ready(
        oc4_emmc_regs_t *oc4_emmc_regs,
        bool *ret_val
);

/**
 * Returns the data as a raw 32-bit value.
 * @param oc4_emmc_regs
 * @param ret_val
 * @return
 */
result_t oc4_emmc_regs_get_data(
        oc4_emmc_regs_t *oc4_emmc_regs,
        uint32_t *ret_val
);

/**
 * Sets the data as a raw 32-bit value.
 * @param oc4_emmc_regs
 * @param val
 * @return
 */
result_t oc4_emmc_regs_set_data(
        oc4_emmc_regs_t *oc4_emmc_regs,
        uint32_t val
);

/**
 * Enables or disables the use of 4 data lines.
 * @param oc4_emmc_regs
 * @param val
 * @return
 */
result_t oc4_emmc_regs_set_bus_width_4(
        oc4_emmc_regs_t *oc4_emmc_regs,
        bool val
);

