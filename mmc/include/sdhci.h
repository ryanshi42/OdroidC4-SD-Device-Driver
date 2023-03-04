#pragma once

#include "result.h"
#include "bcm_emmc_regs.h"
#include "sleep.h"
#include "log.h"
#include "arith.h"

enum tm_auto_cmd_en {
    TM_NO_COMMAND = 0,  // no command
    TM_CMD12 = 1,       // command CMD12
    TM_CMD23 = 2,       // command CMD23
    TM_RESERVED = 3,
};
typedef enum tm_auto_cmd_en tm_auto_cmd_en_t;

enum cmd_rspns_type {
    CMD_NO_RESP = 0,        // no response
    CMD_136BIT_RESP = 1,    // 136 bits response
    CMD_48BIT_RESP = 2,     // 48 bits response
    CMD_BUSY48BIT_RESP = 3, // 48 bits response using busy
};
typedef enum cmd_rspns_type cmd_rspns_type_t;

enum cmd_type {
    CMD_TYPE_NORMAL = 0,  // normal command
    CMD_TYPE_SUSPEND = 1, // suspend command
    CMD_TYPE_RESUME = 2,  // resume command
    CMD_TYPE_ABORT = 3,   // abort command
};
typedef enum cmd_type cmd_type_t;

/**
 * EMMC CMDTM register - BCM2835.PDF Manual Section 5 pages 69-70
 */
typedef struct cmdtm cmdtm_t;
struct __attribute__((__packed__, aligned(4))) cmdtm {
    union {
        struct __attribute__((__packed__, aligned(1))) {
            unsigned reserved: 1;                           // @0		Write as zero read as don't care
            volatile unsigned TM_BLKCNT_EN: 1;              // @1		Enable the block counter for multiple block transfers
            volatile tm_auto_cmd_en_t TM_AUTO_CMD_EN: 2;    // @2-3		Select the command to be sent after completion of a data transfer
            volatile unsigned TM_DAT_DIR: 1;                // @4		Direction of data transfer (0 = host to card , 1 = card to host )
            volatile unsigned TM_MULTI_BLOCK: 1;            // @5		Type of data transfer (0 = single block, 1 = muli block)
            unsigned reserved1: 10;                         // @6-15	Write as zero read as don't care
            volatile cmd_rspns_type_t CMD_RSPNS_TYPE: 2;    // @16-17
            unsigned reserved2: 1;                          // @18		Write as zero read as don't care
            volatile unsigned CMD_CRCCHK_EN: 1;             // @19		Check the responses CRC (0=disabled, 1= enabled)
            volatile unsigned CMD_IXCHK_EN: 1;              // @20		Check that response has same index as command (0=disabled, 1= enabled)
            volatile unsigned CMD_ISDATA: 1;                // @21		Command involves data transfer (0=disabled, 1= enabled)
            volatile cmd_type_t CMD_TYPE: 2;                // @16-17
            volatile unsigned CMD_INDEX: 6;                 // @24-29
            unsigned reserved3: 2;                          // @30-31	Write as zero read as don't care
        };
        volatile uint32_t Raw32;                            // @0-31	Union to access all 32 bits as a uint32_t
    };
};
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
