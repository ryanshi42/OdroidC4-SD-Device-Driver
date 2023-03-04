#pragma once

#include "result.h"

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
