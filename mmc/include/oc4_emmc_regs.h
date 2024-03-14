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
#include "arith.h"
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

typedef volatile struct oc4_emmc_regs oc4_emmc_regs_t;

/**
 * A descriptor struct that stores key information about transfers between SD Card and SD Controller
 */
struct __attribute__((__packed__, aligned(4))) sd_emmc_desc {
    uint32_t cmd_cfg; /* Command Config */
    uint32_t cmd_arg; /* Command Arguments */
    uint32_t cmd_data; /* Command Data */
    uint32_t cmd_resp; /* Command Response */
};

// /**
//  * A struct that stores all the normal registers of the Odroid C4
//  */
// struct __attribute__((__packed__, aligned(4))) oc4_emmc_regs {

// 	uint32_t sd_emmc_clock;		    // 0x00 SD_EMMC_CLOCK : // TODO: make own type later
// 	uint32_t sd_emmc_clock2;		    // 0x00 SD_EMMC_CLOCK : // TODO: make own type later
// 	uint32_t sd_emmc_clock3;		    // 0x00 SD_EMMC_CLOCK : // TODO: make own type later
// 	uint32_t sd_emmc_clock4;		    // 0x00 SD_EMMC_CLOCK : // TODO: make own type later
// 	uint32_t sd_emmc_delay1;		// 0x04 DELAY1 = 0xffe05010 FINAL
// 	uint32_t sd_emmc_delay12;		// 0x04 DELAY1
// 	uint32_t sd_emmc_delay13;		// 0x04 DELAY1
// 	uint32_t sd_emmc_delay14;		// 0x04 DELAY1
// 	uint32_t sd_emmc_delay2;		// 0x08 DELAY1 = 0xffe05020 FINAL
// 	uint32_t sd_emmc_delay22;		// 0x08 DELAY1
// 	uint32_t sd_emmc_delay23;		// 0x08 DELAY1
// 	uint32_t sd_emmc_delay24;		// 0x08 DELAY1
// 	uint32_t sd_emmc_adjust;		// 0x0c DELAY1
// 	uint32_t sd_emmc_adjust2;		// 0x0c DELAY1
// 	uint32_t sd_emmc_adjust3;		// 0x0c DELAY1
// 	uint32_t sd_emmc_adjust4;		// 0x0c DELAY1
// 	uint32_t sd_emmc_calout;		// 0x10 DELAY1
// 	uint32_t sd_emmc_calout2;		// 0x10 DELAY1
// 	uint32_t sd_emmc_calout3;		// 0x10 DELAY1
// 	uint32_t sd_emmc_calout4;		// 0x10 DELAY1
// 	uint32_t undocumented0;		    // 0x14 EMPTY
// 	uint32_t undocumented02;		    // 0x14 EMPTY
// 	uint32_t undocumented03;		    // 0x14 EMPTY
// 	uint32_t undocumented04;		    // 0x14 EMPTY
// 	uint32_t undocumented1;		    // 0x18 EMPTY
// 	uint32_t undocumented12;		    // 0x18 EMPTY
// 	uint32_t undocumented13;		    // 0x18 EMPTY
// 	uint32_t undocumented14;		    // 0x18 EMPTY
// 	uint32_t undocumented2;		    // 0x1c EMPTY
// 	uint32_t undocumented22;		    // 0x1c EMPTY
// 	uint32_t undocumented23;		    // 0x1c EMPTY
// 	uint32_t undocumented24;		    // 0x1c EMPTY
// 	uint32_t sd_emmc_adj_idx_log;   // 0x20 ADJ IDX LOG
// 	uint32_t sd_emmc_adj_idx_log2;   // 0x20 ADJ IDX LOG
// 	uint32_t sd_emmc_adj_idx_log3;   // 0x20 ADJ IDX LOG
// 	uint32_t sd_emmc_adj_idx_log4;   // 0x20 ADJ IDX LOG
// 	uint32_t sd_emmc_clk_test_log;	// 0x24 CLK TEST LOG
// 	uint32_t sd_emmc_clk_test_log2;	// 0x24 CLK TEST LOG
// 	uint32_t sd_emmc_clk_test_log3;	// 0x24 CLK TEST LOG
// 	uint32_t sd_emmc_clk_test_log4;	// 0x24 CLK TEST LOG
// 	uint32_t sd_emmc_clk_test_out;	// 0x28 CLK TEST OUT
// 	uint32_t sd_emmc_clk_test_out2;	// 0x28 CLK TEST OUT
// 	uint32_t sd_emmc_clk_test_out3;	// 0x28 CLK TEST OUT
// 	uint32_t sd_emmc_clk_test_out4;	// 0x28 CLK TEST OUT
// 	uint32_t sd_emmc_eyetest_log;	// 0x2c EYETEST LOG
// 	uint32_t sd_emmc_eyetest_log2;	// 0x2c EYETEST LOG
// 	uint32_t sd_emmc_eyetest_log3;	// 0x2c EYETEST LOG
// 	uint32_t sd_emmc_eyetest_log4;	// 0x2c EYETEST LOG
// 	uint32_t sd_emmc_eyetest_out0;	// 0x30 EYETEST OUT0
// 	uint32_t sd_emmc_eyetest_out02;	// 0x30 EYETEST OUT0
// 	uint32_t sd_emmc_eyetest_out03;	// 0x30 EYETEST OUT0
// 	uint32_t sd_emmc_eyetest_out04;	// 0x30 EYETEST OUT0
// 	uint32_t sd_emmc_eyetest_out1;	// 0x34 EYETEST OUT0
// 	uint32_t sd_emmc_eyetest_out12;	// 0x34 EYETEST OUT0
// 	uint32_t sd_emmc_eyetest_out13;	// 0x34 EYETEST OUT0
// 	uint32_t sd_emmc_eyetest_out14;	// 0x34 EYETEST OUT0
// 	uint32_t sd_emmc_intf3;	        // 0x38 INTF3
// 	uint32_t sd_emmc_intf32;	        // 0x38 INTF3
// 	uint32_t sd_emmc_intf33;	        // 0x38 INTF3
// 	uint32_t sd_emmc_intf34;	        // 0x38 INTF3
// 	uint32_t undocumented3;	        // 0x3c EMPTY
// 	uint32_t undocumented32;	        // 0x3c EMPTY
// 	uint32_t undocumented33;	        // 0x3c EMPTY
// 	uint32_t undocumented34;	        // 0x3c EMPTY
// 	uint32_t sd_emmc_start;	        // 0x40 START
// 	uint32_t sd_emmc_start2;	        // 0x40 START
// 	uint32_t sd_emmc_start3;	        // 0x40 START
// 	uint32_t sd_emmc_start4;	        // 0x40 START
// 	uint32_t sd_emmc_cfg;	        // 0x44 CFG
// 	uint32_t sd_emmc_cfg2;	        // 0x44 CFG
// 	uint32_t sd_emmc_cfg3;	        // 0x44 CFG
// 	uint32_t sd_emmc_cfg4;	        // 0x44 CFG
// 	uint32_t sd_emmc_status;	    // 0x48 STATUS
// 	uint32_t sd_emmc_status2;	    // 0x48 STATUS
// 	uint32_t sd_emmc_status3;	    // 0x48 STATUS
// 	uint32_t sd_emmc_status4;	    // 0x48 STATUS
// 	uint32_t sd_emmc_irq_en;	    // 0x4c IRQ EN
// 	uint32_t sd_emmc_irq_en2;	    // 0x4c IRQ EN
// 	uint32_t sd_emmc_irq_en3;	    // 0x4c IRQ EN
// 	uint32_t sd_emmc_irq_en4;	    // 0x4c IRQ EN
// 	uint32_t sd_emmc_cmd_cfg;	    // 0x50 COMMAND CFG
// 	uint32_t sd_emmc_cmd_cfg2;	    // 0x50 COMMAND CFG
// 	uint32_t sd_emmc_cmd_cfg3;	    // 0x50 COMMAND CFG
// 	uint32_t sd_emmc_cmd_cfg4;	    // 0x50 COMMAND CFG
// 	uint32_t sd_emmc_cmd_arg;	    // 0x54 COMMAND ARG
// 	uint32_t sd_emmc_cmd_arg2;	    // 0x54 COMMAND ARG
// 	uint32_t sd_emmc_cmd_arg3;	    // 0x54 COMMAND ARG
// 	uint32_t sd_emmc_cmd_arg4;	    // 0x54 COMMAND ARG
// 	uint32_t sd_emmc_cmd_dat;	    // 0x58 COMMAND DAT
// 	uint32_t sd_emmc_cmd_dat2;	    // 0x58 COMMAND DAT
// 	uint32_t sd_emmc_cmd_dat3;	    // 0x58 COMMAND DAT
// 	uint32_t sd_emmc_cmd_dat4;	    // 0x58 COMMAND DAT
// 	uint32_t sd_emmc_cmd_rsp;	    // 0x5c COMMAND RSP
// 	uint32_t sd_emmc_cmd_rsp2;	    // 0x5c COMMAND RSP
// 	uint32_t sd_emmc_cmd_rsp3;	    // 0x5c COMMAND RSP
// 	uint32_t sd_emmc_cmd_rsp4;	    // 0x5c COMMAND RSP
// 	uint32_t sd_emmc_cmd_rsp1;	    // 0x60 COMMAND RSP1
// 	uint32_t sd_emmc_cmd_rsp12;	    // 0x60 COMMAND RSP1
// 	uint32_t sd_emmc_cmd_rsp13;	    // 0x60 COMMAND RSP1
// 	uint32_t sd_emmc_cmd_rsp14;	    // 0x60 COMMAND RSP1
// 	uint32_t sd_emmc_cmd_rsp21;	    // 0x64 COMMAND RSP2
// 	uint32_t sd_emmc_cmd_rsp22;	    // 0x64 COMMAND RSP2
// 	uint32_t sd_emmc_cmd_rsp23;	    // 0x64 COMMAND RSP2
// 	uint32_t sd_emmc_cmd_rsp24;	    // 0x64 COMMAND RSP2
// 	uint32_t sd_emmc_cmd_rsp31;	    // 0x68 COMMAND RSP3
// 	uint32_t sd_emmc_cmd_rsp32;	    // 0x68 COMMAND RSP3
// 	uint32_t sd_emmc_cmd_rsp33;	    // 0x68 COMMAND RSP3
// 	uint32_t sd_emmc_cmd_rsp34;	    // 0x68 COMMAND RSP3
// 	uint32_t undocumented4;	        // 0x6c RESERVED
// 	uint32_t undocumented42;	        // 0x6c RESERVED
// 	uint32_t undocumented43;	        // 0x6c RESERVED
// 	uint32_t undocumented44;	        // 0x6c RESERVED
// 	uint32_t sd_emmc_curr_cfg;	    // 0x70 CURR CFG
// 	uint32_t sd_emmc_curr_cfg2;	    // 0x70 CURR CFG
// 	uint32_t sd_emmc_curr_cfg3;	    // 0x70 CURR CFG
// 	uint32_t sd_emmc_curr_cfg4;	    // 0x70 CURR CFG
// 	uint32_t sd_emmc_curr_arg;	    // 0x74 CURR ARG
// 	uint32_t sd_emmc_curr_arg2;	    // 0x74 CURR ARG
// 	uint32_t sd_emmc_curr_arg3;	    // 0x74 CURR ARG
// 	uint32_t sd_emmc_curr_arg4;	    // 0x74 CURR ARG
// 	uint32_t sd_emmc_curr_dat;	    // 0x78 CURR DAT
// 	uint32_t sd_emmc_curr_dat2;	    // 0x78 CURR DAT
// 	uint32_t sd_emmc_curr_dat3;	    // 0x78 CURR DAT
// 	uint32_t sd_emmc_curr_dat4;	    // 0x78 CURR DAT
// 	uint32_t sd_emmc_curr_rsp;	    // 0x7c CURR RSP
// 	uint32_t sd_emmc_curr_rsp2;	    // 0x7c CURR RSP
// 	uint32_t sd_emmc_curr_rsp3;	    // 0x7c CURR RSP
// 	uint32_t sd_emmc_curr_rsp4;	    // 0x7c CURR RSP
// 	uint32_t sd_emmc_next_cfg;	    // 0x80 NEXT CFG
// 	uint32_t sd_emmc_next_cfg2;	    // 0x80 NEXT CFG
// 	uint32_t sd_emmc_next_cfg3;	    // 0x80 NEXT CFG
// 	uint32_t sd_emmc_next_cfg4;	    // 0x80 NEXT CFG
// 	uint32_t sd_emmc_next_arg;	    // 0x84 NEXT ARG
// 	uint32_t sd_emmc_next_arg2;	    // 0x84 NEXT ARG
// 	uint32_t sd_emmc_next_arg3;	    // 0x84 NEXT ARG
// 	uint32_t sd_emmc_next_arg4;	    // 0x84 NEXT ARG
// 	uint32_t sd_emmc_next_dat;	    // 0x88 NEXT DAT
// 	uint32_t sd_emmc_next_dat2;	    // 0x88 NEXT DAT
// 	uint32_t sd_emmc_next_dat3;	    // 0x88 NEXT DAT
// 	uint32_t sd_emmc_next_dat4;	    // 0x88 NEXT DAT
// 	uint32_t sd_emmc_next_rsp;	    // 0x8c NEXT RSP
// 	uint32_t sd_emmc_next_rsp2;	    // 0x8c NEXT RSP
// 	uint32_t sd_emmc_next_rsp3;	    // 0x8c NEXT RSP
// 	uint32_t sd_emmc_next_rsp4;	    // 0x8c NEXT RSP
// 	uint32_t sd_emmc_rxd;	        // 0x90 RXD
// 	uint32_t sd_emmc_rxd2;	        // 0x90 RXD
// 	uint32_t sd_emmc_rxd3;	        // 0x90 RXD
// 	uint32_t sd_emmc_rxd4;	        // 0x90 RXD
// 	uint32_t sd_emmc_txd;	        // 0x94 TXD
// 	uint32_t sd_emmc_txd2;	        // 0x94 TXD
// 	uint32_t sd_emmc_txd3;	        // 0x94 TXD
// 	uint32_t sd_emmc_txd4;	        // 0x94 TXD}; = 0x250 FINAL
// };


/**
 * A struct that stores all the normal registers of the Odroid C4
 */
struct __attribute__((__packed__, aligned(4))) oc4_emmc_regs {

	uint32_t sd_emmc_clock;		    // 0x00 SD_EMMC_CLOCK : // TODO: make own type later
	uint32_t sd_emmc_delay1;		// 0x04 DELAY1 = 0xffe05010 FINAL
	uint32_t sd_emmc_delay2;		// 0x08 DELAY1 = 0xffe05020 FINAL
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
	uint32_t sd_emmc_eyetest_out1;	// 0x34 EYETEST OUT1
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

/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * (C) Copyright 2016 Carlo Caione <carlo@caione.org>
 */

enum meson_gx_mmc_compatible {
	MMC_COMPATIBLE_GX,
	MMC_COMPATIBLE_SM1,
};

#define SDIO_PORT_A			        0
#define SDIO_PORT_B			        1
#define SDIO_PORT_C			        2

#define SD_EMMC_CLKSRC_24M		    24000000	/* 24 MHz */
#define SD_EMMC_CLKSRC_DIV2		    1000000000	/* 1 GHz */

#define SD_EMMC_CLOCK		        0x00
#define   CLK_MAX_DIV			    63
#define   CLK_SRC_24M			    (0 << 6)
#define   CLK_SRC_DIV2			    (1 << 6)
#define   CLK_CO_PHASE_000		    (0 << 8)
#define   CLK_CO_PHASE_090		    (1 << 8)
#define   CLK_CO_PHASE_180		    (2 << 8)
#define   CLK_CO_PHASE_270		    (3 << 8)
#define   CLK_TX_PHASE_000		    (0 << 10)
#define   CLK_TX_PHASE_090		    (1 << 10)
#define   CLK_TX_PHASE_180		    (2 << 10)
#define   CLK_TX_PHASE_270		    (3 << 10)
#define   CLK_RX_PHASE_000		    (0 << 12)
#define   CLK_RX_PHASE_090		    (1 << 12)
#define   CLK_RX_PHASE_180		    (2 << 12)
#define   CLK_RX_PHASE_270		    (3 << 12)
#define   CLK_ALWAYS_ON			    BIT(28) /* THIS LINE SEEMS TO BE WRONG? Should it not be BIT(28) ???? was BIT(24) before */

#define SD_EMMC_DELAY1		        0x04
#define SD_EMMC_ADJUST		        0x0c
#define SD_EMMC_START		        0x40

#define SD_EMMC_CFG		            0x44
#define   CFG_BUS_WIDTH_MASK	    GENMASK_UNSAFE(1, 0) /* Actually 2 bits */
#define   CFG_BUS_WIDTH_1		    0
#define   CFG_BUS_WIDTH_4		    1
#define   CFG_BUS_WIDTH_8		    2
#define   CFG_BL_LEN_MASK		    GENMASK_UNSAFE(7, 4)
#define   CFG_BL_LEN_SHIFT		    4
#define   CFG_BL_LEN_512		    (9 << 4)
#define   CFG_RESP_TIMEOUT_MASK	    GENMASK_UNSAFE(11, 8)
#define   CFG_RESP_TIMEOUT_256	    (8 << 8)

// // TODO: change the 256 to something more reasonable
// #define   CFG_RESP_TIMEOUT_256	    (15 << 8)
#define   CFG_RC_CC_MASK		    GENMASK_UNSAFE(15, 12)
#define   CFG_RC_CC_16			    (4 << 12)
#define   CFG_SDCLK_ALWAYS_ON	    BIT(18)
#define   CFG_AUTO_CLK			    BIT(23)

#define SD_EMMC_STATUS	            0x48
#define   STATUS_MASK			    GENMASK_UNSAFE(15, 0)
#define   STATUS_ERR_MASK		    GENMASK_UNSAFE(12, 0)
#define   STATUS_RXD_ERR_MASK	    GENMASK_UNSAFE(7, 0)
#define   STATUS_TXD_ERR		    BIT(8)
#define   STATUS_DESC_ERR		    BIT(9)
#define   STATUS_RESP_ERR		    BIT(10)
#define   STATUS_RESP_TIMEOUT	    BIT(11)
#define   STATUS_DESC_TIMEOUT	    BIT(12)
#define   STATUS_END_OF_CHAIN	    BIT(13)
#define   STATUS_DESC_BUSY	    	BIT(30)
#define   STATUS_CORE_BUSY	    	BIT(31)

#define SD_EMMC_IRQ_EN	            0x4c

#define SD_EMMC_CMD_CFG	            0x50
#define   CMD_CFG_LENGTH_MASK	    GENMASK_UNSAFE(8, 0)
#define   CMD_CFG_BLOCK_MODE	    BIT(9)
#define   CMD_CFG_R1B			    BIT(10)
#define   CMD_CFG_END_OF_CHAIN	    BIT(11)
#define   CMD_CFG_TIMEOUT_4S	    (12 << 12)
#define   CMD_CFG_TIMEOUT_MASK	    GENMASK_UNSAFE(15, 12)
#define   CMD_CFG_NO_RESP		    BIT(16)
#define   CMD_CFG_DATA_IO		    BIT(18)
#define   CMD_CFG_DATA_WR		    BIT(19)
#define   CMD_CFG_RESP_NOCRC	    BIT(20)
#define   CMD_CFG_RESP_128		    BIT(21)
#define   CMD_CFG_DATA_NUM		    BIT(23)
#define   CMD_CFG_CMD_INDEX			GENMASK_UNSAFE(29, 24)
#define   CMD_CFG_CMD_INDEX_SHIFT	24
#define   CMD_CFG_ERROR			    BIT(30)
#define   CMD_CFG_OWNER			    BIT(31)

#define SD_EMMC_CMD_ARG		        0x54
#define SD_EMMC_CMD_DAT		        0x58
#define SD_EMMC_CMD_RSP		        0x5c
#define SD_EMMC_CMD_RSP1		    0x60
#define SD_EMMC_CMD_RSP2		    0x64
#define SD_EMMC_CMD_RSP3		    0x68

static inline uint32_t div_round_up(uint32_t num, uint32_t den)
{
	return (num + den - 1)/den;
}

// struct meson_mmc_plat {
// 	struct mmc_config cfg;
// 	struct mmc mmc;
// 	void *regbase;
// 	void *w_buf;
// };

result_t oc4_emmc_regs_zero_control0(oc4_emmc_regs_t *oc4_emmc_regs);
result_t oc4_emmc_regs_zero_control1(oc4_emmc_regs_t *oc4_emmc_regs);
result_t oc4_emmc_regs_reset_host_circuit(oc4_emmc_regs_t *oc4_emmc_regs);
result_t oc4_emmc_regs_is_host_circuit_reset(oc4_emmc_regs_t *oc4_emmc_regs, bool *ret_val);
result_t oc4_emmc_regs_enable_internal_clock(oc4_emmc_regs_t *oc4_emmc_regs);
result_t oc4_emmc_regs_is_data_lines_busy(oc4_emmc_regs_t *oc4_emmc_regs, bool *ret_val);
result_t oc4_emmc_regs_is_cmd_line_busy(oc4_emmc_regs_t *oc4_emmc_regs, bool *ret_val);
result_t oc4_emmc_regs_disable_sd_clock(oc4_emmc_regs_t *oc4_emmc_regs);
result_t oc4_emmc_regs_enable_sd_clock(oc4_emmc_regs_t *oc4_emmc_regs);
result_t oc4_emmc_regs_get_host_controller_spec_version(oc4_emmc_regs_t *oc4_emmc_regs, uint8_t *ret_val);
result_t oc4_emmc_regs_set_sd_clock_mode_as_divided(oc4_emmc_regs_t *oc4_emmc_regs);
result_t oc4_emmc_regs_set_sd_clock_divisor(oc4_emmc_regs_t *oc4_emmc_regs, uint16_t divisor);
result_t oc4_emmc_regs_is_sd_clock_stable(oc4_emmc_regs_t *oc4_emmc_regs, bool *ret_val);
result_t oc4_emmc_regs_is_cmd_in_progress(oc4_emmc_regs_t *oc4_emmc_regs, bool *ret_val);
result_t oc4_emmc_regs_set_cmdtm(oc4_emmc_regs_t *oc4_emmc_regs, cmdtm_t val);
result_t oc4_emmc_regs_set_block_size(oc4_emmc_regs_t * oc4_emmc_regs, uint32_t val);
result_t oc4_emmc_regs_set_block_count(oc4_emmc_regs_t *oc4_emmc_regs, uint32_t val);
result_t oc4_emmc_regs_is_read_ready(oc4_emmc_regs_t *oc4_emmc_regs, bool *ret_val);
result_t oc4_emmc_regs_is_write_ready(oc4_emmc_regs_t *oc4_emmc_regs, bool *ret_val);
result_t oc4_emmc_regs_is_data_in_progress(oc4_emmc_regs_t *oc4_emmc_regs, bool *ret_val);
result_t oc4_emmc_regs_set_max_data_timeout(oc4_emmc_regs_t *oc4_emmc_regs);
result_t oc4_emmc_regs_enable_interrupts(oc4_emmc_regs_t *oc4_emmc_regs);
result_t oc4_emmc_regs_mask_interrupt(oc4_emmc_regs_t *oc4_emmc_regs, uint32_t mask, bool *ret_val);
result_t oc4_emmc_regs_set_interrupt_raw32(oc4_emmc_regs_t *oc4_emmc_regs, uint32_t val);
result_t oc4_emmc_regs_is_data_timeout_err(oc4_emmc_regs_t *oc4_emmc_regs, bool *ret_val);
result_t oc4_emmc_regs_is_any_err(oc4_emmc_regs_t *oc4_emmc_regs, bool *ret_val);
result_t oc4_emmc_regs_clear_interrupt(oc4_emmc_regs_t *oc4_emmc_regs);
result_t oc4_emmc_regs_set_arg1(oc4_emmc_regs_t *oc4_emmc_regs, uint32_t val);
result_t oc4_emmc_regs_get_resp0(oc4_emmc_regs_t *oc4_emmc_regs, uint32_t *ret_val);
result_t oc4_emmc_regs_get_resp1(oc4_emmc_regs_t *oc4_emmc_regs, uint32_t *ret_val);
result_t oc4_emmc_regs_get_resp2(oc4_emmc_regs_t *oc4_emmc_regs, uint32_t *ret_val);
result_t oc4_emmc_regs_get_resp3(oc4_emmc_regs_t *oc4_emmc_regs, uint32_t *ret_val);
result_t oc4_emmc_regs_get_data(oc4_emmc_regs_t *oc4_emmc_regs, uint32_t *ret_val);
result_t oc4_emmc_regs_set_data(oc4_emmc_regs_t *oc4_emmc_regs, uint32_t val);
result_t oc4_emmc_regs_is_cmd_timeout_err(oc4_emmc_regs_t *oc4_emmc_regs, bool *ret_val);
result_t oc4_emmc_regs_get_interrupt_raw32(oc4_emmc_regs_t *oc4_emmc_regs, uint32_t *ret_val);
result_t oc4_emmc_regs_set_bus_width_4(oc4_emmc_regs_t *oc4_emmc_regs);