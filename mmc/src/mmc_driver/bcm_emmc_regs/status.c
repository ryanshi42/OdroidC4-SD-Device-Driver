#include "status.h"

result_t status_get_cmd_inhibit(status_t *status, bool *ret_val) {
    if (status == NULL) {
        return result_err("NULL `status` passed to status_get_cmd_inhibit().");
    }
    *ret_val = status->CMD_INHIBIT;
    return result_ok();
}

result_t status_get_dat_inhibit(status_t *status, bool *ret_val) {
    if (status == NULL) {
        return result_err("NULL `status` passed to status_get_dat_inhibit().");
    }
    *ret_val = status->DAT_INHIBIT;
    return result_ok();
}
