#pragma once

#include <stdint.h>

#include "sel4cp_facade.h"
#include "result.h"
#include "bcm_emmc.h"

#include "log.h"
#include "printf.h"
#include "timer_client.h"
#include "sdcard.h"
#include "e2e_test.h"

#define MMC_DRIVER_TO_SERIAL_CLIENT_PUTCHAR_CHANNEL (7)
#define MMC_DRIVER_TO_TIMER_DRIVER_GET_NUM_TICKS_CHANNEL (11)

