#pragma once

#include "ocr.h"
#include "result.h"

/* Card Status Mask that indicates APP_CMD was accepted. */
#define ST_APP_CMD (0x00000020)

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
 * Gets the Relative Card Address.
 * @param sdcard
 * @param ret_val
 * @return
 */
result_t sdcard_get_rca(sdcard_t *sdcard, uint32_t *ret_val);

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
 * Masks the SD card status.
 * @param sdcard
 * @param mask
 * @param ret_val
 * @return
 */
result_t sdcard_mask_status(sdcard_t *sdcard, uint32_t mask, bool* ret_val);

/**
 * Checks if the SD card has an RCA.
 * @param sdcard
 * @param ret_val
 * @return
 */
result_t sdcard_has_rca(sdcard_t *sdcard, bool *ret_val);

/**
 * Checks if the App CMD was accepted using the SD card status.
 * @param sdcard
 * @param ret_val
 * @return
 */
result_t sdcard_is_app_cmd_accepted(sdcard_t *sdcard, bool *ret_val);

/**
 * Returns True if the SD card has powered up and False otherwise.
 * @param sdcard
 * @param ret_val
 * @return
 */
result_t sdcard_has_powered_up(sdcard_t *sdcard, bool *ret_val);

/**
 * Returns True if the SD card is 3.3V and False otherwise.
 * @param sdcard
 * @param ret_val
 * @return
 */
result_t sdcard_is_voltage_3v3(sdcard_t *sdcard, bool *ret_val);

/**
 * Returns True if the SD card is SDHC (High Capacity) and False otherwise.
 * @param sdcard
 * @param ret_val
 * @return
 */
result_t sdcard_is_high_capacity(sdcard_t *sdcard, bool *ret_val);
