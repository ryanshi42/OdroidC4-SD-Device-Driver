#include "sdcard_data_v1.h"

result_t sdcard_data_v1_get_c_size(csd_t *csd, uint32_t *ret_val) {
    if (csd == NULL) {
        return result_err("NULL `csd` passed to sdcard_data_v1_get_c_size().");
    }
    if (ret_val == NULL) {
        return result_err("NULL `ret_val` passed to sdcard_data_v1_get_c_size().");
    }
    return csd_get_c_size(csd, ret_val);
}

result_t sdcard_data_v1_get_memory_capacity(csd_t *csd, uint64_t *ret_val) {
    if (csd == NULL) {
        return result_err("NULL `csd` passed to sdcard_data_v1_get_memory_capacity().");
    }
    if (ret_val == NULL) {
        return result_err("NULL `ret_val` passed to sdcard_data_v1_get_memory_capacity().");
    }
    /* TODO: Refer to the SDHCI physical specification for implementation. */
    *ret_val = 0;
    return result_ok();
}

result_t sdcard_data_v1_get_block_size(csd_t *csd, uint16_t *ret_val) {
    if (csd == NULL) {
        return result_err("NULL `csd` passed to sdcard_data_v1_get_block_size().");
    }
    if (ret_val == NULL) {
        return result_err("NULL `ret_val` passed to sdcard_data_v1_get_block_size().");
    }
    uint8_t read_bl_len;
    result_t res = csd_get_read_bl_len(csd, &read_bl_len);
    if (result_is_err(res)) {
        return result_err("Failed to get read block length in sdcard_data_v1_get_block_size().");
    }
    uint8_t write_bl_len;
    res = csd_get_write_bl_len(csd, &write_bl_len);
    if (result_is_err(res)) {
        return result_err("Failed to get write block length in sdcard_data_v1_get_block_size().");
    }
    if (read_bl_len != write_bl_len) {
        return result_err("Read block length and write block length are not equal in sdcard_data_v1_get_block_size().");
    }
    *ret_val = 1 << read_bl_len;
    return result_ok();
}

result_t sdcard_data_v1_get_num_blocks(csd_t *csd, uint64_t *ret_val) {
    if (csd == NULL) {
        return result_err("NULL `csd` passed to sdcard_data_v1_get_num_blocks().");
    }
    if (ret_val == NULL) {
        return result_err("NULL `ret_val` passed to sdcard_data_v1_get_num_blocks().");
    }
    /* TODO: Refer to the SDHCI physical specification for implementation. */
    *ret_val = 0;
    return result_ok();
}
