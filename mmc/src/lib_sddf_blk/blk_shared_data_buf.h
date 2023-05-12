#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

typedef struct blk_shared_data_buf blk_shared_data_buf_t;
struct blk_shared_data_buf {
    uintptr_t buf_vaddr;
    size_t buf_size;
};

enum blk_shared_data_buf_result {
    OK_BLK_SHARED_DATA_BUF = 0,
    ERR_NULL_BLK_SHARED_DATA_BUF = -1,
    ERR_BLK_SHARED_DATA_BUF_DATA_TOO_LARGE = -2,
    ERR_NULL_RET_VAL_IN_BLK_SHARED_DATA_BUF = -3,
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

/**
 * Inserts data into the memory region represented by a `shared_data_buf`.
 * @param shared_data_buf
 * @param data The data you wish to insert into `shared_data_buf`.
 * @param data_size The size of the data you wish to insert into
 * `shared_data_buf` in bytes.
 * @return
 */
blk_shared_data_buf_result_t blk_shared_data_buf_insert_data(
        blk_shared_data_buf_t *shared_data_buf,
        void *data,
        size_t data_size
);

/**
 * Gets the size of the memory region represented by a `shared_data_buf`.
 * @param shared_data_buf
 * @param ret_val Size of the data you wish to insert into `shared_data_buf` in
 * bytes.
 * @return
 */
blk_shared_data_buf_result_t blk_shared_data_buf_get_buf_size(
        blk_shared_data_buf_t *shared_data_buf,
        size_t *ret_val
);

/**
 * Gets the virtual address of the memory region represented by a
 * `shared_data_buf`.
 * @param shared_data_buf
 * @param ret_val
 * @return
 */
blk_shared_data_buf_result_t blk_shared_data_buf_get_buf_vaddr(
        blk_shared_data_buf_t *shared_data_buf,
        uintptr_t *ret_val
);



