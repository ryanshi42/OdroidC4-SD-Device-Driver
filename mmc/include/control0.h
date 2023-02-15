#pragma once

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "result.h"

typedef struct control0 control0_t;
struct __attribute__((__packed__, aligned(4))) control0 {
    union {
        struct __attribute__((__packed__, aligned(1))) {
            unsigned reserved0: 1;              // @0		Write as zero read as don't care
            volatile unsigned HCTL_DWIDTH: 1;   // @1		Use 4 data lines (true = enable)
            volatile unsigned HCTL_HS_EN: 1;    // @2		Select high speed mode (true = enable)
            unsigned reserved1: 2;              // @3-4		Write as zero read as don't care
            volatile unsigned HCTL_8BIT: 1;     // @5		Use 8 data lines (true = enable)
            unsigned reserved2: 10;             // @6-15	Write as zero read as don't care
            volatile unsigned GAP_STOP: 1;      // @16		Stop the current transaction at the next block gap
            volatile unsigned GAP_RESTART: 1;   // @17		Restart a transaction last stopped using the GAP_STOP
            volatile unsigned READWAIT_EN: 1;   // @18		Use DAT2 read-wait protocol for cards supporting this
            volatile unsigned GAP_IEN: 1;       // @19		Enable SDIO interrupt at block gap
            volatile unsigned SPI_MODE: 1;      // @20		SPI mode enable
            volatile unsigned BOOT_EN: 1;       // @21		Boot mode access
            volatile unsigned ALT_BOOT_EN: 1;   // @22		Enable alternate boot mode access
            unsigned reserved3: 9;              // @23-31	Write as zero read as don't care
        };
        volatile uint32_t raw32;                // @0-31	Union to access all 32 bits as a uint32_t
    };
};

result_t control0_set_raw32(control0_t *control0, uint32_t val);

result_t control0_get_raw32(control0_t *control0, uint32_t *ret_val);
