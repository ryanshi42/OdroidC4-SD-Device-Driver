#pragma once

/* A Request. */

#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "blk_shared_data_buf.h"

enum blk_request_operation {
    GET_NUM_BLOCKS,
    GET_BLOCK_SIZE,
    CTRL_SYNC,
    READ,
    WRITE,
};
typedef enum blk_request_operation blk_request_operation_t;

typedef struct blk_request blk_request_t;
struct blk_request {
    /* The MMC driver operation. */
    blk_request_operation_t operation;
    /* Parameters used by Read/Write operations. */
    size_t lba; /* Logical Block Address. */
    size_t num_blocks; /* Number of blocks of data used by the client in its
    request to the driver. */
    blk_shared_data_buf_t shared_data_buf; /* The shared data buffer carrying any
    request data. */
};

enum blk_request_result {
    OK_BLK_REQUEST = 0,
    ERR_NULL_BLK_REQUEST = -1, /* A NULL `blk_request` pointer was passed in. */
    ERR_NULL_BLK_SHARED_DATA_REGION = -2, /* A NULL `blk_shared_data_region` */
};
typedef enum blk_request_result blk_request_result_t;

/**
 * Initialises a `GET_NUM_BLOCKS` request.
 * @param request
 * @param shared_data_buf
 * @return
 */
blk_request_result_t blk_request_init_get_num_blocks(
        blk_request_t *request,
        blk_shared_data_buf_t *shared_data_buf
);

/**
 * Initialises a `GET_BLOCK_SIZE` request.
 * @param request
 * @param shared_data_buf
 * @return
 */
blk_request_result_t blk_request_init_get_block_size(
        blk_request_t *request,
        blk_shared_data_buf_t *shared_data_buf
);

/**
 * Initialises a `CTRL_SYNC` request.
 * @param request
 * @param shared_data_buf
 * @return
 */
blk_request_result_t blk_request_init_ctrl_sync(
        blk_request_t *request,
        blk_shared_data_buf_t *shared_data_buf
);
