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
