#pragma once

/* A Request. */

#include <stddef.h>
#include <stdint.h>

#include "blk_shared_data_buf.h"

enum blk_request_operation {
    GET_SECTOR_COUNT,
    GET_SECTOR_SIZE,
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



