#pragma once

#include "result.h"
#include "csd.h"

/**
 * Returns the C_SIZE from the CSD register.
 * @param csd
 * @param ret_val
 * @return
 */
result_t sdcard_data_v2_get_c_size(csd_t *csd, uint32_t *ret_val);

/**
 * Returns the memory capacity of the SD card.
 * @param csd
 * @param ret_val
 * @return
 */
result_t sdcard_data_v2_get_memory_capacity(csd_t *csd, uint64_t *ret_val);
