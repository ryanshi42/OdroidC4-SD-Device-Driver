#pragma once

#include "result.h"
#include "cmd_rspns_type.h"
#include "cmd_index.h"

enum tm_auto_cmd_en {
    TM_NO_COMMAND = 0,  // no command
    TM_CMD12 = 1,       // command CMD12
    TM_CMD23 = 2,       // command CMD23
    TM_RESERVED = 3,
};
typedef enum tm_auto_cmd_en tm_auto_cmd_en_t;

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
            volatile cmd_index_t CMD_INDEX: 6;              // @24-29
            unsigned reserved3: 2;                          // @30-31	Write as zero read as don't care
        };
        volatile uint32_t raw32;                            // @0-31	Union to access all 32 bits as a uint32_t
    };
};

/**
 * Returns the `CMD_RSPNS_TYPE` field from the `cmdtm_t` structure.
 * @param cmdtm
 * @param ret_val
 * @return
 */
result_t cmdtm_get_cmd_rspns_type(
        cmdtm_t *cmdtm,
        cmd_rspns_type_t *ret_val
);

/**
 * Returns the `CMD_INDEX` field from the `cmdtm_t` structure.
 * @param cmdtm
 * @param ret_val
 * @return
 */
result_t cmdtm_get_cmd_index(
        cmdtm_t *cmdtm,
        cmd_index_t *ret_val
);

