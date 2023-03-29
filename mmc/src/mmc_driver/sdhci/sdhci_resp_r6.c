#include "sdhci_resp_r6.h"

result_t sdhci_resp_r6_get_rca(uint32_t resp, uint32_t *ret_val) {
    if (ret_val == NULL) {
        return result_err("NULL `ret_val` passed to sdhci_resp_r6_get_rca().");
    }
    *ret_val = resp & 0xffff0000; // RCA[31:16] of response
    return result_ok();
}

result_t sdhci_resp_r6_get_status(uint32_t resp, uint32_t *ret_val) {
    if (ret_val == NULL) {
        return result_err("NULL `ret_val` passed to sdhci_resp_r6_get_status().");
    }
    *ret_val = ((resp & 0x00001fff)) |       // 12:0 map directly to status 12:0
               ((resp & 0x00002000) << 6) |  // 13 maps to status 19 ERROR
               ((resp & 0x00004000) << 8) |  // 14 maps to status 22 ILLEGAL_COMMAND
               ((resp & 0x00008000) << 8);
    return result_ok();
}
