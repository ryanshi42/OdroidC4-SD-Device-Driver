#pragma once

#include <stddef.h>

#include "sel4cp_facade.h"
#include "blk_request_queue.h"
#include "blk_response_queue.h"
#include "blk_shared_data_queue.h"
#include "blk_request.h"
#include "blk_response.h"
#include "blk_shared_data_buf.h"
#include "result.h"

typedef struct mmc_driver_client mmc_driver_client_t;
struct mmc_driver_client {
    /* sel4cp channel ID to send Request notifications to `mmc_driver`. */
    size_t sel4cp_channel_id_request;
    blk_request_queue_t *request_queue;
    blk_response_queue_t *response_queue;
    blk_shared_data_queue_t shared_data_queue;
};

/**
 * Initialise our `mmc_driver_client`.
 * @param mmc_driver_client
 * @param sel4cp_channel_id_request
 * @param request_queue
 * @param response_queue
 * @param shared_data_queue
 * @return
 */
result_t mmc_driver_client_init(
        mmc_driver_client_t *mmc_driver_client,
        size_t sel4cp_channel_id_request,
        blk_request_queue_t *request_queue,
        blk_response_queue_t *response_queue,
        blk_shared_data_queue_t *shared_data_queue
);

/**
 * Services the `GET_SECTOR_COUNT` FatFs request.
 * @param mmc_driver_client
 * @param ret_val
 * @return
 */
result_t mmc_driver_client_get_num_blocks(
        mmc_driver_client_t *mmc_driver_client,
        uint64_t *ret_val
);

