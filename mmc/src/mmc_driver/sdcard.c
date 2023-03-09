#include "sdcard.h"

result_t sdcard_init(sdcard_t *sdcard) {
    if (sdcard == NULL) {
        return result_err("NULL `sdcard` passed to sdcard_init().");
    }
    /* Initialise everything to 0. */
    memset(sdcard, 0, sizeof(sdcard_t));
    return result_ok();
}

result_t sdcard_set_ocr_raw32(sdcard_t *sdcard, uint32_t val) {
    if (sdcard == NULL) {
        return result_err("NULL `sdcard` passed to sdcard_set_ocr_raw32().");
    }
    return ocr_set_raw32(&sdcard->ocr, val);
}

result_t sdcard_get_rca(sdcard_t *sdcard, uint32_t *ret_val) {
    if (sdcard == NULL) {
        return result_err("NULL `sdcard` passed to sdcard_get_rca().");
    }
    if (ret_val == NULL) {
        return result_err("NULL `ret_val` passed to sdcard_get_rca().");
    }
    *ret_val = sdcard->rca;
    return result_ok();
}

result_t sdcard_set_rca(sdcard_t *sdcard, uint32_t val) {
    if (sdcard == NULL) {
        return result_err("NULL `sdcard` passed to sdcard_set_rca().");
    }
    sdcard->rca = val;
    return result_ok();
}

result_t sdcard_set_status(sdcard_t *sdcard, uint32_t val) {
    if (sdcard == NULL) {
        return result_err("NULL `sdcard` passed to sdcard_set_status().");
    }
    sdcard->status = val;
    return result_ok();
}

result_t sdcard_mask_status(sdcard_t *sdcard, uint32_t mask, bool* ret_val) {
    if (sdcard == NULL) {
        return result_err("NULL `sdcard` passed to sdcard_mask_status().");
    }
    if (ret_val == NULL) {
        return result_err("NULL `ret_val` passed to sdcard_mask_status().");
    }
    *ret_val = sdcard->status & mask;
    return result_ok();
}

result_t sdcard_has_rca(sdcard_t *sdcard, bool *ret_val) {
    if (sdcard == NULL) {
        return result_err("NULL `sdcard` passed to sdcard_has_rca().");
    }
    if (ret_val == NULL) {
        return result_err("NULL `ret_val` passed to sdcard_has_rca().");
    }
    *ret_val = sdcard->rca != 0;
    return result_ok();
}

result_t sdcard_is_app_cmd_accepted(sdcard_t *sdcard, bool *ret_val) {
    if (sdcard == NULL) {
        return result_err("NULL `sdcard` passed to sdcard_is_app_cmd_accepted().");
    }
    return sdcard_mask_status(sdcard, ST_APP_CMD, ret_val);
}

result_t sdcard_has_powered_up(sdcard_t *sdcard, bool *ret_val) {
    if (sdcard == NULL) {
        return result_err("NULL `sdcard` passed to sdcard_has_powered_up().");
    }
    return ocr_get_card_power_up_busy(&sdcard->ocr, ret_val);
}

result_t sdcard_is_voltage_3v3(sdcard_t *sdcard, bool *ret_val) {
    if (sdcard == NULL) {
        return result_err("NULL `sdcard` passed to sdcard_is_voltage_3v3().");
    }
    bool voltage3v2to3v3 = false;
    result_t result = ocr_get_voltage_3v2to3v3(&sdcard->ocr, &voltage3v2to3v3);
    if (result_is_err(result)) {
        return result_err_chain(result, "Failed to get voltage 3v2to3v3.");
    }
    bool voltage3v3to3v4 = false;
    result = ocr_get_voltage_3v3to3v4(&sdcard->ocr, &voltage3v3to3v4);
    if (result_is_err(result)) {
        return result_err_chain(result, "Failed to get voltage 3v3to3v4.");
    }
    *ret_val = voltage3v2to3v3 || voltage3v3to3v4;
    return result_ok();
}

result_t sdcard_is_high_capacity(sdcard_t *sdcard, bool *ret_val) {
    if (sdcard == NULL) {
        return result_err("NULL `sdcard` passed to sdcard_is_high_capacity().");
    }
    bool has_powered_up = false;
    result_t result = sdcard_has_powered_up(sdcard, &has_powered_up);
    if (result_is_err(result)) {
        return result_err_chain(result, "Failed to get power up status in sdcard_is_high_capacity().");
    }
    /* Per the SD Physical spec, the capacity bit is only valid when the card is powered up. */
    if (!has_powered_up) {
        return result_err("SD card has not powered up in sdcard_is_high_capacity().");
    }
    return ocr_get_card_capacity(&sdcard->ocr, ret_val);
}

result_t sdcard_set_cid(
        sdcard_t *sdcard,
        uint32_t resp0,
        uint32_t resp1,
        uint32_t resp2,
        uint32_t resp3
) {
    if (sdcard == NULL) {
        return result_err("NULL `sdcard` passed to sdcard_set_cid().");
    }
    result_t res;
    res = cid_set_raw32_3(&sdcard->cid, resp0);
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to set CID raw32_3 in sdcard_set_cid().");
    }
    res = cid_set_raw32_2(&sdcard->cid, resp1);
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to set CID raw32_2 in sdcard_set_cid().");
    }
    res = cid_set_raw32_1(&sdcard->cid, resp2);
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to set CID raw32_1 in sdcard_set_cid().");
    }
    res = cid_set_raw32_0(&sdcard->cid, resp3);
    return result_ok_or(res, "Failed to set CID raw32_0 in sdcard_set_cid().");
}

