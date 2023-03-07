#pragma once

#include <stdint.h>

#include "cmdtm.h"
#include "result.h"
#include "cmd_rspns_type.h"
#include "cmd_index.h"

/*--------------------------------------------------------------------------}
{						   SD CARD COMMAND RECORD						    }
{--------------------------------------------------------------------------*/
typedef struct sdhci_cmd sdhci_cmd_t;
struct sdhci_cmd {
    const char cmd_name[16];
    cmdtm_t cmdtm; /* Populated with values to put into the command register. */
    bool use_rca; /* Whether the command uses rca or not. */
    size_t delay; /* Delay in microseconds to apply after command */
//    bool is_app_cmd; /* Whether the command is an application command. */
};

/**
 * Gets the cmdtm register from the sdhci_cmd_t structure.
 * @param sdhci_cmd
 * @param ret_val
 * @return
 */
result_t sdhci_cmd_get_cmdtm(
        sdhci_cmd_t *sdhci_cmd,
        cmdtm_t *ret_val
);

/**
 * Gets the delay from the sdhci_cmd_t structure.
 * @param sdhci_cmd
 * @param ret_val
 * @return
 */
result_t sdhci_cmd_get_delay(
        sdhci_cmd_t *sdhci_cmd,
        size_t *ret_val
);

/**
 * Gets the command response type from the sdhci_cmd_t structure.
 * @param sdhci_cmd
 * @param ret_val
 * @return
 */
result_t sdhci_cmd_get_cmd_rspns_type(
        sdhci_cmd_t *sdhci_cmd,
        cmd_rspns_type_t *ret_val
);

/**
 * Returns the command index.
 * @param sdhci_cmd
 * @param ret_val
 * @return
 */
result_t sdhci_cmd_get_cmd_index(
        sdhci_cmd_t *sdhci_cmd,
        cmd_index_t *ret_val
);
