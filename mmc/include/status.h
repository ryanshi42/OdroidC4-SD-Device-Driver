#pragma once

#include <stdint.h>
#include "result.h"

typedef struct status status_t;
struct __attribute__((__packed__, aligned(4))) status {
    union {
        struct __attribute__((__packed__, aligned(1))) {
            volatile unsigned CMD_INHIBIT: 1;          // @0		Command line still used by previous command
            volatile unsigned DAT_INHIBIT: 1;          // @1		Data lines still used by previous data transfer
            volatile unsigned DAT_ACTIVE: 1;           // @2		At least one data line is active
            unsigned reserved: 5;                      // @3-7		Write as zero read as don't care
            volatile unsigned WRITE_TRANSFER: 1;       // @8		New data can be written to EMMC
            volatile unsigned READ_TRANSFER: 1;        // @9		New data can be read from EMMC
            unsigned reserved1: 10;                    // @10-19	Write as zero read as don't care
            volatile unsigned DAT_LEVEL0: 4;           // @20-23	Value of data lines DAT3 to DAT0
            volatile unsigned CMD_LEVEL: 1;            // @24		Value of command line CMD
            volatile unsigned DAT_LEVEL1: 4;           // @25-28	Value of data lines DAT7 to DAT4
            unsigned reserved2: 3;                     // @29-31	Write as zero read as don't care
        };
        volatile uint32_t raw32;                       // @0-31	Union to access all 32 bits as a uint32_t
    };
};

/**
 * Returns the `CMD_INHIBIT` field of the `status` register.
 * @param status
 * @param ret_val
 * @return
 */
result_t status_get_cmd_inhibit(status_t *status, bool *ret_val);

/**
 * Returns the `DAT_INHIBIT` field of the `status` register.
 * @param status
 * @param ret_val
 * @return
 */
result_t status_get_dat_inhibit(status_t *status, bool *ret_val);

/**
 * Returns the `READ_TRANSFER` field of the `status` register.
 * @param status
 * @param ret_val
 * @return
 */
result_t status_get_read_transfer(status_t *status, bool *ret_val);

/**
 * Returns the `WRITE_TRANSFER` field of the `status` register.
 * @param status
 * @param ret_val
 * @return
 */
result_t status_get_write_transfer(status_t *status, bool *ret_val);
