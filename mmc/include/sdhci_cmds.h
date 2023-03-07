#pragma once

#include "sdhci_cmd.h"
#include "result.h"

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
 * Returns True if the `sdhci_cmd_index` is an App Command and False otherwise.
 * @param cmd_index
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
