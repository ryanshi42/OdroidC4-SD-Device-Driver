#pragma once

#include <stdint.h>

#include "cmdtm.h"

/*--------------------------------------------------------------------------}
{						   SD CARD COMMAND RECORD						    }
{--------------------------------------------------------------------------*/
typedef struct EMMCCommand {
    const char cmd_name[16];
    cmdtm_t cmdtm; /* Populated with values to put into the command register. */
    bool use_rca; /* Whether the command uses rca or not. */
    uint16_t delay; /* Delay to apply after command */
//    bool is_app_cmd; /* Whether the command is an application command. */
} EMMCCommand;


