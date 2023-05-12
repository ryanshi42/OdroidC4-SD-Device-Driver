#include "blk_shared_data_buf.h"

blk_shared_data_buf_result_t blk_shared_data_buf_init(
        blk_shared_data_buf_t *shared_data_buf,
        uintptr_t buf_vaddr,
        size_t buf_size
) {
    if (shared_data_buf == NULL) {
        return ERR_NULL_BLK_SHARED_DATA_BUF;
    }
    shared_data_buf->buf_vaddr = buf_vaddr;
    shared_data_buf->buf_size = buf_size;
    return OK_BLK_SHARED_DATA_BUF;
}

blk_shared_data_buf_result_t blk_shared_data_buf_equals(
        blk_shared_data_buf_t *shared_data_buf_1,
        blk_shared_data_buf_t *shared_data_buf_2,
        bool *ret_val
) {
    if (shared_data_buf_1 == NULL || shared_data_buf_2 == NULL) {
        return ERR_NULL_BLK_SHARED_DATA_BUF;
    }
    *ret_val = shared_data_buf_1->buf_vaddr == shared_data_buf_2->buf_vaddr &&
               shared_data_buf_1->buf_size == shared_data_buf_2->buf_size;
    return OK_BLK_SHARED_DATA_BUF;
}

blk_shared_data_buf_result_t blk_shared_data_buf_insert_data(
        blk_shared_data_buf_t *shared_data_buf,
        void *data,
        size_t data_size
) {
    if (shared_data_buf == NULL) {
        return ERR_NULL_BLK_SHARED_DATA_BUF;
    }
    if (data_size > shared_data_buf->buf_size) {
        return ERR_BLK_SHARED_DATA_BUF_DATA_TOO_LARGE;
    }
    /* Copy the data into the memory region represented by a `shared_data_buf`. */
    memcpy((void *) shared_data_buf->buf_vaddr, data, data_size);
    return OK_BLK_SHARED_DATA_BUF;
}

blk_shared_data_buf_result_t blk_shared_data_buf_get_buf_size(
        blk_shared_data_buf_t *shared_data_buf,
        size_t *ret_val
) {
    if (shared_data_buf == NULL) {
        return ERR_NULL_BLK_SHARED_DATA_BUF;
    }
    if (ret_val == NULL) {
        return ERR_NULL_RET_VAL_IN_BLK_SHARED_DATA_BUF;
    }
    *ret_val = shared_data_buf->buf_size;
    return OK_BLK_SHARED_DATA_BUF;
}
