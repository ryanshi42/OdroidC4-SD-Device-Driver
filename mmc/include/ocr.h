#pragma once

#include <stdint.h>

#include "result.h"

/**
 * SD card Operation Conditions Register (OCR) register.
 */
typedef struct ocr ocr_t;
struct __attribute__((__packed__, aligned(4))) ocr {
    union {
        struct __attribute__((__packed__, aligned(1))) {
            unsigned reserved: 15;           // @0-14	Write as zero read as don't care
            unsigned voltage2v7to2v8: 1;     // @15		Voltage window 2.7v to 2.8v
            unsigned voltage2v8to2v9: 1;     // @16		Voltage window 2.8v to 2.9v
            unsigned voltage2v9to3v0: 1;     // @17		Voltage window 2.9v to 3.0v
            unsigned voltage3v0to3v1: 1;     // @18		Voltage window 3.0v to 3.1v
            unsigned voltage3v1to3v2: 1;     // @19		Voltage window 3.1v to 3.2v
            unsigned voltage3v2to3v3: 1;     // @20		Voltage window 3.2v to 3.3v
            unsigned voltage3v3to3v4: 1;     // @21		Voltage window 3.3v to 3.4v
            unsigned voltage3v4to3v5: 1;     // @22		Voltage window 3.4v to 3.5v
            unsigned voltage3v5to3v6: 1;     // @23		Voltage window 3.5v to 3.6v
            unsigned reserved1: 6;           // @24-29	Write as zero read as don't care
            unsigned card_capacity: 1;       // @30		Card Capacity status
            unsigned card_power_up_busy: 1;  // @31		Card power up status (busy)
        };
        volatile uint32_t raw32;             // @0-31	Union to access 32 bits as a uint32_t
    };
};

/**
 * Sets the OCR register to the given value.
 * @param ocr
 * @param val
 * @return
 */
result_t ocr_set_raw32(ocr_t *ocr, uint32_t val);

/**
 * True means the card has finished Power Up Routine and False means the card is not Powered Up.
 * @param ocr
 * @param ret_val
 * @return
 */
result_t ocr_get_card_power_up_busy(ocr_t *ocr, bool *ret_val);

/**
 * Returns True if voltage window is 3.2v to 3.3v.
 * @param ocr
 * @param ret_val
 * @return
 */
result_t ocr_get_voltage_3v2to3v3(ocr_t *ocr, bool *ret_val);

/**
 * Returns True if voltage window is 3.3v to 3.4v.
 * @param ocr
 * @param ret_val
 * @return
 */
result_t ocr_get_voltage_3v3to3v4(ocr_t *ocr, bool *ret_val);

/**
 * Returns True if card is SDSC or SDHC.
 * @param ocr
 * @param ret_val
 * @return
 */
result_t ocr_get_card_capacity(ocr_t *ocr, bool *ret_val);

