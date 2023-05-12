#pragma once

/* A Response. */

#include <stddef.h>
#include <string.h>

#include "blk_shared_data_buf.h"

enum sddf_blk_response_result {
    OK_SDDF_BLK_RESPONSE,
    ERR_SDDF_BLK_RESPONSE,
};
typedef enum sddf_blk_response_result sddf_blk_response_result_t;

typedef struct blk_response blk_response_t;
struct blk_response {
    sddf_blk_response_result_t result; /* The response result. */
    size_t num_blocks; /* Number of blocks of data returned to the client from
    the driver. */
    blk_shared_data_buf_t shared_data_buf; /* The shared data buffer carrying any
    response data. */
};

enum blk_response_result {
    OK_BLK_RESPONSE = 0,
    ERR_NULL_BLK_RESPONSE_IN_BLK_RESPONSE = -1, /* A NULL `blk_response` pointer
    was passed in. */
    ERR_NULL_BLK_SHARED_DATA_REGION_IN_BLK_RESPONSE = -2, /* A NULL
    `blk_shared_data_region` */
    ERR_NULL_RET_VAL_IN_BLK_RESPONSE = -3, /* A NULL `ret_val` */
};
typedef enum blk_response_result blk_response_result_t;

/**
 * Responses initialised by this function indicate a successful `GET_NUM_BLOCKS`
 * request.
 * @param response
 * @param shared_data_buf
 * @return
 */
blk_response_result_t blk_response_init_ok_get_num_blocks(
        blk_response_t *response,
        blk_shared_data_buf_t *shared_data_buf
);

/**
 * Responses initialised by this function indicate an unsuccessful
 * `GET_NUM_BLOCKS` request.
 * @param response
 * @param shared_data_buf
 * @return
 */
blk_response_result_t blk_response_init_error_get_num_blocks(
        blk_response_t *response,
        blk_shared_data_buf_t *shared_data_buf
);

/**
 * Returns True if the response is ok and False otherwise.
 * @param response
 * @param ret_val
 * @return
 */
blk_response_result_t blk_response_is_ok(
        blk_response_t *response,
        bool *ret_val
);




