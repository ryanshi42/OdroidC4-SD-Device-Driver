#include "blk_response.h"

static inline blk_response_result_t blk_response_init_no_args(
        blk_response_t *response,
        blk_shared_data_buf_t *shared_data_buf,
        sddf_blk_response_result_t result
);

static inline blk_response_result_t blk_response_init_no_args(
        blk_response_t *response,
        blk_shared_data_buf_t *shared_data_buf,
        sddf_blk_response_result_t result
) {
    if (response == NULL) {
        return ERR_NULL_BLK_RESPONSE_IN_BLK_RESPONSE;
    }
    if (shared_data_buf == NULL) {
        return ERR_NULL_BLK_SHARED_DATA_REGION_IN_BLK_RESPONSE;
    }
    memset(response, 0, sizeof(*response));
    response->result = result;
    response->shared_data_buf = *shared_data_buf;
    return OK_BLK_RESPONSE;
}

blk_response_result_t blk_response_init_ok_get_num_blocks(
        blk_response_t *response,
        blk_shared_data_buf_t *shared_data_buf
) {
    return blk_response_init_no_args(
            response,
            shared_data_buf,
            OK_SDDF_BLK_RESPONSE
    );
}

blk_response_result_t blk_response_init_error_get_num_blocks(
        blk_response_t *response,
        blk_shared_data_buf_t *shared_data_buf
) {
    return blk_response_init_no_args(
            response,
            shared_data_buf,
            ERR_SDDF_BLK_RESPONSE
    );
}

blk_response_result_t blk_response_is_ok(
        blk_response_t *response,
        bool *ret_val
) {
    if (response == NULL) {
        return ERR_NULL_BLK_RESPONSE_IN_BLK_RESPONSE;
    }
    if (ret_val == NULL) {
        return ERR_NULL_RET_VAL_IN_BLK_RESPONSE;
    }
    *ret_val = (response->result == OK_SDDF_BLK_RESPONSE);
    return OK_BLK_RESPONSE;
}
