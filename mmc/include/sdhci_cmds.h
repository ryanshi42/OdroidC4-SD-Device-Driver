#pragma once

#include "sdhci_cmd.h"
#include "result.h"

/*--------------------------------------------------------------------------}
{						SD CARD COMMAND INDEX DEFINITIONS				    }
{--------------------------------------------------------------------------*/

//: These are covered in sdhci_cmds.c
#define IDX_GO_IDLE_STATE    0
#define IDX_ALL_SEND_CID     1
#define IDX_SEND_REL_ADDR    2
#define IDX_SET_DSR          3
#define IDX_SWITCH_FUNC      4
#define IDX_CARD_SELECT      5
#define IDX_SEND_IF_COND     6
#define IDX_SEND_CSD         7
#define IDX_SEND_CID         8
#define IDX_VOLTAGE_SWITCH   9
#define IDX_STOP_TRANS       10
#define IDX_SEND_STATUS      11
#define IDX_GO_INACTIVE      12
#define IDX_SET_BLOCKLEN     13
#define IDX_READ_SINGLE      14
#define IDX_READ_MULTI       15
#define IDX_SEND_TUNING      16
#define IDX_SPEED_CLASS      17
#define IDX_SET_BLOCKCNT     18
#define IDX_WRITE_SINGLE     19
#define IDX_WRITE_MULTI      20
#define IDX_PROGRAM_CSD      21
#define IDX_SET_WRITE_PR     22
#define IDX_CLR_WRITE_PR     23
#define IDX_SND_WRITE_PR     24
#define IDX_ERASE_WR_ST      25
#define IDX_ERASE_WR_END     26
#define IDX_ERASE            27
#define IDX_LOCK_UNLOCK      28
#define IDX_APP_CMD          29
#define IDX_APP_CMD_RCA      30
#define IDX_GEN_CMD          31

// #define IDX_GO_IDLE_STATE       0
// #define IDX_SEND_OP_COND        1
// #define IDX_SWITCH_FUNC         6
// #define IDX_SEND_CSD            7
// #define IDX_SEND_IF_COND        8
// #define IDX_CSD                 9
// #define IDX_CID                 10
// #define IDX_STOP_TRANSMISSION   12
// #define IDX_SEND_STATUS         13
// #define IDX_SET_BLOCKLEN        16
// #define IDX_READ_SINGLE         17
// #define IDX_READ_MULTI          18
// #define IDX_SEND_TUNING         16
// #define IDX_WRITE_SINGLE        24
// #define IDX_WRITE_MULTI         25
// #define IDX_PROGRAM_CSD         27
// #define IDX_SET_WRITE_PR        28
// #define IDX_CLR_WRITE_PR        29
// #define IDX_SND_WRITE_PR        30
// #define IDX_ERASE_WR_ST         32
// #define IDX_ERASE_WR_END        33
// #define IDX_ERASE               38

// Commands hereafter require APP_CMD.
#define IDX_APP_CMD_START    32
#define IDX_SET_BUS_WIDTH    32
#define IDX_SD_STATUS        33
#define IDX_SEND_NUM_WRBL    34
#define IDX_SEND_NUM_ERS     35
#define IDX_APP_SEND_OP_COND 36
#define IDX_SET_CLR_DET      37
#define IDX_SEND_SCR         38

/**
 * CMD 41 BIT SELECTIONS
 */
#define ACMD41_HCS           0x40000000
#define ACMD41_SDXC_POWER    0x10000000
#define ACMD41_S18R          0x04000000
#define ACMD41_VOLTAGE       0x00ff8000
/* PI DOES NOT SUPPORT VOLTAGE SWITCH */
//! Careful. On the Odroid, DO NOT comment this out or change this.
#define ACMD41_ARG_HC        (ACMD41_HCS|ACMD41_SDXC_POWER|ACMD41_VOLTAGE|ACMD41_S18R)
// #define ACMD41_ARG_HC        (ACMD41_HCS|ACMD41_SDXC_POWER|ACMD41_VOLTAGE)//(ACMD41_HCS|ACMD41_SDXC_POWER|ACMD41_VOLTAGE|ACMD41_S18R)
#define ACMD41_ARG_SC        (ACMD41_VOLTAGE)   //(ACMD41_VOLTAGE|ACMD41_S18R)

/**
 * Returns True if the `sdhci_cmd_index` is an App Command and False otherwise.
 * @param sdhci_cmd_index
 * @param is_app_cmd
 * @return
 */
result_t sdhci_cmds_is_app_cmd(
        uint32_t sdhci_cmd_index,
        bool *is_app_cmd
);

/**
 * Returns the SDHCI Command for the given `sdhci_cmd_index`.
 * @param sdhci_cmd_index
 * @param ret_val Pointer to the SDHCI command.
 * @return
 */
result_t sdhci_cmds_get_cmd(
        uint32_t sdhci_cmd_index,
        sdhci_cmd_t **ret_val
);
