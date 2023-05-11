#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct blk_shared_data_buf blk_shared_data_buf_t;
struct blk_shared_data_buf {
    uintptr_t buf_vaddr;
    size_t buf_size;
};

enum blk_shared_data_buf_result {
    OK_BLK_SHARED_DATA_BUF = 0,
    ERR_NULL_BLK_SHARED_DATA_BUF = -1,
};
typedef enum blk_shared_data_buf_result blk_shared_data_buf_result_t;

/**
 * Initialises a `blk_shared_data_buf_t`.
 * @param shared_data_buf
 * @param buf_vaddr
 * @param buf_size
 * @return
 */
blk_shared_data_buf_result_t blk_shared_data_buf_init(
        blk_shared_data_buf_t *shared_data_buf,
        uintptr_t buf_vaddr,
        size_t buf_size
);

/**
 * Checks if two `blk_shared_data_buf_t`s are equal.
 * @param shared_data_buf_1
 * @param shared_data_buf_2
 * @param ret_val
 * @return
 */
blk_shared_data_buf_result_t blk_shared_data_buf_equals(
        blk_shared_data_buf_t *shared_data_buf_1,
        blk_shared_data_buf_t *shared_data_buf_2,
        bool *ret_val
);
