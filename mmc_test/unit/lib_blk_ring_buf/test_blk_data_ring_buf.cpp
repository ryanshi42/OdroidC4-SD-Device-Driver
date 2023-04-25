#include "gtest/gtest.h"

extern "C" {
#include "blk_data_ring_buf.h"
}

/* init() */

TEST(test_blk_data_ring_buf, init_should_compute_correct_capacity) {
    blk_data_ring_buf_t ring_buf = {0};
    char data_region[100] = {0};
    ASSERT_EQ(
            0,
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
            -1,
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
            -1,
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
            -1,
            blk_data_ring_buf_init(
                    &ring_buf,
                    0,
                    100,
                    10
            )
    );
}


