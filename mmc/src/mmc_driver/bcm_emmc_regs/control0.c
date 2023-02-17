#include "control0.h"

result_t control0_set_raw32(control0_t *control0, uint32_t val) {
    if (control0 == NULL) {
        return result_err("NULL `control0` passed to control0_set_raw32().");
    }
    control0->raw32 = val;
    return result_ok();
}

result_t control0_get_raw32(control0_t *control0, uint32_t *ret_val) {
    if (control0 == NULL) {
        return result_err("NULL `control0` passed to control0_get_raw32().");
    }
    if (ret_val == NULL) {
        return result_err("NULL `ret_val` passed to control0_get_raw32().");
    }
    *ret_val = control0->raw32;
    return result_ok();
}
