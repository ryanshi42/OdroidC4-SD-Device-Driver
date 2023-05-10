#pragma once

/* Request Buffer. */

#include <stddef.h>
#include <stdint.h>

#include "blk_shared_data_buf.h"

enum blk_req_operation {
    READ,
    WRITE,
};
typedef enum blk_req_operation blk_req_operation_t;

typedef struct blk_req_buf blk_req_buf_t;
struct blk_req_buf {
    /* The MMC driver operation. */
    blk_req_operation_t operation;
    /* Parameters used by Read/Write operations. */
    size_t lba; /* Logical Block Address. */
    size_t num_blocks; /* Number of blocks of data used by the client in its
    request to the driver. */
    blk_shared_data_buf_t shared_data_buf; /* The shared data buffer carrying any
    request data. */
};



