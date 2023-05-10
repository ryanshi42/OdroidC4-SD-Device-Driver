#include "gtest/gtest.h"

extern "C" {
#include "blk_req_ring_buf.h"
}

/* init() */

TEST(test_blk_data_ring_buf, init_should_reject_null_ring_buf) {
    ASSERT_EQ(
            ERR_NULL_BLK_REQ_RING_BUF,
            blk_req_ring_buf_init(
                    NULL,
                    0
            )
    );
}

TEST(test_blk_data_ring_buf, init_should_reject_shared_memory_regions_that_are_too_small) {
    blk_req_ring_buf_t ring_buf = {0};
    ASSERT_EQ(
            ERR_BLK_REQ_RING_BUF_REGION_TOO_SMALL,
            blk_req_ring_buf_init(
                    &ring_buf,
                    sizeof(blk_req_ring_buf_t) - 1
            )
    );
}

TEST(test_blk_data_ring_buf, init_should_clear_the_ring_buf) {
    blk_req_ring_buf_t ring_buf = {1};
    ASSERT_EQ(
            OK_BLK_REQ_RING_BUF,
            blk_req_ring_buf_init(
                    &ring_buf,
                    sizeof(blk_req_ring_buf_t)
            )
    );
    /* Entire ring buffer should be zeroed. */
    blk_req_ring_buf_t expected = {0};
    ASSERT_EQ(0, memcmp(&expected, &ring_buf, sizeof(ring_buf)));
}

TEST(test_blk_data_ring_buf, init_should_accept_shared_memory_regions_that_are_slightly_oversized) {
    blk_req_ring_buf_t ring_buf = {1};
    ASSERT_EQ(
            OK_BLK_REQ_RING_BUF,
            blk_req_ring_buf_init(
                    &ring_buf,
                    sizeof(blk_req_ring_buf_t) + sizeof(blk_req_buf_t) - 1
            )
    );
    /* Entire ring buffer should be zeroed. */
    blk_req_ring_buf_t expected = {0};
    ASSERT_EQ(0, memcmp(&expected, &ring_buf, sizeof(ring_buf)));
}

TEST(test_blk_data_ring_buf, init_should_reject_shared_memory_regions_that_are_too_oversized) {
    blk_req_ring_buf_t ring_buf = {0};
    ASSERT_EQ(
            ERR_BLK_REQ_RING_BUF_REGION_TOO_LARGE,
            blk_req_ring_buf_init(
                    &ring_buf,
                    sizeof(blk_req_ring_buf_t) + sizeof(blk_req_buf_t)
            )
    );
}

