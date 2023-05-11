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

