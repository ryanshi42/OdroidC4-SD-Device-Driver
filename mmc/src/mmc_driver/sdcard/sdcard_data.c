#include "sdcard_data.h"

result_t sdcard_data_init(
        sdcard_data_t *sdcard_data,
        uint32_t resp0,
        uint32_t resp1,
        uint32_t resp2,
        uint32_t resp3
) {
    if (sdcard_data == NULL) {
        return result_err("NULL `sdcard_data` passed to sdcard_data_init().");
    }
    /* Initialise the Card Specific Data. */
    result_t res = csd_init(&sdcard_data->csd, resp0, resp1, resp2, resp3);
    if (result_is_err(res)) {
        return result_err("Failed to initialize CSD.");
    }
    /* Get the CSD version. */
    csd_structure_t csd_structure;
    res = csd_get_csd_structure(&sdcard_data->csd, &csd_structure);
    if (result_is_err(res)) {
        return result_err("Failed to get CSD structure.");
    }
    if (csd_structure == CSD_VERSION_1) {
        sdcard_data->sdcard_data_impl_get_c_size = sdcard_data_v1_get_c_size;
    } else if (csd_structure == CSD_VERSION_2) {
        sdcard_data->sdcard_data_impl_get_c_size = sdcard_data_v2_get_c_size;
    } else {
        return result_err("Unknown CSD structure.");
    }

    return result_ok();
}

result_t sdcard_data_get_c_size(
        sdcard_data_t *sdcard_data,
        uint32_t *ret_val
) {
    if (sdcard_data == NULL) {
        return result_err("NULL `sdcard_data` passed to sdcard_data_get_c_size().");
    }
    if (ret_val == NULL) {
        return result_err("NULL `ret_val` passed to sdcard_data_get_c_size().");
    }
    return sdcard_data->sdcard_data_impl_get_c_size(&sdcard_data->csd, ret_val);
}

result_t sdcard_data_get_block_size(
        sdcard_data_t *sdcard_data,
        uint16_t *ret_val
) {
    if (sdcard_data == NULL) {
        return result_err("NULL `sdcard_data` passed to sdcard_data_get_block_size().");
    }
    if (ret_val == NULL) {
        return result_err("NULL `ret_val` passed to sdcard_data_get_block_size().");
    }
    uint8_t read_bl_len;
    result_t res = csd_get_read_bl_len(&sdcard_data->csd, &read_bl_len);
    if (result_is_err(res)) {
        return result_err("Failed to get read block length in sdcard_data_get_block_size().");
    }
    uint8_t write_bl_len;
    res = csd_get_write_bl_len(&sdcard_data->csd, &write_bl_len);
    if (result_is_err(res)) {
        return result_err("Failed to get write block length in sdcard_data_get_block_size().");
    }
    if (read_bl_len != write_bl_len) {
        return result_err("Read block length and write block length are not equal in sdcard_data_get_block_size().");
    }
    *ret_val = 1 << read_bl_len;
    return result_ok();
}


