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

result_t sdcard_data_v2_get_block_size(csd_t *csd, uint16_t *ret_val) {
    if (csd == NULL) {
        return result_err("NULL `csd` passed to sdcard_data_v2_get_block_size().");
    }
    if (ret_val == NULL) {
        return result_err("NULL `ret_val` passed to sdcard_data_v2_get_block_size().");
    }
    uint8_t read_bl_len;
    result_t res = csd_get_read_bl_len(csd, &read_bl_len);
    if (result_is_err(res)) {
        return result_err("Failed to get read block length in sdcard_data_v2_get_block_size().");
    }
    uint8_t write_bl_len;
    res = csd_get_write_bl_len(csd, &write_bl_len);
    if (result_is_err(res)) {
        return result_err("Failed to get write block length in sdcard_data_v2_get_block_size().");
    }
    if (read_bl_len != write_bl_len) {
        return result_err("Read block length and write block length are not equal in sdcard_data_v2_get_block_size().");
    }
    if (read_bl_len != 9) {
        return result_err("Read block length is not 512 bytes in sdcard_data_v2_get_block_size().");
    }
    *ret_val = 512;
    return result_ok();
}

result_t sdcard_data_v2_get_num_blocks(csd_t *csd, uint64_t *ret_val) {
    if (csd == NULL) {
        return result_err("NULL `csd` passed to sdcard_data_v2_get_num_blocks().");
    }
    if (ret_val == NULL) {
        return result_err("NULL `ret_val` passed to sdcard_data_v2_get_num_blocks().");
    }
    /* Get Card Size. */
    uint32_t c_size = 0;
    result_t res = sdcard_data_v2_get_c_size(csd, &c_size);
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to get C_SIZE from CSD in sdcard_get_memory_capacity().");
    }
    /* Compute number of blocks.*/
    *ret_val = (c_size + 1) * ((uint64_t) 1024);
    return result_ok();
}


