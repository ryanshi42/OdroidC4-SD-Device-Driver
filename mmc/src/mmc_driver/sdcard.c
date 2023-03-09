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
