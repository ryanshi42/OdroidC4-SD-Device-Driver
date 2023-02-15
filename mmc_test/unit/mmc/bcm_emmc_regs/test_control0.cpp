#include "gtest/gtest.h"

extern "C" {
#include "control0.h"
}

/* set_raw32 */

TEST(test_control0, set_raw32_should_set_it_correctly) {
    control0_t control0 = {};
    /* Initialise everything to 1. */
    memset(&control0, '1', sizeof(control0));
    /* Set value to 0. */
    result_t res = control0_set_raw32(&control0, 0);
    /* Should succeed. */
    ASSERT_TRUE(result_is_ok(res));
    /* Assert value is actually now 0. */
    uint32_t actual_val;
    control0_get_raw32(&control0, &actual_val);
    ASSERT_EQ(0, actual_val);
}

TEST(test_control0, set_raw32_should_not_accept_NULL_ptr) {
    result_t res = control0_set_raw32(NULL, 0);
    /* Should fail. */
    ASSERT_TRUE(result_is_err(res));
}

/* get_raw32 */

TEST(test_control0, get_raw32_should_not_accept_NULL_control0_ptr) {
    uint32_t ret_val;
    result_t res = control0_get_raw32(NULL, &ret_val);
    /* Should fail. */
    ASSERT_TRUE(result_is_err(res));
}

TEST(test_control0, get_raw32_should_not_accept_NULL_retval_ptr) {
    control0_t control0 = {};
    result_t res = control0_get_raw32(&control0, NULL);
    /* Should fail. */
    ASSERT_TRUE(result_is_err(res));
}
