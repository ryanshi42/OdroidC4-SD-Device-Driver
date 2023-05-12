#include "gtest/gtest.h"

extern "C" {
#include "blk_shared_data_buf.h"
}

TEST(test_blk_shared_data_buf, init_should_set_values_correctly) {
    blk_shared_data_buf_t shared_data_buf = {0};
    ASSERT_EQ(
            OK_BLK_SHARED_DATA_BUF,
            blk_shared_data_buf_init(
                    &shared_data_buf,
                    1,
                    2
            )
    );
    ASSERT_EQ(1, shared_data_buf.buf_vaddr);
    ASSERT_EQ(2, shared_data_buf.buf_size);
}

/* insert_data() */

TEST(test_blk_shared_data_buf, insert_data_should_insert_strings) {
    char buf[100] = {0};
    blk_shared_data_buf_t shared_data_buf = {0};
    ASSERT_EQ(
            OK_BLK_SHARED_DATA_BUF,
            blk_shared_data_buf_init(
                    &shared_data_buf,
                    (uintptr_t) buf,
                    sizeof(buf)
            )
    );
    char data[] = "hello";
    ASSERT_EQ(
            OK_BLK_SHARED_DATA_BUF,
            blk_shared_data_buf_insert_data(
                    &shared_data_buf,
                    data,
                    strlen(data) + 1
            )
    );
    ASSERT_EQ(0, strcmp(data, (char *) shared_data_buf.buf_vaddr));
}

TEST(test_blk_shared_data_buf, insert_data_should_insert_size_ts) {
    char buf[100] = {0};
    blk_shared_data_buf_t shared_data_buf = {0};
    ASSERT_EQ(
            OK_BLK_SHARED_DATA_BUF,
            blk_shared_data_buf_init(
                    &shared_data_buf,
                    (uintptr_t) buf,
                    sizeof(buf)
            )
    );
    size_t data = 123;
    ASSERT_EQ(
            OK_BLK_SHARED_DATA_BUF,
            blk_shared_data_buf_insert_data(
                    &shared_data_buf,
                    &data,
                    sizeof(data)
            )
    );
    ASSERT_EQ(data, *(size_t *) shared_data_buf.buf_vaddr);
}

TEST(test_blk_shared_data_buf, insert_data_should_reject_data_that_is_too_large) {
    char buf[100] = {0};
    blk_shared_data_buf_t shared_data_buf = {0};
    ASSERT_EQ(
            OK_BLK_SHARED_DATA_BUF,
            blk_shared_data_buf_init(
                    &shared_data_buf,
                    (uintptr_t) buf,
                    sizeof(buf)
            )
    );
    char data[101] = {0};
    memset(data, 'a', sizeof(data));
    ASSERT_EQ(
            ERR_BLK_SHARED_DATA_BUF_DATA_TOO_LARGE,
            blk_shared_data_buf_insert_data(
                    &shared_data_buf,
                    data,
                    sizeof(data)
            )
    );
    /* The original `buf` should still have nothing in it. */
    ASSERT_EQ(0, strcmp("", (char *) shared_data_buf.buf_vaddr));
    ASSERT_EQ(0, strcmp("", buf));
}

/* get_buf_size() */

TEST(test_blk_shared_data_buf, get_buf_size_should_get_buf_size) {
    char buf[100] = {0};
    blk_shared_data_buf_t shared_data_buf = {0};
    ASSERT_EQ(
            OK_BLK_SHARED_DATA_BUF,
            blk_shared_data_buf_init(
                    &shared_data_buf,
                    (uintptr_t) buf,
                    sizeof(buf)
            )
    );
    size_t buf_size = 0;
    ASSERT_EQ(
            OK_BLK_SHARED_DATA_BUF,
            blk_shared_data_buf_get_buf_size(
                    &shared_data_buf,
                    &buf_size
            )
    );
    ASSERT_EQ(sizeof(buf), buf_size);
}
