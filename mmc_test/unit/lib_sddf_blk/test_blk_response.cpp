#include "gtest/gtest.h"

extern "C" {
#include "blk_response.h"
}

/* sizeof() */

TEST(test_blk_response, response_should_be_the_correct_size) {
    /* If the size of `blk_response_t` changes, make sure to update
     * `MAX_NUM_BLK_RESPONSES` in `blk_response_queue.h`. */
    ASSERT_EQ(
            32,
            sizeof(blk_response_t)
    );
}

/* is_ok() */

TEST(test_blk_response, is_ok_should_return_true_if_result_is_ok) {
    blk_response_t response = {};
    blk_shared_data_buf_t shared_data_buf = {0};
    ASSERT_EQ(
            OK_BLK_SHARED_DATA_BUF,
            blk_response_init_ok(
                    &response,
                    &shared_data_buf
            )
    );
    bool is_ok = false;
    ASSERT_EQ(
            OK_BLK_SHARED_DATA_BUF,
            blk_response_is_ok(
                    &response,
                    &is_ok
            )
    );
    ASSERT_TRUE(is_ok);
}

TEST(test_blk_response, is_ok_should_return_false_if_result_is_not_ok) {
    blk_response_t response = {};
    blk_shared_data_buf_t shared_data_buf = {0};
    ASSERT_EQ(
            OK_BLK_SHARED_DATA_BUF,
            blk_response_init_error(
                    &response,
                    &shared_data_buf
            )
    );
    bool is_ok = false;
    ASSERT_EQ(
            OK_BLK_SHARED_DATA_BUF,
            blk_response_is_ok(
                    &response,
                    &is_ok
            )
    );
    ASSERT_FALSE(is_ok);
}
