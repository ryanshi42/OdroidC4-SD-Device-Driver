#include "sdcard_data_v2.h"

result_t sdcard_data_v2_get_c_size(csd_t *csd, uint32_t *ret_val) {
    if (csd == NULL) {
        return result_err("NULL `csd` passed to sdcard_data_v2_get_c_size().");
    }
    if (ret_val == NULL) {
        return result_err("NULL `ret_val` passed to sdcard_data_v2_get_c_size().");
    }
    return csd_get_ver2_c_size(csd, ret_val);
}

result_t sdcard_data_v2_get_memory_capacity(csd_t *csd, uint64_t *ret_val) {
    if (csd == NULL) {
        return result_err("NULL `csd` passed to sdcard_data_v2_get_memory_capacity().");
    }
    if (ret_val == NULL) {
        return result_err("NULL `ret_val` passed to sdcard_data_v2_get_memory_capacity().");
    }
    /* Get Card Size. */
    uint32_t c_size = 0;
    result_t res = sdcard_data_v2_get_c_size(csd, &c_size);
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to get C_SIZE from CSD in sdcard_get_memory_capacity().");
    }
    /* Compute memory capacity. */
    *ret_val = (c_size + 1) * ((uint64_t) 512) * ((uint64_t) 1024);
    return result_ok();
}
