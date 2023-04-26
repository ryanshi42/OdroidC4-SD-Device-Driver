#include "gtest/gtest.h"

extern "C" {
#include "blk_data_ring_buf.h"
}

/* init() */

TEST(test_blk_data_ring_buf, init_should_compute_correct_capacity) {
    blk_data_ring_buf_t ring_buf = {0};
    char data_region[100] = {0};
    ASSERT_EQ(
            OK_BLK_DATA_RING_BUF,
            blk_data_ring_buf_init(
                    &ring_buf,
                    (uintptr_t) data_region,
                    100,
                    10
            )
    );
    size_t capacity;
    ASSERT_EQ(0, blk_data_ring_buf_capacity(&ring_buf, &capacity));
    ASSERT_EQ(capacity, 10);
}

TEST(test_blk_data_ring_buf, init_should_reject_data_regions_that_are_not_divisible_by_data_buf_size) {
    blk_data_ring_buf_t ring_buf = {0};
    char data_region[100] = {0};
    ASSERT_EQ(
            ERR_BLK_DATA_BUF_SIZE_MISALIGNED,
            blk_data_ring_buf_init(
                    &ring_buf,
                    (uintptr_t) data_region,
                    100,
                    33
            )
    );
}

TEST(test_blk_data_ring_buf, init_should_reject_invalid_buffer_sizes) {
    blk_data_ring_buf_t ring_buf = {0};
    char data_region[100] = {0};
    ASSERT_EQ(
            ERR_BLK_DATA_BUF_SIZE_TOO_LARGE,
            blk_data_ring_buf_init(
                    &ring_buf,
                    (uintptr_t) data_region,
                    100,
                    101
            )
    );
}

TEST(test_blk_data_ring_buf, init_should_reject_invalid_shared_data_region_vaddr) {
    blk_data_ring_buf_t ring_buf = {0};
    ASSERT_EQ(
            ERR_INVALID_BLK_DATA_REGION,
            blk_data_ring_buf_init(
                    &ring_buf,
                    0,
                    100,
                    10
            )
    );
}

TEST(test_blk_data_ring_buf, init_should_reject_null_ring_buf) {
    char data_region[100] = {0};
    ASSERT_EQ(
            ERR_NULL_BLK_DATA_RING_BUF,
            blk_data_ring_buf_init(
                    NULL,
                    (uintptr_t) data_region,
                    100,
                    10
            )
    );
}

TEST(test_blk_data_ring_buf, init_should_throw_error_if_data_bufs_is_not_large_enough) {
    blk_data_ring_buf_t ring_buf = {0};
    char data_region[100] = {0};
    ASSERT_EQ(
            ERR_INCREASE_MAX_NUM_BLK_DATA_BUFS,
            blk_data_ring_buf_init(
                    &ring_buf,
                    (uintptr_t) data_region,
                    MAX_NUM_BLK_DATA_BUFS,
                    1
            )
    );
}

TEST(test_blk_data_ring_buf, init_should_not_throw_error_if_data_bufs_is_large_enough) {
    blk_data_ring_buf_t ring_buf = {0};
    char data_region[100] = {0};
    ASSERT_EQ(
            OK_BLK_DATA_RING_BUF,
            blk_data_ring_buf_init(
                    &ring_buf,
                    (uintptr_t) data_region,
                    MAX_NUM_BLK_DATA_BUFS - 1,
                    1
            )
    );
}

/* TODO: Write test for blk_data_ring_buf_is_full(). */

/* TODO: Write test for blk_data_ring_buf_enqueue(). */