#pragma once

#include <stdint.h>
#include "result.h"

typedef struct slotisr_ver slotisr_ver_t;
struct __attribute__((__packed__, aligned(4))) slotisr_ver {
    union {
        struct __attribute__((__packed__, aligned(1))) {
            volatile unsigned SLOT_STATUS: 8;  // @0-7		Logical OR of interrupt and wakeup signal for each slot
            unsigned reserved: 8;              // @8-15	    Write as zero read as don't care
            volatile unsigned SDVERSION: 8;    // @16-23	Host Controller specification version
            volatile unsigned VENDOR: 8;       // @24-31	Vendor Version Number
        };
        volatile uint32_t raw32;               // @0-31	    Union to access all 32 bits as a uint32_t
    };
};

/**
 * Returns the `SDVERSION` field of the `slotisr_ver` register.
 * @param slotisr_ver
 * @param ret_val
 * @return
 */
result_t slotisr_ver_get_sdversion(slotisr_ver_t *slotisr_ver, uint8_t *ret_val);

