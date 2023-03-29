#pragma once

#include "result.h"

/**
 * Returns RCA from R6 Response.
 * @param[in] resp
 * @param[out] ret_val
 * @return
 */
result_t sdhci_resp_r6_get_rca(uint32_t resp, uint32_t *ret_val);

/**
 * Returns Status from R6 Response.
 * @param[in] resp
 * @param[out] ret_val
 * @return
 */
result_t sdhci_resp_r6_get_status(uint32_t resp, uint32_t *ret_val);
