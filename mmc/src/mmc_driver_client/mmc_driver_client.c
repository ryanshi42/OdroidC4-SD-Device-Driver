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
    /* Save all variables passed into this constructor. */
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
        return result_err("NULL `mmc_driver_client` passed to mmc_driver_client_get_num_blocks().");
    }
    if (ret_val == NULL) {
        return result_err("NULL `ret_val` passed to mmc_driver_client_get_num_blocks().");
    }
    /* Dequeue a Shared Data Buffer from the Shared Data Buffer queue. */
    blk_shared_data_buf_t shared_data_buf = {0};
    if (blk_shared_data_queue_dequeue(
            &mmc_driver_client->shared_data_queue,
            &shared_data_buf
    ) != OK_BLK_SHARED_DATA_QUEUE) {
        return result_err("Failed to dequeue a Shared Data Buffer from the Shared Data Buffer queue.");
    }
    /* Initialise a Request for enqueuing onto the Request queue. */
    blk_request_t request = {0};
    if (blk_request_init_get_num_blocks(
            &request,
            &shared_data_buf
    ) != OK_BLK_REQUEST) {
        return result_err("Failed to initialise a Request for enqueuing onto the Request queue.");
    }
    /* Enqueue our Request onto the Request queue. */
    if (blk_request_queue_enqueue(
            mmc_driver_client->request_queue,
            &request
    ) != OK_BLK_REQUEST_QUEUE) {
        return result_err("Failed to enqueue our Request onto the Request queue.");
    }
    /* Notify `mmc_driver` of our new Request. */
    sel4cp_notify(mmc_driver_client->sel4cp_channel_id_request);

    /* Keep trying to dequeue from the Response queue for a Response */
    blk_response_t response = {0};
    while (blk_response_queue_dequeue(
            mmc_driver_client->response_queue,
            &response
    ) != OK_BLK_RESPONSE_QUEUE) {}
    /* Check if the Response is success or failure. */
    bool is_response_ok = false;
    if (blk_response_is_ok(
            &response,
            &is_response_ok
    ) != OK_BLK_RESPONSE) {
        return result_err("Failed to check if the Response is success or failure in mmc_driver_client_get_num_blocks().");
    }
    /* Get the virtual address of the Shared Data buffer. */
    uintptr_t buf_vaddr = 0;
    if (blk_shared_data_buf_get_buf_vaddr(
            &request.shared_data_buf,
            &buf_vaddr
    ) != OK_BLK_SHARED_DATA_BUF) {
        return result_err("Failed to get virtual address of shared data buffer in mmc_driver_client_get_num_blocks().");
    }
    /* Enqueue the Shared Data buffer back onto the Shared Data buffer queue for
     * reuse by other Requests. */
    if (blk_shared_data_queue_enqueue(
            &mmc_driver_client->shared_data_queue,
            &shared_data_buf
    ) != OK_BLK_SHARED_DATA_QUEUE) {
        return result_err("Failed to enqueue the Shared Data buffer back onto the Shared Data buffer queue.");
    }
    /* If the Response was a success, get the number of blocks from the Shared
     * Data buffer. Otherwise, return an error. */
    if (is_response_ok) {
        *ret_val = *((uint64_t *) buf_vaddr);
    } else {
        return result_err("Response was not OK in mmc_driver_client_get_num_blocks().");
    }
    return result_ok();
}

result_t mmc_driver_client_get_block_size(
        mmc_driver_client_t *mmc_driver_client,
        uint16_t *ret_val
) {
    if (mmc_driver_client == NULL) {
        return result_err("NULL `mmc_driver_client` passed to mmc_driver_client_get_block_size().");
    }
    if (ret_val == NULL) {
        return result_err("NULL `ret_val` passed to mmc_driver_client_get_block_size().");
    }
    /* Dequeue a Shared Data Buffer from the Shared Data Buffer queue. */
    blk_shared_data_buf_t shared_data_buf = {0};
    if (blk_shared_data_queue_dequeue(
            &mmc_driver_client->shared_data_queue,
            &shared_data_buf
    ) != OK_BLK_SHARED_DATA_QUEUE) {
        return result_err("Failed to dequeue a Shared Data Buffer from the Shared Data Buffer queue.");
    }
    /* Initialise a Request for enqueuing onto the Request queue. */
    blk_request_t request = {0};
    if (blk_request_init_get_block_size(
            &request,
            &shared_data_buf
    ) != OK_BLK_REQUEST) {
        return result_err("Failed to initialise a Request for enqueuing onto the Request queue.");
    }
    /* Enqueue our Request onto the Request queue. */
    if (blk_request_queue_enqueue(
            mmc_driver_client->request_queue,
            &request
    ) != OK_BLK_REQUEST_QUEUE) {
        return result_err("Failed to enqueue our Request onto the Request queue.");
    }
    /* Notify `mmc_driver` of our new Request. */
    sel4cp_notify(mmc_driver_client->sel4cp_channel_id_request);

    /* Keep trying to dequeue from the Response queue for a Response */
    blk_response_t response = {0};
    while (blk_response_queue_dequeue(
            mmc_driver_client->response_queue,
            &response
    ) != OK_BLK_RESPONSE_QUEUE) {}
    /* Check if the Response is success or failure. */
    bool is_response_ok = false;
    if (blk_response_is_ok(
            &response,
            &is_response_ok
    ) != OK_BLK_RESPONSE) {
        return result_err("Failed to check if the Response is success or failure in mmc_driver_client_get_block_size().");
    }
    /* Get the virtual address of the Shared Data buffer. */
    uintptr_t buf_vaddr = 0;
    if (blk_shared_data_buf_get_buf_vaddr(
            &request.shared_data_buf,
            &buf_vaddr
    ) != OK_BLK_SHARED_DATA_BUF) {
        return result_err("Failed to get virtual address of shared data buffer in mmc_driver_client_get_block_size().");
    }
    /* Enqueue the Shared Data buffer back onto the Shared Data buffer queue for
     * reuse by other Requests. */
    if (blk_shared_data_queue_enqueue(
            &mmc_driver_client->shared_data_queue,
            &shared_data_buf
    ) != OK_BLK_SHARED_DATA_QUEUE) {
        return result_err("Failed to enqueue the Shared Data buffer back onto the Shared Data buffer queue.");
    }
    /* If the Response was a success, get the number of blocks from the Shared
     * Data buffer. Otherwise, return an error. */
    if (is_response_ok) {
        *ret_val = *((uint16_t *) buf_vaddr);
    } else {
        return result_err("Response was not OK in mmc_driver_client_get_block_size().");
    }
    return result_ok();
}

result_t mmc_driver_client_read(
        mmc_driver_client_t *mmc_driver_client,
        size_t lba,
        size_t num_blocks,
        size_t block_size,
        char *dst_buf
) {
    if (mmc_driver_client == NULL) {
        return result_err("NULL `mmc_driver_client` passed to mmc_driver_client_read().");
    }
    if (dst_buf == NULL) {
        return result_err("NULL `dst_buf` passed to mmc_driver_client_read().");
    }
    /* Dequeue a Shared Data Buffer from the Shared Data Buffer queue. */
    blk_shared_data_buf_t shared_data_buf = {0};
    if (blk_shared_data_queue_dequeue(
            &mmc_driver_client->shared_data_queue,
            &shared_data_buf
    ) != OK_BLK_SHARED_DATA_QUEUE) {
        return result_err("Failed to dequeue a Shared Data Buffer from the Shared Data Buffer queue.");
    }
    /* Initialise a Request for enqueuing onto the Request queue. */
    blk_request_t request = {0};
    if (blk_request_init_read(
            &request,
            &shared_data_buf,
            lba,
            num_blocks
    ) != OK_BLK_REQUEST) {
        return result_err("Failed to initialise a Request for enqueuing onto the Request queue.");
    }
    /* Enqueue our Request onto the Request queue. */
    if (blk_request_queue_enqueue(
            mmc_driver_client->request_queue,
            &request
    ) != OK_BLK_REQUEST_QUEUE) {
        return result_err("Failed to enqueue our Request onto the Request queue.");
    }
    /* Notify `mmc_driver` of our new Request. */
    sel4cp_notify(mmc_driver_client->sel4cp_channel_id_request);

    /* Keep trying to dequeue from the Response queue for a Response */
    blk_response_t response = {0};
    while (blk_response_queue_dequeue(
            mmc_driver_client->response_queue,
            &response
    ) != OK_BLK_RESPONSE_QUEUE) {}
    /* Check if the Response is success or failure. */
    bool is_response_ok = false;
    if (blk_response_is_ok(
            &response,
            &is_response_ok
    ) != OK_BLK_RESPONSE) {
        return result_err("Failed to check if the Response is success or failure in mmc_driver_client_read().");
    }
    /* Get the virtual address of the Shared Data buffer. */
    uintptr_t buf_vaddr = 0;
    if (blk_shared_data_buf_get_buf_vaddr(
            &request.shared_data_buf,
            &buf_vaddr
    ) != OK_BLK_SHARED_DATA_BUF) {
        return result_err("Failed to get virtual address of shared data buffer in mmc_driver_client_read().");
    }
    /* Enqueue the Shared Data buffer back onto the Shared Data buffer queue for
     * reuse by other Requests. */
    if (blk_shared_data_queue_enqueue(
            &mmc_driver_client->shared_data_queue,
            &shared_data_buf
    ) != OK_BLK_SHARED_DATA_QUEUE) {
        return result_err("Failed to enqueue the Shared Data buffer back onto the Shared Data buffer queue.");
    }
    /* If the Response was a success, get the number of blocks from the Shared
     * Data buffer. Otherwise, return an error. */
    if (is_response_ok) {
        /* Copy data from the Shared Data buffer into the user's buffer. */
        memcpy(dst_buf, (char *) buf_vaddr, num_blocks * block_size);
    } else {
        return result_err("Response was not OK in mmc_driver_client_read().");
    }
    return result_ok();
}

result_t mmc_driver_client_write(
        mmc_driver_client_t *mmc_driver_client,
        size_t lba,
        size_t num_blocks,
        size_t block_size,
        char *src_buf
) {
    if (mmc_driver_client == NULL) {
        return result_err("NULL `mmc_driver_client` passed to mmc_driver_client_write().");
    }
    if (src_buf == NULL) {
        return result_err("NULL `src_buf` passed to mmc_driver_client_write().");
    }
    /* Dequeue a Shared Data Buffer from the Shared Data Buffer queue. */
    blk_shared_data_buf_t shared_data_buf = {0};
    if (blk_shared_data_queue_dequeue(
            &mmc_driver_client->shared_data_queue,
            &shared_data_buf
    ) != OK_BLK_SHARED_DATA_QUEUE) {
        return result_err("Failed to dequeue a Shared Data Buffer from the Shared Data Buffer queue.");
    }
    /* Copy data from user's buffer into the Shared Data buffer. */
    if (blk_shared_data_buf_insert_data(
            &shared_data_buf,
            src_buf,
            num_blocks * block_size
    ) != OK_BLK_SHARED_DATA_BUF) {
        return result_err("Failed to insert data into the Shared Data Buffer.");
    }
    /* Initialise a Request for enqueuing onto the Request queue. */
    blk_request_t request = {0};
    if (blk_request_init_write(
            &request,
            &shared_data_buf,
            lba,
            num_blocks
    ) != OK_BLK_REQUEST) {
        return result_err("Failed to initialise a Request for enqueuing onto the Request queue.");
    }
    /* Enqueue our Request onto the Request queue. */
    if (blk_request_queue_enqueue(
            mmc_driver_client->request_queue,
            &request
    ) != OK_BLK_REQUEST_QUEUE) {
        return result_err("Failed to enqueue our Request onto the Request queue.");
    }
    /* Notify `mmc_driver` of our new Request. */
    sel4cp_notify(mmc_driver_client->sel4cp_channel_id_request);

    /* Keep trying to dequeue from the Response queue for a Response */
    blk_response_t response = {0};
    while (blk_response_queue_dequeue(
            mmc_driver_client->response_queue,
            &response
    ) != OK_BLK_RESPONSE_QUEUE) {}
    /* Check if the Response is success or failure. */
    bool is_response_ok = false;
    if (blk_response_is_ok(
            &response,
            &is_response_ok
    ) != OK_BLK_RESPONSE) {
        return result_err("Failed to check if the Response is success or failure in mmc_driver_client_write().");
    }
    /* Get the virtual address of the Shared Data buffer. */
    uintptr_t buf_vaddr = 0;
    if (blk_shared_data_buf_get_buf_vaddr(
            &request.shared_data_buf,
            &buf_vaddr
    ) != OK_BLK_SHARED_DATA_BUF) {
        return result_err("Failed to get virtual address of shared data buffer in mmc_driver_client_write().");
    }
    /* Enqueue the Shared Data buffer back onto the Shared Data buffer queue for
     * reuse by other Requests. */
    if (blk_shared_data_queue_enqueue(
            &mmc_driver_client->shared_data_queue,
            &shared_data_buf
    ) != OK_BLK_SHARED_DATA_QUEUE) {
        return result_err("Failed to enqueue the Shared Data buffer back onto the Shared Data buffer queue.");
    }
    /* If the Response was a success, get the number of blocks from the Shared
     * Data buffer. Otherwise, return an error. */
    if (is_response_ok) {
        /* TODO: Clean this up. */
//        memcpy(ret_val, (char *) buf_vaddr, num_blocks * block_size);
    } else {
        return result_err("Response was not OK in mmc_driver_client_write().");
    }
    return result_ok();
}
