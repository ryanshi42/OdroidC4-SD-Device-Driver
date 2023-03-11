#pragma once

#include <stdint.h>

#include "result.h"

/**
 * EMMC BLKSIZECNT register - BCM2835.PDF Manual Section 5 page 68
 */
typedef struct blksizecnt blksizecnt_t;
struct __attribute__((__packed__, aligned(4))) blksizecnt {
    union {
        struct __attribute__((__packed__, aligned(1))) {
            volatile unsigned BLKSIZE: 10;  // @0-9		Block size in bytes
            unsigned reserved: 6;           // @10-15	Write as zero read as don't care
            volatile unsigned BLKCNT: 16;   // @16-31	Number of blocks to be transferred
        };
        volatile uint32_t raw32;            // @0-31	Union to access all 32 bits as a uint32_t
    };
};

/**
 * Sets the BLKSIZE field of the blksizecnt register.
 * @param blksizecnt
 * @param val
 * @return
 */
result_t blksizecnt_set_blksize(blksizecnt_t *blksizecnt, uint32_t val);

/**
 * Sets the BLKCNT field of the blksizecnt register.
 * @param blksizecnt
 * @param val
 * @return
 */
result_t blksizecnt_set_blkcnt(blksizecnt_t *blksizecnt, uint32_t val);
