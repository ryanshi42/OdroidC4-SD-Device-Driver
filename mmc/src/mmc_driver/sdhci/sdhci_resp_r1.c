#include "sdhci_resp_r1.h"

result_t sdhci_resp_r1_has_error(uint32_t resp, bool *ret_val) {
    *ret_val = resp & R1_ERRORS_MASK;
    return result_ok();
}
