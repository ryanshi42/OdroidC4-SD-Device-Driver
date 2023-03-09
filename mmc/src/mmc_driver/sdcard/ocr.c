#include "ocr.h"


result_t ocr_set_raw32(ocr_t *ocr, uint32_t val) {
    if (ocr == NULL) {
        return result_err("NULL `ocr` passed to ocr_set_raw32().");
    }
    ocr->raw32 = val;
    return result_ok();
}

result_t ocr_get_card_power_up_busy(ocr_t *ocr, bool *ret_val) {
    if (ocr == NULL) {
        return result_err("NULL `ocr` passed to ocr_get_card_power_up_busy().");
    }
    *ret_val = ocr->card_power_up_busy;
    return result_ok();
}
