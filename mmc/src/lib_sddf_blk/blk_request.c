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
        return ERR_NULL_BLK_SHARED_DATA_REGION_IN_BLK_REQUEST;
    }
    memset(request, 0, sizeof(*request));
    request->operation = operation;
    request->shared_data_buf = *shared_data_buf;
    return OK_BLK_REQUEST;
}

/**
 * Initialises an I/O request that takes an LBA and number of blocks.
 * @param request
 * @param shared_data_buf
 * @param operation
 * @param lba
 * @param num_blocks
 * @return
 */
static inline blk_request_result_t blk_request_init_io(
        blk_request_t *request,
        blk_shared_data_buf_t *shared_data_buf,
        blk_request_operation_t operation,
        size_t lba,
        size_t num_blocks
);

static inline blk_request_result_t blk_request_init_io(
        blk_request_t *request,
        blk_shared_data_buf_t *shared_data_buf,
        blk_request_operation_t operation,
        size_t lba,
        size_t num_blocks
) {
    if (request == NULL) {
        return ERR_NULL_BLK_REQUEST;
    }
    if (shared_data_buf == NULL) {
        return ERR_NULL_BLK_SHARED_DATA_REGION_IN_BLK_REQUEST;
    }
    memset(request, 0, sizeof(*request));
    request->operation = operation;
    request->shared_data_buf = *shared_data_buf;
    request->lba = lba;
    request->num_blocks = num_blocks;
    return OK_BLK_REQUEST;
}

blk_request_result_t blk_request_init_get_num_blocks(
        blk_request_t *request,
        blk_shared_data_buf_t *shared_data_buf
) {
    return blk_request_init_no_args(
            request,
            shared_data_buf,
            GET_NUM_BLOCKS
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

blk_request_result_t blk_request_init_read(
        blk_request_t *request,
        blk_shared_data_buf_t *shared_data_buf,
        size_t lba,
        size_t num_blocks
) {
    return blk_request_init_io(
            request,
            shared_data_buf,
            READ,
            lba,
            num_blocks
    );
}

blk_request_result_t blk_request_init_write(
        blk_request_t *request,
        blk_shared_data_buf_t *shared_data_buf,
        size_t lba,
        size_t num_blocks
) {
    return blk_request_init_io(
            request,
            shared_data_buf,
            WRITE,
            lba,
            num_blocks
    );
}

