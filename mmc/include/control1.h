#pragma once

#include <stdint.h>

#include "result.h"

typedef struct control1 control1_t;
struct __attribute__((__packed__, aligned(4))) control1 {
    union {
        struct __attribute__((__packed__, aligned(1))) {
            volatile unsigned CLK_INTLEN: 1;           // @0		Clock enable for internal EMMC clocks for power saving
            volatile const unsigned CLK_STABLE: 1;     // @1		SD clock stable  0=No 1=yes   **read only
            volatile unsigned CLK_EN: 1;               // @2		SD clock enable  0=disable 1=enable
            unsigned reserved: 2;                      // @3-4		Write as zero read as don't care
            volatile unsigned CLK_GENSEL: 1;           // @5		Mode of clock generation (0=Divided, 1=Programmable)
            volatile unsigned CLK_FREQ_MS2: 2;         // @6-7		SD clock base divider MSBs (Version3+ only)
            volatile unsigned CLK_FREQ8: 8;            // @8-15	SD clock base divider LSBs
            volatile unsigned DATA_TOUNIT: 4;          // @16-19	Data timeout unit exponent
            unsigned reserved1: 4;                     // @20-23	Write as zero read as don't care
            volatile unsigned SRST_HC: 1;              // @24		Reset the complete host circuit
            volatile unsigned SRST_CMD: 1;             // @25		Reset the command handling circuit
            volatile unsigned SRST_DATA: 1;            // @26		Reset the data handling circuit
            unsigned reserved2: 5;                     // @27-31	Write as zero read as don't care
        };
        volatile uint32_t raw32;                       // @0-31	Union to access all 32 bits as a uint32_t
    };
};

/**
 * Set `control1` register to a `uint32`.
 * @param control1
 * @param val
 * @return
 */
result_t control1_set_raw32(control1_t *control1, uint32_t val);

/**
 * Obtains the `control1` register as a `uint32`.
 * @param control1
 * @param ret_val
 * @return
 */
result_t control1_get_raw32(control1_t *control1, uint32_t *ret_val);

/**
 * Set the `CLK_INTLEN` field of the `control1` register.
 * @param control1
 * @param val
 * @return
 */
result_t control1_set_clk_intlen(control1_t *control1, bool val);

/**
 * Sets the `CLK_EN` field of the `control1` register.
 * @param control1
 * @param ret_val
 * @return
 */
result_t control1_set_clk_en(control1_t *control1, bool val);

/**
 * Obtains the `DATA_TOUNIT` field of the `control1` register.
 * @param control1
 * @param ret_val
 * @return
 */
result_t control1_get_data_tounit(control1_t *control1, uint8_t *ret_val);

/**
 * Set the `DATA_TOUNIT` field of the `control1` register.
 * @param control1
 * @param val
 * @return
 */
result_t control1_set_data_tounit(control1_t *control1, uint8_t val);

/**
 * Set the `SRST_HC` field of the `control1` register.
 * @param control1
 * @param val
 * @return
 */
result_t control1_set_srst_hc(control1_t *control1, bool val);

/**
 * Obtains the `SRST_HC` field of the `control1` register.
 * @param control1
 * @param ret_val
 * @return
 */
result_t control1_get_srst_hc(control1_t *control1, bool *ret_val);

/**
 * Obtains the `SRST_CMD` field of the `control1` register.
 * @param control1
 * @param ret_val
 * @return
 */
result_t control1_get_srst_cmd(control1_t *control1, bool *ret_val);

/**
 * Obtains the `SRST_DATA` field of the `control1` register.
 * @param control1
 * @param ret_val
 * @return
 */
result_t control1_get_srst_data(control1_t *control1, bool *ret_val);
