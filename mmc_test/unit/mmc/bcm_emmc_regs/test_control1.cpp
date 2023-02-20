#include "gtest/gtest.h"

extern "C" {
#include "control1.h"
}

/* set_raw32 */

TEST(test_control1, set_raw32_should_set_it_correctly) {
    control1_t control1 = {};
    /* Initialise everything to 1. */
    memset(&control1, '1', sizeof(control1));
    /* Set value to 0. */
    result_t res = control1_set_raw32(&control1, 0);
    /* Should succeed. */
    ASSERT_TRUE(result_is_ok(res));
    /* Assert value is actually now 0. */
    uint32_t actual_val;
    control1_get_raw32(&control1, &actual_val);
    ASSERT_EQ(0, actual_val);
}

TEST(test_control1, set_raw32_should_not_accept_NULL_ptr) {
    result_t res = control1_set_raw32(NULL, 0);
    /* Should fail. */
    ASSERT_TRUE(result_is_err(res));
}

/* get_raw32 */

TEST(test_control1, get_raw32_should_not_accept_NULL_control1) {
    uint32_t ret_val;
    result_t res = control1_get_raw32(NULL, &ret_val);
    /* Should fail. */
    ASSERT_TRUE(result_is_err(res));
}

TEST(test_control1, get_raw32_should_not_accept_NULL_retval_ptr) {
    control1_t control1 = {};
    result_t res = control1_get_raw32(&control1, NULL);
    /* Should fail. */
    ASSERT_TRUE(result_is_err(res));
}
