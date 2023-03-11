#include "scr.h"

result_t scr_set_raw32_lo(scr_t *scr, uint32_t val) {
    if (scr == NULL) {
        return result_err("NULL `scr` passed to scr_set_raw32_lo().");
    }
    scr->raw32_lo = val;
    return result_ok();
}

result_t scr_set_raw32_hi(scr_t *scr, uint32_t val) {
    if (scr == NULL) {
        return result_err("NULL `scr` passed to scr_set_raw32_hi().");
    }
    scr->raw32_hi = val;
    return result_ok();
}

