#pragma once

#include "ocr.h"
#include "result.h"

typedef struct sdcard sdcard_t;
struct sdcard {
    /* Operation Conditions Register. */
    ocr_t ocr;
    /* Relative Card Address. */
    uint32_t rca;
    /* Card Status. */
    uint32_t status;
};

/**
 * Initialises the SD card.
 * @param sdcard
 * @return
 */
result_t sdcard_init(sdcard_t *sdcard);

/**
 * Saves the OCR register to the SD card.
 * @param sdcard
 * @param val
 * @return
 */
result_t sdcard_set_ocr_raw32(sdcard_t *sdcard, uint32_t val);

/**
 * Saves the Relative Card Address.
 * @param sdcard
 * @param val
 * @return
 */
result_t sdcard_set_rca(sdcard_t *sdcard, uint32_t val);

/**
 * Saves Card Status.
 * @param sdcard
 * @param val
 * @return
 */
result_t sdcard_set_status(sdcard_t *sdcard, uint32_t val);

/**
 * Checks if the SD card has an RCA.
 * @param sdcard
 * @param ret_val
 * @return
 */
result_t sdcard_has_rca(sdcard_t *sdcard, bool *ret_val);

