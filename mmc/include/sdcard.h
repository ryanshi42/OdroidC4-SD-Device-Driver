#pragma once

#include "ocr.h"
#include "result.h"

typedef struct sdcard sdcard_t;
struct sdcard {
    ocr_t ocr;
};

/**
 * Saves the OCR register to the SD card.
 * @param sdcard
 * @param val
 * @return
 */
result_t sdcard_set_ocr_raw32(sdcard_t *sdcard, uint32_t val);

