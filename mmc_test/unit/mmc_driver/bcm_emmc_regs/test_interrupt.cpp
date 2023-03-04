#include "gtest/gtest.h"

extern "C" {
#include "interrupt.h"
}

/* mask_raw32 */

TEST(test_control1, mask_raw32_should_mask_correctly) {
    interrupt_t interrupt = {};
    uint32_t mask = 0b10101010;

    bool expected;
    result_t result = interrupt_mask_raw32(&interrupt, mask, &expected);
    ASSERT_TRUE(result_is_ok(result));
    ASSERT_FALSE(expected);
    interrupt_set_raw32(&interrupt, mask);

    result = interrupt_mask_raw32(&interrupt, mask, &expected);
    ASSERT_TRUE(result_is_ok(result));
    ASSERT_TRUE(expected);
}
