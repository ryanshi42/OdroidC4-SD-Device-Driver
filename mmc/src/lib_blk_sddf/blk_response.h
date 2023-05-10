#pragma once

/* A Response. */

#include <stddef.h>

#include "blk_shared_data_buf.h"

enum blk_response_result {
    OK,
    ERR_READ,
    ERR_WRITE,
};
typedef enum blk_response_result blk_response_result_t;

typedef struct blk_response blk_response_t;
struct blk_response {
    blk_response_result_t result; /* The response result. */
    size_t num_blocks; /* Number of blocks of data returned to the client from
    the driver. */
    blk_shared_data_buf_t shared_data_buf; /* The shared data buffer carrying any
    response data. */
};
