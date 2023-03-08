#include "cmdtm.h"

result_t cmdtm_get_cmd_rspns_type(
        cmdtm_t *cmdtm,
        cmd_rspns_type_t *ret_val
) {
    if (cmdtm == NULL) {
        return result_err("NULL `cmdtm` passed to cmdtm_get_cmd_rspns_type().");
    }
    if (ret_val == NULL) {
        return result_err("NULL `ret_val` passed to cmdtm_get_cmd_rspns_type().");
    }
    *ret_val = cmdtm->CMD_RSPNS_TYPE;
    return result_ok();
}

result_t cmdtm_get_cmd_index(
        cmdtm_t *cmdtm,
        cmd_index_t *ret_val
) {
    if (cmdtm == NULL) {
        return result_err("NULL `cmdtm` passed to cmdtm_get_cmd_index().");
    }
    if (ret_val == NULL) {
        return result_err("NULL `ret_val` passed to cmdtm_get_cmd_index().");
    }
    *ret_val = cmdtm->CMD_INDEX;
    return result_ok();
}

