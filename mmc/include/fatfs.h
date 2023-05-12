#pragma once

#include "sel4cp_facade.h"
#include "printf.h"
#include "log.h"
#include "result.h"
#include "blk_request_queue.h"
#include "blk_response_queue.h"
#include "blk_shared_data_queue.h"
#include "fatfs_e2e.h"
#include "diskio.h"
#include "mmc_driver_client.h"

#define FATFS_TO_SERIAL_CLIENT_PUTCHAR_CHANNEL (13)
#define FATFS_TO_MMC_DRIVER_REQUEST_CHANNEL (15)

#define FAT_CLUSTER_SIZE (0x10000)

