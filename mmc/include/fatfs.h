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
#include "memory_facade.h"

#define FATFS_TO_SERIAL_CLIENT_PUTCHAR_CHANNEL (13)
#define FATFS_TO_MMC_DRIVER_REQUEST_CHANNEL (15)

/* The largest possible cluster size for FAT file systems is 256KB. */
#define MAX_FAT_CLUSTER_SIZE (0x40000)

/**
 * Obtains the size of a FAT cluster in bytes.
 * @param mmc_driver_client
 * @param ret_val
 * @return
 */
result_t fatfs_get_cluster_size_in_bytes(
        mmc_driver_client_t *mmc_driver_client,
        size_t *ret_val
);
