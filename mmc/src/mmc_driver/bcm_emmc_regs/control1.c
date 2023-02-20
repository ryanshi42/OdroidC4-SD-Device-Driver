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

result_t control1_set_srst_hc(control1_t *control1, bool val) {
    if (control1 == NULL) {
        return result_err("NULL `control1` passed to control1_set_srst_hc().");
    }
    control1->SRST_HC = val;
    return result_ok();
}

result_t control1_get_srst_hc(control1_t *control1, bool *ret_val) {
    if (control1 == NULL) {
        return result_err("NULL `control1` passed to control1_get_srst_hc().");
    }
    *ret_val = control1->SRST_HC;
    return result_ok();
}

result_t control1_get_srst_cmd(control1_t *control1, bool *ret_val) {
    if (control1 == NULL) {
        return result_err("NULL `control1` passed to control1_get_srst_cmd().");
    }
    *ret_val = control1->SRST_CMD;
    return result_ok();
}

result_t control1_get_srst_data(control1_t *control1, bool *ret_val) {
    if (control1 == NULL) {
        return result_err("NULL `control1` passed to control1_get_srst_data().");
    }
    *ret_val = control1->SRST_DATA;
    return result_ok();
}
