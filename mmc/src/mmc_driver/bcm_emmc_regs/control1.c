#include "control1.h"

result_t control1_set_raw32(control1_t *control1, uint32_t val) {
    if (control1 == NULL) {
        return result_err("NULL `control1` passed to control1_set_raw32().");
    }
    control1->raw32 = val;
    return result_ok();
}

result_t control1_get_raw32(control1_t *control1, uint32_t *ret_val) {
    if (control1 == NULL) {
        return result_err("NULL `control1` passed to control1_get_raw32().");
    }
    if (ret_val == NULL) {
        return result_err("NULL `ret_val` passed to control1_get_raw32().");
    }
    *ret_val = control1->raw32;
    return result_ok();
}
