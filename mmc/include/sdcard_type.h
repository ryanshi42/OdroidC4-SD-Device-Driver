#pragma once

#include "sdcard_type.h"
#include "result.h"

/**
 * PUBLIC ENUMERATION OF SD CARD TYPE
 */
enum sdcard_type {
    SD_TYPE_UNKNOWN = 0,
    SD_TYPE_MMC = 1,
    SD_TYPE_1 = 2,
    SD_TYPE_2_SC = 3,
    SD_TYPE_2_HC = 4,
};
typedef enum sdcard_type sdcard_type_t;
