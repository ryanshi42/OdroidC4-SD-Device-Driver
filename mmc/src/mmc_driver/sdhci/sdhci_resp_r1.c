#include "sdhci_resp_r1.h"

result_t sdhci_resp_r1_has_error(uint32_t resp, bool *ret_val) {
    if (ret_val == NULL) {
        return result_err("NULL `ret_val` passed to sdhci_resp_r1_has_error().");
    }
    *ret_val = resp & R1_ERRORS_MASK;
    return result_ok();
}
