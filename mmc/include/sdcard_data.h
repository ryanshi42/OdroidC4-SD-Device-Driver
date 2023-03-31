#pragma once

#include "result.h"
#include "csd.h"
#include "sdcard_data_v1.h"
#include "sdcard_data_v2.h"

typedef struct sdcard_data sdcard_data_t;
struct sdcard_data {
    /* Card Specific Data (CSD) Register. */
    csd_t csd;
    /* Function Pointers for different ways to compute values from the CSD.  */
    result_t (*sdcard_data_impl_get_c_size)(csd_t *csd, uint32_t *ret_val);
};

/**
 * Initialize the SDCard Data.
 * @param sdcard_data
 * @param resp0
 * @param resp1
 * @param resp2
 * @param resp3
 * @return
 */
result_t sdcard_data_init(
        sdcard_data_t *sdcard_data,
        uint32_t resp0,
        uint32_t resp1,
        uint32_t resp2,
        uint32_t resp3
);

/**
 * Returns the C_SIZE field the CSD register.
 * @param sdcard_data
 * @param ret_val
 * @return
 */
result_t sdcard_data_get_c_size(
        sdcard_data_t *sdcard_data,
        uint32_t *ret_val
);

/**
 * Returns the SD cards's block size.
 * @param sdcard_data
 * @param ret_val
 * @return
 */
result_t sdcard_data_get_block_size(
        sdcard_data_t *sdcard_data,
        uint16_t *ret_val
);

