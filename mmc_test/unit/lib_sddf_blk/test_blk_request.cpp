#include "gtest/gtest.h"

extern "C" {
#include "blk_request.h"
}

/* sizeof() */

TEST(test_blk_request, request_should_be_the_correct_size) {
    /* If the size of `blk_request_t` changes, make sure to update
     * `MAX_NUM_BLK_REQUESTS` in `blk_request_queue.h`. */
    ASSERT_EQ(
            40,
            sizeof(blk_request_t)
    );
}

/* init_get_sector_count() */

TEST(test_blk_request, init_get_sector_count_should_set_internal_values_correctly) {
    blk_request_t req = {};
    blk_shared_data_buf_t shared_data_buf = {0};
    ASSERT_EQ(
            OK_BLK_SHARED_DATA_BUF,
            blk_shared_data_buf_init(
                    &shared_data_buf,
                    1,
                    2
            )
    );
    ASSERT_EQ(
            OK_BLK_REQUEST,
            blk_request_init_get_sector_count(
                    &req,
                    &shared_data_buf
            )
    );
    /* Zero out shared_data_buf. */
    memset(&shared_data_buf, 0, sizeof(shared_data_buf));

    /* Construct a new shared_data_buf for comparison. */
    blk_shared_data_buf_t expected_shared_data_buf = {0};
    ASSERT_EQ(
            OK_BLK_SHARED_DATA_BUF,
            blk_shared_data_buf_init(
                    &expected_shared_data_buf,
                    1,
                    2
            )
    );

    bool is_equal = false;
    ASSERT_EQ(
            OK_BLK_SHARED_DATA_BUF,
            blk_shared_data_buf_equals(
                    &expected_shared_data_buf,
                    &req.shared_data_buf,
                    &is_equal
            )
    );
    ASSERT_TRUE(is_equal);
}


