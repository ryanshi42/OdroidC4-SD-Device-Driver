#pragma once

#include <stdint.h>

#include "sel4cp_facade.h"
#include "result.h"
#include "bcm_emmc.h"
#include "sd.h"

#include "printf.h"
#include "sleep.h"
#include "assert.h"
#include "time.h"
#include "timer_client.h"
#include "sdcard.h"

#define MMC_DRIVER_TO_SERIAL_CLIENT_PUTCHAR_CHANNEL (7)
#define MMC_DRIVER_TO_TIMER_DRIVER_GET_NUM_TICKS_CHANNEL (11)

