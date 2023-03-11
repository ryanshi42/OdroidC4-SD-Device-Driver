#include "gtest/gtest.h"

extern "C" {
#include "blksizecnt.h"
}

/* set_blksize() */

TEST(test_blksizecnt, set_blksize_should_reject_large_values) {
    blksizecnt_t blksizecnt = {};

    for (uint32_t i = 1; i < 100; i++) {
        result_t res = blksizecnt_set_blksize(&blksizecnt, 0b1111111111 + i);
        ASSERT_FALSE(result_is_ok(res));
    }
}

/* set_blkcnt() */

TEST(test_blksizecnt, set_blkcnt_should_reject_large_values) {
    blksizecnt_t blksizecnt = {};

    for (uint32_t i = 1; i < 100; i++) {
        result_t res = blksizecnt_set_blkcnt(&blksizecnt, 0b1111111111111111 + i);
        ASSERT_FALSE(result_is_ok(res));
    }
}


