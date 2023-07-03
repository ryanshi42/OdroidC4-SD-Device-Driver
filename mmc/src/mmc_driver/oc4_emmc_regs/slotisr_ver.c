#include "slotisr_ver.h"

result_t slotisr_ver_get_sdversion(slotisr_ver_t *slotisr_ver, uint8_t *ret_val) {
    if (slotisr_ver == NULL) {
        return result_err("NULL `slotisr_ver` passed to slotisr_ver_get_sdversion().");
    }
    if (ret_val == NULL) {
        return result_err("NULL `ret_val` passed to slotisr_ver_get_sdversion().");
    }
    *ret_val = slotisr_ver->SDVERSION;
    return result_ok();
}


