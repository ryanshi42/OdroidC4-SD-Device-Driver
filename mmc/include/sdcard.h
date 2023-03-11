#pragma once

#include "result.h"
#include "ocr.h"
#include "cid.h"
#include "sdcard_type.h"
#include "scr.h"

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
    /* Card Identification Register. */
    cid_t cid;
    /* SD Card Type. */
    sdcard_type_t type;
    /* SD Card Configuration Register. */
    scr_t scr;
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

/**
 * Saves the CID register to the SD card.
 * @param sdcard
 * @param resp0 RESP0 register value.
 * @param resp1 RESP1 register value.
 * @param resp2 RESP2 register value.
 * @param resp3 RESP3 register value.
 * @return
 */
result_t sdcard_set_cid(
        sdcard_t *sdcard,
        uint32_t resp0,
        uint32_t resp1,
        uint32_t resp2,
        uint32_t resp3
);

/**
 * Sets the SD card type.
 * @param sdcard
 * @param type
 * @return
 */
result_t sdcard_set_type(sdcard_t *sdcard, sdcard_type_t type);

/**
 * Saves the lower 32 bits of the SD card SCR register.
 * @param sdcard
 * @param val
 * @return
 */
result_t sdcard_set_scr_raw32_lo(sdcard_t *sdcard, uint32_t val);

/**
 * Saves the upper 32 bits of the SD card SCR register.
 * @param sdcard
 * @param val
 * @return
 */
result_t sdcard_set_scr_raw32_hi(sdcard_t *sdcard, uint32_t val);

/**
 * Checks if the SD card is 4-bit bus width.
 * @param sdcard
 * @param ret_val
 * @return
 */
result_t sdcard_is_bus_width_4(sdcard_t *sdcard, bool *ret_val);

/**
 * Checks if the SD card is 1-bit bus width.
 * @param sdcard
 * @param ret_val
 * @return
 */
result_t sdcard_is_bus_width_1(sdcard_t *sdcard, bool *ret_val);
