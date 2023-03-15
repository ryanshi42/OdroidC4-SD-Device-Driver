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

result_t scr_get_bus_width(scr_t *scr, bus_width_t *ret_val) {
    if (scr == NULL) {
        return result_err("NULL `scr` passed to scr_get_bus_width().");
    }
    if (ret_val == NULL) {
        return result_err("NULL `ret_val` passed to scr_get_bus_width().");
    }
    *ret_val = scr->BUS_WIDTH;
    return result_ok();
}

result_t scr_is_bus_width_1_supported(scr_t *scr, bool *ret_val) {
    if (scr == NULL) {
        return result_err("NULL `scr` passed to scr_is_bus_width_1().");
    }
    if (ret_val == NULL) {
        return result_err("NULL `ret_val` passed to scr_is_bus_width_1().");
    }
    *ret_val = scr->BUS_WIDTH & BUS_WIDTH_1;
    return result_ok();
}

result_t scr_is_bus_width_4_supported(scr_t *scr, bool *ret_val) {
    if (scr == NULL) {
        return result_err("NULL `scr` passed to scr_is_bus_width_4().");
    }
    if (ret_val == NULL) {
        return result_err("NULL `ret_val` passed to scr_is_bus_width_4().");
    }
    *ret_val = scr->BUS_WIDTH & BUS_WIDTH_4;
    return result_ok();
}

result_t scr_is_set_block_count_cmd_supported(scr_t *scr, bool *ret_val) {
    if (scr == NULL) {
        return result_err("NULL `scr` passed to scr_is_set_block_count_cmd_supported().");
    }
    if (ret_val == NULL) {
        return result_err("NULL `ret_val` passed to scr_is_set_block_count_cmd_supported().");
    }
    *ret_val = scr->CMD_SUPPORT & CMD_SUPP_SET_BLKCNT;
    return result_ok();
}

result_t scr_is_speed_class_control_cmd_supported(scr_t *scr, bool *ret_val) {
    if (scr == NULL) {
        return result_err("NULL `scr` passed to scr_is_speed_class_control_cmd_supported().");
    }
    if (ret_val == NULL) {
        return result_err("NULL `ret_val` passed to scr_is_speed_class_control_cmd_supported().");
    }
    *ret_val = scr->CMD_SUPPORT & CMD_SUPP_SPEED_CLASS;
    return result_ok();
}


