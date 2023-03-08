#include "sdcard.h"


result_t sdcard_set_ocr_raw32(sdcard_t *sdcard, uint32_t val) {
    if (sdcard == NULL) {
        return result_err("NULL `sdcard` passed to sdcard_set_ocr_raw32().");
    }
    return ocr_set_raw32(&sdcard->ocr, val);
}


