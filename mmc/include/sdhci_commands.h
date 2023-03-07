#pragma once

#include "sdhci_command.h"

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

/*--------------------------------------------------------------------------}
{							  SD CARD COMMAND TABLE						    }
{--------------------------------------------------------------------------*/
EMMCCommand sdCommandTable[IX_SEND_SCR + 1] = {
        [IX_GO_IDLE_STATE] = {
                "GO_IDLE_STATE",
                .cmdtm.CMD_INDEX = 0x00,
                .cmdtm.CMD_RSPNS_TYPE = CMD_NO_RESP,
                .use_rca = 0,
                .delay = 0
        },
        [IX_ALL_SEND_CID] = {
                "ALL_SEND_CID",
                .cmdtm.CMD_INDEX = 0x02,
                .cmdtm.CMD_RSPNS_TYPE = CMD_136BIT_RESP,
                .use_rca = 0,
                .delay = 0
        },
        [IX_SEND_REL_ADDR] = {
                "SEND_REL_ADDR",
                .cmdtm.CMD_INDEX = 0x03,
                .cmdtm.CMD_RSPNS_TYPE = CMD_48BIT_RESP,
                .use_rca = 0,
                .delay = 0
        },
        [IX_SET_DSR] = {
                "SET_DSR",
                .cmdtm.CMD_INDEX = 0x04,
                .cmdtm.CMD_RSPNS_TYPE = CMD_NO_RESP,
                .use_rca = 0,
                .delay = 0
        },
        [IX_SWITCH_FUNC] = {
                "SWITCH_FUNC",
                .cmdtm.CMD_INDEX = 0x06,
                .cmdtm.CMD_RSPNS_TYPE = CMD_48BIT_RESP,
                .use_rca = 0,
                .delay = 0
        },
        [IX_CARD_SELECT] = {
                "CARD_SELECT",
                .cmdtm.CMD_INDEX = 0x07,
                .cmdtm.CMD_RSPNS_TYPE = CMD_BUSY48BIT_RESP,
                .use_rca = 1,
                .delay = 0
        },
        [IX_SEND_IF_COND] = {
                "SEND_IF_COND",
                .cmdtm.CMD_INDEX = 0x08,
                .cmdtm.CMD_RSPNS_TYPE = CMD_48BIT_RESP,
                .use_rca = 0,
                .delay = 100
        },
        [IX_SEND_CSD] = {
                "SEND_CSD",
                .cmdtm.CMD_INDEX = 0x09,
                .cmdtm.CMD_RSPNS_TYPE = CMD_136BIT_RESP,
                .use_rca = 1,
                .delay = 0
        },
        [IX_SEND_CID] = {
                "SEND_CID",
                .cmdtm.CMD_INDEX = 0x0A,
                .cmdtm.CMD_RSPNS_TYPE = CMD_136BIT_RESP,
                .use_rca = 1,
                .delay = 0
        },
        [IX_VOLTAGE_SWITCH] = {
                "VOLT_SWITCH",
                .cmdtm.CMD_INDEX = 0x0B,
                .cmdtm.CMD_RSPNS_TYPE = CMD_48BIT_RESP,
                .use_rca = 0,
                .delay = 0
        },
        [IX_STOP_TRANS] = {
                "STOP_TRANS",
                .cmdtm.CMD_INDEX = 0x0C,
                .cmdtm.CMD_RSPNS_TYPE = CMD_BUSY48BIT_RESP,
                .use_rca = 0,
                .delay = 0
        },
        [IX_SEND_STATUS] = {
                "SEND_STATUS",
                .cmdtm.CMD_INDEX = 0x0D,
                .cmdtm.CMD_RSPNS_TYPE = CMD_48BIT_RESP,
                .use_rca = 1,
                .delay = 0
        },
        [IX_GO_INACTIVE] = {
                "GO_INACTIVE",
                .cmdtm.CMD_INDEX = 0x0F,
                .cmdtm.CMD_RSPNS_TYPE = CMD_NO_RESP,
                .use_rca = 1,
                .delay = 0
        },
        [IX_SET_BLOCKLEN] = {
                "SET_BLOCKLEN",
                .cmdtm.CMD_INDEX = 0x10,
                .cmdtm.CMD_RSPNS_TYPE = CMD_48BIT_RESP,
                .use_rca = 0,
                .delay = 0
        },
        [IX_READ_SINGLE] = {"READ_SINGLE",
                .cmdtm.CMD_INDEX = 0x11,
                .cmdtm.CMD_RSPNS_TYPE = CMD_48BIT_RESP,
                .cmdtm.CMD_ISDATA = 1,
                .cmdtm.TM_DAT_DIR = 1,
                .use_rca = 0,
                .delay = 0
        },
        [IX_READ_MULTI] = {"READ_MULTI",
                .cmdtm.CMD_INDEX = 0x12,
                .cmdtm.CMD_RSPNS_TYPE = CMD_48BIT_RESP,
                .cmdtm.CMD_ISDATA = 1,
                .cmdtm.TM_DAT_DIR = 1,
                .cmdtm.TM_BLKCNT_EN =1,
                .cmdtm.TM_MULTI_BLOCK = 1,
                .use_rca = 0,
                .delay = 0
        },
        [IX_SEND_TUNING] = {
                "SEND_TUNING",
                .cmdtm.CMD_INDEX = 0x13,
                .cmdtm.CMD_RSPNS_TYPE = CMD_48BIT_RESP,
                .use_rca = 0,
                .delay = 0
        },
        [IX_SPEED_CLASS] = {
                "SPEED_CLASS",
                .cmdtm.CMD_INDEX = 0x14,
                .cmdtm.CMD_RSPNS_TYPE = CMD_BUSY48BIT_RESP,
                .use_rca = 0,
                .delay = 0
        },
        [IX_SET_BLOCKCNT] = {
                "SET_BLOCKCNT",
                .cmdtm.CMD_INDEX = 0x17,
                .cmdtm.CMD_RSPNS_TYPE = CMD_48BIT_RESP,
                .use_rca = 0,
                .delay = 0
        },
        [IX_WRITE_SINGLE] = {"WRITE_SINGLE",
                .cmdtm.CMD_INDEX = 0x18,
                .cmdtm.CMD_RSPNS_TYPE = CMD_48BIT_RESP,
                .cmdtm.CMD_ISDATA = 1,
                .use_rca = 0,
                .delay = 0
        },
        [IX_WRITE_MULTI] = {"WRITE_MULTI",
                .cmdtm.CMD_INDEX = 0x19,
                .cmdtm.CMD_RSPNS_TYPE = CMD_48BIT_RESP,
                .cmdtm.CMD_ISDATA = 1,
                .cmdtm.TM_BLKCNT_EN = 1,
                .cmdtm.TM_MULTI_BLOCK = 1,
                .use_rca = 0,
                .delay = 0
        },
        [IX_PROGRAM_CSD] = {
                "PROGRAM_CSD",
                .cmdtm.CMD_INDEX = 0x1B,
                .cmdtm.CMD_RSPNS_TYPE = CMD_48BIT_RESP,
                .use_rca = 0,
                .delay = 0
        },
        [IX_SET_WRITE_PR] = {
                "SET_WRITE_PR",
                .cmdtm.CMD_INDEX = 0x1C,
                .cmdtm.CMD_RSPNS_TYPE = CMD_BUSY48BIT_RESP,
                .use_rca = 0,
                .delay = 0
        },
        [IX_CLR_WRITE_PR] = {
                "CLR_WRITE_PR",
                .cmdtm.CMD_INDEX = 0x1D,
                .cmdtm.CMD_RSPNS_TYPE = CMD_BUSY48BIT_RESP,
                .use_rca = 0,
                .delay = 0
        },
        [IX_SND_WRITE_PR] = {
                "SND_WRITE_PR",
                .cmdtm.CMD_INDEX = 0x1E,
                .cmdtm.CMD_RSPNS_TYPE = CMD_48BIT_RESP,
                .use_rca = 0,
                .delay = 0
        },
        [IX_ERASE_WR_ST] = {
                "ERASE_WR_ST",
                .cmdtm.CMD_INDEX = 0x20,
                .cmdtm.CMD_RSPNS_TYPE = CMD_48BIT_RESP,
                .use_rca = 0,
                .delay = 0
        },
        [IX_ERASE_WR_END] = {
                "ERASE_WR_END",
                .cmdtm.CMD_INDEX = 0x21,
                .cmdtm.CMD_RSPNS_TYPE = CMD_48BIT_RESP,
                .use_rca = 0,
                .delay = 0
        },
        [IX_ERASE] = {
                "ERASE",
                .cmdtm.CMD_INDEX = 0x26,
                .cmdtm.CMD_RSPNS_TYPE = CMD_BUSY48BIT_RESP,
                .use_rca = 0,
                .delay = 0
        },
        [IX_LOCK_UNLOCK] = {
                "LOCK_UNLOCK",
                .cmdtm.CMD_INDEX = 0x2A,
                .cmdtm.CMD_RSPNS_TYPE = CMD_48BIT_RESP,
                .use_rca = 0,
                .delay = 0
        },
        [IX_APP_CMD] = {
                "APP_CMD",
                .cmdtm.CMD_INDEX = 0x37,
                .cmdtm.CMD_RSPNS_TYPE = CMD_NO_RESP,
                .use_rca = 0,
                .delay = 100
        },
        [IX_APP_CMD_RCA] = {
                "APP_CMD",
                .cmdtm.CMD_INDEX = 0x37,
                .cmdtm.CMD_RSPNS_TYPE = CMD_48BIT_RESP,
                .use_rca = 1,
                .delay = 0
        },
        [IX_GEN_CMD] = {
                "GEN_CMD",
                .cmdtm.CMD_INDEX = 0x38,
                .cmdtm.CMD_RSPNS_TYPE = CMD_48BIT_RESP,
                .use_rca = 0,
                .delay = 0
        },

        // APP commands must be prefixed by an APP_CMD.
        [IX_SET_BUS_WIDTH] = {
                "SET_BUS_WIDTH",
                .cmdtm.CMD_INDEX = 0x06,
                .cmdtm.CMD_RSPNS_TYPE = CMD_48BIT_RESP,
                .use_rca = 0,
                .delay = 0
        },
        [IX_SD_STATUS] = {
                "SD_STATUS",
                .cmdtm.CMD_INDEX = 0x0D,
                .cmdtm.CMD_RSPNS_TYPE = CMD_48BIT_RESP,
                .use_rca = 1,
                .delay = 0
        },
        [IX_SEND_NUM_WRBL] = {
                "SEND_NUM_WRBL",
                .cmdtm.CMD_INDEX = 0x16,
                .cmdtm.CMD_RSPNS_TYPE = CMD_48BIT_RESP,
                .use_rca = 0,
                .delay = 0
        },
        [IX_SEND_NUM_ERS] = {
                "SEND_NUM_ERS",
                .cmdtm.CMD_INDEX = 0x17,
                .cmdtm.CMD_RSPNS_TYPE = CMD_48BIT_RESP,
                .use_rca = 0,
                .delay = 0
        },
        [IX_APP_SEND_OP_COND] = {
                "SD_SENDOPCOND",
                .cmdtm.CMD_INDEX = 0x29,
                .cmdtm.CMD_RSPNS_TYPE = CMD_48BIT_RESP,
                .use_rca = 0,
                .delay = 1000
        },
        [IX_SET_CLR_DET] = {
                "SET_CLR_DET",
                .cmdtm.CMD_INDEX = 0x2A,
                .cmdtm.CMD_RSPNS_TYPE = CMD_48BIT_RESP,
                .use_rca = 0,
                .delay = 0
        },
        [IX_SEND_SCR] = {"SEND_SCR",
                .cmdtm.CMD_INDEX = 0x33,
                .cmdtm.CMD_RSPNS_TYPE = CMD_48BIT_RESP,
                .cmdtm.CMD_ISDATA = 1,
                .cmdtm.TM_DAT_DIR = 1,
                .use_rca = 0,
                .delay = 0
        },
};

