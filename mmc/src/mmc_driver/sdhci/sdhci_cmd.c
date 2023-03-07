#include "sdhci_cmd.h"

result_t sdhci_cmd_get_cmdtm(
        sdhci_cmd_t *sdhci_cmd,
        cmdtm_t *ret_val
) {
    if (sdhci_cmd == NULL) {
        return result_err("NULL `sdhci_cmd` passed to sdhci_cmd_get_cmdtm().");
    }
    if (ret_val == NULL) {
        return result_err("NULL `ret_val` passed to sdhci_cmd_get_cmdtm().");
    }
    *ret_val = sdhci_cmd->cmdtm;
    return result_ok();
}

result_t sdhci_cmd_get_delay(
        sdhci_cmd_t *sdhci_cmd,
        size_t *ret_val
) {
    if (sdhci_cmd == NULL) {
        return result_err("NULL `sdhci_cmd` passed to sdhci_cmd_get_delay().");
    }
    if (ret_val == NULL) {
        return result_err("NULL `ret_val` passed to sdhci_cmd_get_delay().");
    }
    *ret_val = sdhci_cmd->delay;
    return result_ok();
}

result_t sdhci_cmd_get_cmd_rspns_type(
        sdhci_cmd_t *sdhci_cmd,
        cmd_rspns_type_t *ret_val
) {
    if (sdhci_cmd == NULL) {
        return result_err("NULL `sdhci_cmd` passed to sdhci_cmd_get_cmd_rspns_type().");
    }
    return cmdtm_get_cmd_rspns_type(&sdhci_cmd->cmdtm, ret_val);
}

result_t sdhci_cmd_get_cmd_index(
        sdhci_cmd_t *sdhci_cmd,
        cmd_index_t *ret_val
) {
    if (sdhci_cmd == NULL) {
        return result_err("NULL `sdhci_cmd` passed to sdhci_cmd_get_cmd_index().");
    }
    return cmdtm_get_cmd_index(&sdhci_cmd->cmdtm, ret_val);
}

