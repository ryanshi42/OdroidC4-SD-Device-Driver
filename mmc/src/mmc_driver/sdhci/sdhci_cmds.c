#include "sdhci_cmds.h"

/*
 * SD CARD COMMAND TABLE
 */
sdhci_cmd_t sdhci_cmds[IDX_SEND_SCR + 1] = {
        [IDX_GO_IDLE_STATE] = {
                "GO_IDLE_STATE",
                .cmdtm.CMD_INDEX = 0x00,
                .cmdtm.CMD_RSPNS_TYPE = CMD_NO_RESP,
                .use_rca = 0,
                .delay = 0
        },
        [IDX_ALL_SEND_CID] = {
                "ALL_SEND_CID",
                .cmdtm.CMD_INDEX = 0x02,
                .cmdtm.CMD_RSPNS_TYPE = CMD_136BIT_RESP,
                .use_rca = 0,
                .delay = 0
        },
        [IDX_SEND_REL_ADDR] = {
                "SEND_REL_ADDR",
                .cmdtm.CMD_INDEX = 0x03,
                .cmdtm.CMD_RSPNS_TYPE = CMD_48BIT_RESP,
                .use_rca = 0,
                .delay = 0
        },
        [IDX_SET_DSR] = {
                "SET_DSR",
                .cmdtm.CMD_INDEX = 0x04,
                .cmdtm.CMD_RSPNS_TYPE = CMD_NO_RESP,
                .use_rca = 0,
                .delay = 0
        },
        [IDX_SWITCH_FUNC] = {
                "SWITCH_FUNC",
                .cmdtm.CMD_INDEX = 0x06,
                .cmdtm.CMD_RSPNS_TYPE = CMD_48BIT_RESP,
                .use_rca = 0,
                .delay = 0
        },
        [IDX_CARD_SELECT] = {
                "CARD_SELECT",
                .cmdtm.CMD_INDEX = 0x07,
                .cmdtm.CMD_RSPNS_TYPE = CMD_BUSY48BIT_RESP,
                .use_rca = 1,
                .delay = 0
        },
        [IDX_SEND_IF_COND] = {
                "SEND_IF_COND",
                .cmdtm.CMD_INDEX = 0x08,
                .cmdtm.CMD_RSPNS_TYPE = CMD_48BIT_RESP,
                .use_rca = 0,
                .delay = 100
        },
        [IDX_SEND_CSD] = {
                "SEND_CSD",
                .cmdtm.CMD_INDEX = 0x09,
                .cmdtm.CMD_RSPNS_TYPE = CMD_136BIT_RESP,
                .use_rca = 1,
                .delay = 0
        },
        [IDX_SEND_CID] = {
                "SEND_CID",
                .cmdtm.CMD_INDEX = 0x0A,
                .cmdtm.CMD_RSPNS_TYPE = CMD_136BIT_RESP,
                .use_rca = 1,
                .delay = 0
        },
        [IDX_VOLTAGE_SWITCH] = {
                "VOLT_SWITCH",
                .cmdtm.CMD_INDEX = 0x0B,
                .cmdtm.CMD_RSPNS_TYPE = CMD_48BIT_RESP,
                .use_rca = 0,
                .delay = 0
        },
        [IDX_STOP_TRANS] = {
                "STOP_TRANS",
                .cmdtm.CMD_INDEX = 0x0C,
                .cmdtm.CMD_RSPNS_TYPE = CMD_BUSY48BIT_RESP,
                .use_rca = 0,
                .delay = 0
        },
        [IDX_SEND_STATUS] = {
                "SEND_STATUS",
                .cmdtm.CMD_INDEX = 0x0D,
                .cmdtm.CMD_RSPNS_TYPE = CMD_48BIT_RESP,
                .use_rca = 1,
                .delay = 0
        },
        [IDX_GO_INACTIVE] = {
                "GO_INACTIVE",
                .cmdtm.CMD_INDEX = 0x0F,
                .cmdtm.CMD_RSPNS_TYPE = CMD_NO_RESP,
                .use_rca = 1,
                .delay = 0
        },
        [IDX_SET_BLOCKLEN] = {
                "SET_BLOCKLEN",
                .cmdtm.CMD_INDEX = 0x10,
                .cmdtm.CMD_RSPNS_TYPE = CMD_NO_RESP,
                .use_rca = 0,
                .delay = 0
        },
        [IDX_READ_SINGLE] = {
                "READ_SINGLE",
                .cmdtm.CMD_INDEX = 0x11,
                .cmdtm.CMD_RSPNS_TYPE = CMD_48BIT_RESP,
                .cmdtm.CMD_ISDATA = 1,
                .cmdtm.TM_DAT_DIR = 1,
                .use_rca = 0,
                .delay = 0
        },
        [IDX_READ_MULTI] = {
                "READ_MULTI",
                .cmdtm.CMD_INDEX = 0x12,
                .cmdtm.CMD_RSPNS_TYPE = CMD_48BIT_RESP,
                .cmdtm.CMD_ISDATA = 1,
                .cmdtm.TM_DAT_DIR = 1,
                .cmdtm.TM_BLKCNT_EN =1,
                .cmdtm.TM_MULTI_BLOCK = 1,
                .use_rca = 0,
                .delay = 0
        },
        [IDX_SEND_TUNING] = {
                "SEND_TUNING",
                .cmdtm.CMD_INDEX = 0x13,
                .cmdtm.CMD_RSPNS_TYPE = CMD_48BIT_RESP,
                .use_rca = 0,
                .delay = 0
        },
        [IDX_SPEED_CLASS] = {
                "SPEED_CLASS",
                .cmdtm.CMD_INDEX = 0x14,
                .cmdtm.CMD_RSPNS_TYPE = CMD_BUSY48BIT_RESP,
                .use_rca = 0,
                .delay = 0
        },
        [IDX_SET_BLOCKCNT] = {
                "SET_BLOCKCNT",
                .cmdtm.CMD_INDEX = 0x17,
                .cmdtm.CMD_RSPNS_TYPE = CMD_48BIT_RESP,
                .use_rca = 0,
                .delay = 0
        },
        [IDX_WRITE_SINGLE] = {
                "WRITE_SINGLE",
                .cmdtm.CMD_INDEX = 0x18,
                .cmdtm.CMD_RSPNS_TYPE = CMD_48BIT_RESP,
                .cmdtm.CMD_ISDATA = 1,
                .use_rca = 0,
                .delay = 0
        },
        [IDX_WRITE_MULTI] = {
                "WRITE_MULTI",
                .cmdtm.CMD_INDEX = 0x19,
                .cmdtm.CMD_RSPNS_TYPE = CMD_48BIT_RESP,
                .cmdtm.CMD_ISDATA = 1,
                .cmdtm.TM_BLKCNT_EN = 1,
                .cmdtm.TM_MULTI_BLOCK = 1,
                .use_rca = 0,
                .delay = 0
        },
        [IDX_PROGRAM_CSD] = {
                "PROGRAM_CSD",
                .cmdtm.CMD_INDEX = 0x1B,
                .cmdtm.CMD_RSPNS_TYPE = CMD_48BIT_RESP,
                .use_rca = 0,
                .delay = 0
        },
        [IDX_SET_WRITE_PR] = {
                "SET_WRITE_PR",
                .cmdtm.CMD_INDEX = 0x1C,
                .cmdtm.CMD_RSPNS_TYPE = CMD_BUSY48BIT_RESP,
                .use_rca = 0,
                .delay = 0
        },
        [IDX_CLR_WRITE_PR] = {
                "CLR_WRITE_PR",
                .cmdtm.CMD_INDEX = 0x1D,
                .cmdtm.CMD_RSPNS_TYPE = CMD_BUSY48BIT_RESP,
                .use_rca = 0,
                .delay = 0
        },
        [IDX_SND_WRITE_PR] = {
                "SND_WRITE_PR",
                .cmdtm.CMD_INDEX = 0x1E,
                .cmdtm.CMD_RSPNS_TYPE = CMD_48BIT_RESP,
                .use_rca = 0,
                .delay = 0
        },
        [IDX_ERASE_WR_ST] = {
                "ERASE_WR_ST",
                .cmdtm.CMD_INDEX = 0x20,
                .cmdtm.CMD_RSPNS_TYPE = CMD_48BIT_RESP,
                .use_rca = 0,
                .delay = 0
        },
        [IDX_ERASE_WR_END] = {
                "ERASE_WR_END",
                .cmdtm.CMD_INDEX = 0x21,
                .cmdtm.CMD_RSPNS_TYPE = CMD_48BIT_RESP,
                .use_rca = 0,
                .delay = 0
        },
        [IDX_ERASE] = {
                "ERASE",
                .cmdtm.CMD_INDEX = 0x26,
                .cmdtm.CMD_RSPNS_TYPE = CMD_BUSY48BIT_RESP,
                .use_rca = 0,
                .delay = 0
        },
        [IDX_LOCK_UNLOCK] = {
                "LOCK_UNLOCK",
                .cmdtm.CMD_INDEX = 0x2A,
                .cmdtm.CMD_RSPNS_TYPE = CMD_48BIT_RESP,
                .use_rca = 0,
                .delay = 0
        },
        [IDX_APP_CMD] = {
                "APP_CMD",
                .cmdtm.CMD_INDEX = 0x37,
                .cmdtm.CMD_RSPNS_TYPE = CMD_48BIT_RESP,
                .use_rca = 0,
                .delay = 100
        },
        [IDX_APP_CMD_RCA] = {
                "APP_CMD",
                .cmdtm.CMD_INDEX = 0x37,
                .cmdtm.CMD_RSPNS_TYPE = CMD_48BIT_RESP,
                .use_rca = 1,
                .delay = 0
        },
        [IDX_GEN_CMD] = {
                "GEN_CMD",
                .cmdtm.CMD_INDEX = 0x38,
                .cmdtm.CMD_RSPNS_TYPE = CMD_48BIT_RESP,
                .use_rca = 0,
                .delay = 0
        },





        // APP commands must be prefixed by an APP_CMD.
        [IDX_SET_BUS_WIDTH] = {
                "SET_BUS_WIDTH",
                .cmdtm.CMD_INDEX = 0x06,
                .cmdtm.CMD_RSPNS_TYPE = CMD_48BIT_RESP,
                .use_rca = 0,
                .delay = 0
        },
        [IDX_SD_STATUS] = {
                "SD_STATUS",
                .cmdtm.CMD_INDEX = 0x0D,
                .cmdtm.CMD_RSPNS_TYPE = CMD_48BIT_RESP,
                .use_rca = 1,
                .delay = 0
        },
        [IDX_SEND_NUM_WRBL] = {
                "SEND_NUM_WRBL",
                .cmdtm.CMD_INDEX = 0x16,
                .cmdtm.CMD_RSPNS_TYPE = CMD_48BIT_RESP,
                .use_rca = 0,
                .delay = 0
        },
        [IDX_SEND_NUM_ERS] = {
                "SEND_NUM_ERS",
                .cmdtm.CMD_INDEX = 0x17,
                .cmdtm.CMD_RSPNS_TYPE = CMD_48BIT_RESP,
                .use_rca = 0,
                .delay = 0
        },
        [IDX_APP_SEND_OP_COND] = {
                "SD_SENDOPCOND",
                .cmdtm.CMD_INDEX = 0x29,
                .cmdtm.CMD_RSPNS_TYPE = CMD_48BIT_RESP,
                .use_rca = 0,
                .delay = 1000
        },
        [IDX_SET_CLR_DET] = {
                "SET_CLR_DET",
                .cmdtm.CMD_INDEX = 0x2A,
                .cmdtm.CMD_RSPNS_TYPE = CMD_48BIT_RESP,
                .use_rca = 0,
                .delay = 0
        },
        [IDX_SEND_SCR] = {
                "SEND_SCR",
                .cmdtm.CMD_INDEX = 0x33,
                .cmdtm.CMD_RSPNS_TYPE = CMD_48BIT_RESP,
                .cmdtm.CMD_ISDATA = 1,
                .cmdtm.TM_DAT_DIR = 1,
                .use_rca = 0,
                .delay = 0
        },
};

result_t sdhci_cmds_is_app_cmd(
        uint32_t sdhci_cmd_index,
        bool *is_app_cmd
) {
    if (sdhci_cmd_index > IDX_SEND_SCR) {
        return result_err("Command index out of range in sdhci_commands_is_app_cmd()");
    }
    /* If the command index is in the range of app commands then return true. */
    *is_app_cmd = (sdhci_cmd_index >= IDX_APP_CMD_START);
    return result_ok();
}

result_t sdhci_cmds_get_cmd(
        uint32_t sdhci_cmd_index,
        sdhci_cmd_t **ret_val
) {
    if (sdhci_cmd_index > IDX_SEND_SCR) {
        return result_err("Command index out of range in sdhci_commands_get_cmd()");
    }
    if (ret_val == NULL) {
        return result_err("ret_val is NULL in sdhci_commands_get_cmd()");
    }
    /* Obtains the relevant SDHCI command. */
    *ret_val = &sdhci_cmds[sdhci_cmd_index];
    return result_ok();
}
