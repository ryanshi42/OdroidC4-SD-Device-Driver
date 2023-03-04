#pragma once

#include "result.h"
#include "bcm_emmc_regs.h"
#include "sleep.h"
#include "log.h"
#include "arith.h"

/*--------------------------------------------------------------------------}
{						   SD CARD COMMAND RECORD						    }
{--------------------------------------------------------------------------*/
typedef struct EMMCCommand {
    const char cmd_name[16];
    cmdtm_t code;
    struct __attribute__((__packed__)) {
        unsigned use_rca: 1;                                        // @0		Command uses rca
        unsigned reserved: 15;                                        // @1-15	Write as zero read as don't care
        uint16_t delay;                                                // @16-31	Delay to apply after command
    };
} EMMCCommand;

/*--------------------------------------------------------------------------}
{						SD CARD COMMAND INDEX DEFINITIONS				    }
{--------------------------------------------------------------------------*/
#define IX_GO_IDLE_STATE    0
#define IX_ALL_SEND_CID     1
#define IX_SEND_REL_ADDR    2
#define IX_SET_DSR          3
#define IX_SWITCH_FUNC      4
#define IX_CARD_SELECT      5
#define IX_SEND_IF_COND     6
#define IX_SEND_CSD         7
#define IX_SEND_CID         8
#define IX_VOLTAGE_SWITCH   9
#define IX_STOP_TRANS       10
#define IX_SEND_STATUS      11
#define IX_GO_INACTIVE      12
#define IX_SET_BLOCKLEN     13
#define IX_READ_SINGLE      14
#define IX_READ_MULTI       15
#define IX_SEND_TUNING      16
#define IX_SPEED_CLASS      17
#define IX_SET_BLOCKCNT     18
#define IX_WRITE_SINGLE     19
#define IX_WRITE_MULTI      20
#define IX_PROGRAM_CSD      21
#define IX_SET_WRITE_PR     22
#define IX_CLR_WRITE_PR     23
#define IX_SND_WRITE_PR     24
#define IX_ERASE_WR_ST      25
#define IX_ERASE_WR_END     26
#define IX_ERASE            27
#define IX_LOCK_UNLOCK      28
#define IX_APP_CMD          29
#define IX_APP_CMD_RCA      30
#define IX_GEN_CMD          31

// Commands hereafter require APP_CMD.
#define IX_APP_CMD_START    32
#define IX_SET_BUS_WIDTH    32
#define IX_SD_STATUS        33
#define IX_SEND_NUM_WRBL    34
#define IX_SEND_NUM_ERS     35
#define IX_APP_SEND_OP_COND 36
#define IX_SET_CLR_DET      37
#define IX_SEND_SCR         38

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
