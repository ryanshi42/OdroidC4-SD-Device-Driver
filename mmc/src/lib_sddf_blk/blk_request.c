#include "blk_request.h"

/**
 * Initialises a request that takes no arguments.
 * @param request
 * @param shared_data_buf
 * @param operation
 * @return
 */
static inline blk_request_result_t blk_request_init_no_args(
        blk_request_t *request,
        blk_shared_data_buf_t *shared_data_buf,
        blk_request_operation_t operation
);

static inline blk_request_result_t blk_request_init_no_args(
        blk_request_t *request,
        blk_shared_data_buf_t *shared_data_buf,
        blk_request_operation_t operation
) {
    if (request == NULL) {
        return ERR_NULL_BLK_REQUEST;
    }
    if (shared_data_buf == NULL) {
        return ERR_NULL_BLK_SHARED_DATA_REGION;
    }
    memset(request, 0, sizeof(*request));
    request->operation = operation;
    request->shared_data_buf = *shared_data_buf;
    return OK_BLK_REQUEST;
}

blk_request_result_t blk_request_init_get_sector_count(
        blk_request_t *request,
        blk_shared_data_buf_t *shared_data_buf
) {
    return blk_request_init_no_args(
            request,
            shared_data_buf,
            GET_SECTOR_COUNT
    );
}

blk_request_result_t blk_request_init_get_sector_size(
        blk_request_t *request,
        blk_shared_data_buf_t *shared_data_buf
) {
    return blk_request_init_no_args(
            request,
            shared_data_buf,
            GET_SECTOR_SIZE
    );
}

blk_request_result_t blk_request_init_get_block_size(
        blk_request_t *request,
        blk_shared_data_buf_t *shared_data_buf
) {
    return blk_request_init_no_args(
            request,
            shared_data_buf,
            GET_BLOCK_SIZE
    );
}

blk_request_result_t blk_request_init_ctrl_sync(
        blk_request_t *request,
        blk_shared_data_buf_t *shared_data_buf
) {
    return blk_request_init_no_args(
            request,
            shared_data_buf,
            CTRL_SYNC
    );
}
