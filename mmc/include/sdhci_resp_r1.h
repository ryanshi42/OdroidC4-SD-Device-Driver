#pragma once

#include "result.h"

#define R1_ERRORS_MASK       0xfff9c004

/**
 * Returns True if R1 Response has Error and False otherwise.
 * @param[in] resp
 * @param[out] ret_val
 * @return
 */
result_t sdhci_resp_r1_has_error(uint32_t resp, bool *ret_val);



