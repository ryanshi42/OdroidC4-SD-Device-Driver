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
        sdcard_data->sdcard_data_impl_get_memory_capacity = sdcard_data_v1_get_memory_capacity;
        sdcard_data->sdcard_data_impl_get_block_size = sdcard_data_v1_get_block_size;
        sdcard_data->sdcard_data_impl_get_num_blocks = sdcard_data_v1_get_num_blocks;
    } else if (csd_structure == CSD_VERSION_2) {
        sdcard_data->sdcard_data_impl_get_c_size = sdcard_data_v2_get_c_size;
        sdcard_data->sdcard_data_impl_get_memory_capacity = sdcard_data_v2_get_memory_capacity;
        sdcard_data->sdcard_data_impl_get_block_size = sdcard_data_v2_get_block_size;
        sdcard_data->sdcard_data_impl_get_num_blocks = sdcard_data_v2_get_num_blocks;
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
    return sdcard_data->sdcard_data_impl_get_block_size(&sdcard_data->csd, ret_val);
}

result_t sdcard_data_get_memory_capacity(
        sdcard_data_t *sdcard_data,
        uint64_t *ret_val
) {
    if (sdcard_data == NULL) {
        return result_err("NULL `sdcard_data` passed to sdcard_data_get_memory_capacity().");
    }
    if (ret_val == NULL) {
        return result_err("NULL `ret_val` passed to sdcard_data_get_memory_capacity().");
    }
    return sdcard_data->sdcard_data_impl_get_memory_capacity(&sdcard_data->csd, ret_val);
}

result_t sdcard_data_get_num_blocks(
        sdcard_data_t *sdcard_data,
        uint64_t *ret_val
) {
    if (sdcard_data == NULL) {
        return result_err("NULL `sdcard_data` passed to sdcard_data_get_num_blocks().");
    }
    if (ret_val == NULL) {
        return result_err("NULL `ret_val` passed to sdcard_data_get_num_blocks().");
    }
    return sdcard_data->sdcard_data_impl_get_num_blocks(&sdcard_data->csd, ret_val);
}

