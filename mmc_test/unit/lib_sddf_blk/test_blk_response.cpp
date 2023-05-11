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
