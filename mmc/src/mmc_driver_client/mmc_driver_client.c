#include "mmc_driver_client.h"

result_t mmc_driver_client_init(
        mmc_driver_client_t *mmc_driver_client,
        size_t sel4cp_channel_id_request,
        blk_request_queue_t *request_queue,
        blk_response_queue_t *response_queue,
        blk_shared_data_queue_t *shared_data_queue
) {
    if (mmc_driver_client == NULL) {
        return result_err("NULL `mmc_driver_client` passed to mmc_driver_client_init().");
    }
    if (request_queue == NULL) {
        return result_err("NULL `request_queue` passed to mmc_driver_client_init().");
    }
    if (response_queue == NULL) {
        return result_err("NULL `response_queue` passed to mmc_driver_client_init().");
    }
    if (shared_data_queue == NULL) {
        return result_err("NULL `shared_data_queue` passed to mmc_driver_client_init().");
    }
    /* Save all variables passed into this initialiser. */
    mmc_driver_client->sel4cp_channel_id_request = sel4cp_channel_id_request;
    mmc_driver_client->request_queue = request_queue;
    mmc_driver_client->response_queue = response_queue;
    if (blk_shared_data_queue_copy(
            &mmc_driver_client->shared_data_queue,
            shared_data_queue
    ) != OK_BLK_SHARED_DATA_QUEUE) {
        return result_err("Failed to copy `shared_data_queue` to `mmc_driver_client->shared_data_queue`.");
    }
    return result_ok();
}

result_t mmc_driver_client_get_num_blocks(
        mmc_driver_client_t *mmc_driver_client,
        uint64_t *ret_val
) {
    if (mmc_driver_client == NULL) {
        return result_err("NULL `mmc_driver_client` passed to mmc_driver_client_get_sector_count().");
    }
    if (ret_val == NULL) {
        return result_err("NULL `ret_val` passed to mmc_driver_client_get_sector_count().");
    }
    /* Dequeue a Shared Data Buffer from the Shared Data Buffer queue. */
    blk_shared_data_buf_t shared_data_buf = {0};
    if (blk_shared_data_queue_dequeue(
            &mmc_driver_client->shared_data_queue,
            &shared_data_buf
    ) != OK_BLK_SHARED_DATA_QUEUE) {
        return result_err("Failed to dequeue a Shared Data Buffer from the Shared Data Buffer queue.");
    }
    /* Initialise a Request for enqueuing onto the Request queue.. */
    blk_request_t request = {0};
    if (blk_request_init_get_num_blocks(
            &request,
            &shared_data_buf
    ) != OK_BLK_REQUEST) {
        return result_err("Failed to initialise a request for enqueuing onto the Request queue.");
    }
    /* Enqueue our Request onto the Request queue. */
    if (blk_request_queue_enqueue(
            mmc_driver_client->request_queue,
            &request
    ) != OK_BLK_REQUEST_QUEUE) {
        return result_err("Failed to enqueue our request onto the request queue.");
    }
    /* Notify `mmc_driver` of our new request. */
    sel4cp_notify(mmc_driver_client->sel4cp_channel_id_request);


    return result_ok();
}
