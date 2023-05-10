#include "gtest/gtest.h"

extern "C" {
#include "blk_data_queue.h"
}

/* init() */

TEST(test_blk_data_queue, init_should_compute_correct_capacity) {
    blk_data_queue_t queue = {0};
    char data_region[100] = {0};
    ASSERT_EQ(
            OK_BLK_DATA_QUEUE,
            blk_data_queue_init(
                    &queue,
                    (uintptr_t) data_region,
                    100,
                    10
            )
    );
    size_t capacity;
    ASSERT_EQ(
            OK_BLK_DATA_QUEUE,
            blk_data_queue_capacity(&queue, &capacity)
    );
    ASSERT_EQ(capacity, 10);
}

TEST(test_blk_data_queue, init_should_completely_fill_queuefer) {
    blk_data_queue_t queue = {0};
    char data_region[100] = {0};
    uintptr_t data_region_vaddr = (uintptr_t) data_region;
    ASSERT_EQ(
            OK_BLK_DATA_QUEUE,
            blk_data_queue_init(
                    &queue,
                    data_region_vaddr,
                    100,
                    10
            )
    );
    /* Queue should initially be full. */
    bool is_full;
    ASSERT_EQ(
            OK_BLK_DATA_QUEUE,
            blk_data_queue_is_full(&queue, &is_full)
    );
    ASSERT_TRUE(is_full);
    /* Second check the Queue is full. */
    size_t num_data_bufs;
    ASSERT_EQ(
            OK_BLK_DATA_QUEUE,
            blk_data_queue_size(&queue, &num_data_bufs)
    );
    ASSERT_EQ(num_data_bufs, 10);
    /* There should only be 10 buffers in the queue. */
    for (size_t i = 0; i < 10; i++) {
        blk_data_buf_t data_buf = {0};
        ASSERT_EQ(
                OK_BLK_DATA_QUEUE,
                blk_data_queue_dequeue(&queue, &data_buf)
        );
        ASSERT_EQ(data_buf.buf_vaddr, data_region_vaddr + (i * 10));
        ASSERT_EQ(data_buf.buf_size, 10);
    }
    /* Queue should be empty. */
    bool is_empty;
    ASSERT_EQ(
            OK_BLK_DATA_QUEUE,
            blk_data_queue_is_empty(&queue, &is_empty)
    );
    ASSERT_TRUE(is_empty);
    /* Second check the Queue is empty. */
    ASSERT_EQ(
            OK_BLK_DATA_QUEUE,
            blk_data_queue_size(&queue, &num_data_bufs)
    );
    ASSERT_EQ(num_data_bufs, 0);
}

TEST(test_blk_data_queue, init_should_reject_data_regions_that_are_not_divisible_by_data_buf_size) {
    blk_data_queue_t queue = {0};
    char data_region[100] = {0};
    ASSERT_EQ(
            ERR_BLK_DATA_BUF_SIZE_MISALIGNED,
            blk_data_queue_init(
                    &queue,
                    (uintptr_t) data_region,
                    100,
                    33
            )
    );
}

TEST(test_blk_data_queue, init_should_reject_invalid_buffer_sizes) {
    blk_data_queue_t queue = {0};
    char data_region[100] = {0};
    ASSERT_EQ(
            ERR_BLK_DATA_BUF_SIZE_TOO_LARGE,
            blk_data_queue_init(
                    &queue,
                    (uintptr_t) data_region,
                    100,
                    101
            )
    );
}

TEST(test_blk_data_queue, init_should_reject_invalid_shared_data_region_vaddr) {
    blk_data_queue_t queue = {0};
    ASSERT_EQ(
            ERR_INVALID_BLK_DATA_REGION,
            blk_data_queue_init(
                    &queue,
                    0,
                    100,
                    10
            )
    );
}

TEST(test_blk_data_queue, init_should_reject_null_queue) {
    char data_region[100] = {0};
    ASSERT_EQ(
            ERR_NULL_BLK_DATA_QUEUE,
            blk_data_queue_init(
                    NULL,
                    (uintptr_t) data_region,
                    100,
                    10
            )
    );
}

TEST(test_blk_data_queue, init_should_throw_error_if_data_bufs_is_not_large_enough) {
    blk_data_queue_t queue = {0};
    char data_region[100] = {0};
    ASSERT_EQ(
            ERR_INCREASE_MAX_NUM_BLK_DATA_BUFS,
            blk_data_queue_init(
                    &queue,
                    (uintptr_t) data_region,
                    MAX_NUM_BLK_DATA_BUFS,
                    1
            )
    );
}

TEST(test_blk_data_queue, init_should_not_throw_error_if_data_bufs_is_large_enough) {
    blk_data_queue_t queue = {0};
    char data_region[100] = {0};
    ASSERT_EQ(
            OK_BLK_DATA_QUEUE,
            blk_data_queue_init(
                    &queue,
                    (uintptr_t) data_region,
                    MAX_NUM_BLK_DATA_BUFS - 1,
                    1
            )
    );
}

/* capacity() */

TEST(test_blk_data_queue, capacity_should_reject_null_queue) {
    size_t capacity = 0;
    ASSERT_EQ(
            ERR_NULL_BLK_DATA_QUEUE,
            blk_data_queue_capacity(
                    NULL,
                    &capacity
            )
    );
}

TEST(test_blk_data_queue, capacity_should_reject_null_capacity) {
    blk_data_queue_t queue = {0};
    ASSERT_EQ(
            ERR_NULL_RET_VAL_PTR_PASSED_TO_BLK_DATA_QUEUE_FN,
            blk_data_queue_capacity(
                    &queue,
                    NULL
            )
    );
}

/* is_full() */

TEST(test_blk_data_queue, is_full_should_return_true_if_queue_is_full_and_false_otherwise) {
    blk_data_queue_t queue = {0};
    char data_region[100] = {0};
    uintptr_t data_region_vaddr = (uintptr_t) data_region;
    ASSERT_EQ(
            OK_BLK_DATA_QUEUE,
            blk_data_queue_init(
                    &queue,
                    data_region_vaddr,
                    100,
                    10
            )
    );
    /* Queue should initially be full. */
    bool is_full;
    ASSERT_EQ(
            OK_BLK_DATA_QUEUE,
            blk_data_queue_is_full(&queue, &is_full)
    );
    ASSERT_TRUE(is_full);
    /* We dequeue a single buffer. */
    blk_data_buf_t data_buf = {0};
    ASSERT_EQ(
            OK_BLK_DATA_QUEUE,
            blk_data_queue_dequeue(
                    &queue,
                    &data_buf
            )
    );
    /* Queue should no longer be full. */
    ASSERT_EQ(
            OK_BLK_DATA_QUEUE,
            blk_data_queue_is_full(&queue, &is_full)
    );
    ASSERT_FALSE(is_full);
}

/* is_empty() */

TEST(test_blk_data_queue, is_empty_should_return_true_if_queue_is_empty_and_false_otherwise) {
    blk_data_queue_t queue = {0};
    char data_region[100] = {0};
    uintptr_t data_region_vaddr = (uintptr_t) data_region;
    ASSERT_EQ(
            OK_BLK_DATA_QUEUE,
            blk_data_queue_init(
                    &queue,
                    data_region_vaddr,
                    100,
                    10
            )
    );
    /* Get capacity of ring buffer. */
    size_t capacity;
    ASSERT_EQ(
            OK_BLK_DATA_QUEUE,
            blk_data_queue_capacity(&queue, &capacity)
    );
    /* We dequeue until there is only a single buffer left in the queue. */
    blk_data_buf_t data_buf = {};
    for (size_t i = 0; i < capacity - 1; i++) {
        ASSERT_EQ(
                OK_BLK_DATA_QUEUE,
                blk_data_queue_dequeue(
                        &queue,
                        &data_buf
                )
        );
    }
    /* Queue should not be empty. */
    bool is_empty;
    ASSERT_EQ(
            OK_BLK_DATA_QUEUE,
            blk_data_queue_is_empty(&queue, &is_empty)
    );
    ASSERT_FALSE(is_empty);
    /* We dequeue once more. */
    ASSERT_EQ(
            OK_BLK_DATA_QUEUE,
            blk_data_queue_dequeue(
                    &queue,
                    &data_buf
            )
    );
    /* Queue should be empty now. */
    ASSERT_EQ(
            OK_BLK_DATA_QUEUE,
            blk_data_queue_is_empty(&queue, &is_empty)
    );
    ASSERT_TRUE(is_empty);
}

/* dequeue() */

TEST(test_blk_data_queue, dequeue_should_dequeue_capacity_number_of_data_bufs) {
    blk_data_queue_t queue = {0};
    char data_region[100] = {0};
    uintptr_t data_region_vaddr = (uintptr_t) data_region;
    ASSERT_EQ(
            OK_BLK_DATA_QUEUE,
            blk_data_queue_init(
                    &queue,
                    data_region_vaddr,
                    100,
                    10
            )
    );
    /* Get capacity of ring buffer. */
    size_t capacity;
    ASSERT_EQ(
            OK_BLK_DATA_QUEUE,
            blk_data_queue_capacity(&queue, &capacity)
    );
    /* Queue is initially full so we must dequeue all buffers first to test enqueue. */
    blk_data_buf_t data_buf = {};
    for (size_t i = 0; i < capacity; i++) {
        ASSERT_EQ(
                OK_BLK_DATA_QUEUE,
                blk_data_queue_dequeue(
                        &queue,
                        &data_buf
                )
        );
    }
    /* Queue should be empty. */
    bool is_empty;
    ASSERT_EQ(OK_BLK_DATA_QUEUE, blk_data_queue_is_empty(&queue, &is_empty));
    ASSERT_TRUE(is_empty);
    /* Queue is empty so user should not be able to dequeue from the buffer. */
    ASSERT_EQ(
            ERR_BLK_DATA_QUEUE_EMPTY,
            blk_data_queue_dequeue(
                    &queue,
                    &data_buf
            )
    );
}

/* enqueue() */

TEST(test_blk_data_queue, enqueue_should_enqueue_capacity_number_of_data_bufs) {
    blk_data_queue_t queue = {0};
    char data_region[100] = {0};
    uintptr_t data_region_vaddr = (uintptr_t) data_region;
    ASSERT_EQ(
            OK_BLK_DATA_QUEUE,
            blk_data_queue_init(
                    &queue,
                    data_region_vaddr,
                    100,
                    10
            )
    );
    /* Get capacity of ring buffer. */
    size_t capacity;
    ASSERT_EQ(
            OK_BLK_DATA_QUEUE,
            blk_data_queue_capacity(&queue, &capacity)
    );
    /* Queue is initially full so we must dequeue all buffers first to test enqueue. */
    blk_data_buf_t data_buf = {};
    for (size_t i = 0; i < capacity; i++) {
        ASSERT_EQ(
                OK_BLK_DATA_QUEUE,
                blk_data_queue_dequeue(
                        &queue,
                        &data_buf
                )
        );
    }
    /* Queue should be empty. */
    bool is_empty;
    ASSERT_EQ(OK_BLK_DATA_QUEUE, blk_data_queue_is_empty(&queue, &is_empty));
    ASSERT_TRUE(is_empty);
    /* Enqueue `capacity` number of buffers. */
    for (size_t i = 0; i < capacity; i++) {
        ASSERT_EQ(
                OK_BLK_DATA_QUEUE,
                blk_data_queue_enqueue(
                        &queue,
                        &data_buf
                )
        );
    }
    /* Queue should be full. */
    bool is_full;
    ASSERT_EQ(
            OK_BLK_DATA_QUEUE,
            blk_data_queue_is_full(&queue, &is_full)
    );
    ASSERT_TRUE(is_full);
    /* Queue is full so user should not be able to enqueue onto the buffer. */
    ASSERT_EQ(
            ERR_BLK_DATA_QUEUE_FULL,
            blk_data_queue_enqueue(
                    &queue,
                    &data_buf
            )
    );
}

/* size() */

TEST(test_blk_data_queue, size_should_return_correct_size_if_tail_less_than_head) {
    blk_data_queue_t queue = {0};
    queue.head_idx = MAX_NUM_BLK_DATA_BUFS - 5;
    queue.tail_idx = 5;
    size_t size;
    ASSERT_EQ(
            OK_BLK_DATA_QUEUE,
            blk_data_queue_size(&queue, &size)
    );
    ASSERT_EQ(size, 10);
}
