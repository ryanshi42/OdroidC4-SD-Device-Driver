#include "gtest/gtest.h"

extern "C" {
#include "control1.h"
}

/* set_raw32 */

TEST(test_control1, set_raw32_should_set_it_correctly) {
    control1_t control1 = {};
    /* Initialise bit to 1. */
    memset((void *) &control1, 0xFF, sizeof(control1));
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

/* set_srst_hc */

TEST(test_control1, set_srst_hc_should_set_srst_hc_only) {
    control1_t control1 = {};
    /* Initialise every bit to 1. */
    memset((void *) &control1, 0xFF, sizeof(control1));
    bool actual_val;
    /* SRST_CMD should be initialised to 1. */
    control1_get_srst_cmd(&control1, &actual_val);
    ASSERT_EQ(true, actual_val);
    /* SRST_DATA should be initialised to 1. */
    control1_get_srst_data(&control1, &actual_val);
    ASSERT_EQ(true, actual_val);
    /* Set `SRST_HC` to false. */
    result_t res = control1_set_srst_hc(&control1, false);
    /* Should succeed. */
    ASSERT_TRUE(result_is_ok(res));
    /* Assert value is actually now false. */
    control1_get_srst_hc(&control1, &actual_val);
    ASSERT_EQ(false, actual_val);
    /* SRST_CMD should still be 1. */
    control1_get_srst_cmd(&control1, &actual_val);
    ASSERT_EQ(true, actual_val);
    /* SRST_DATA should still be 1. */
    control1_get_srst_data(&control1, &actual_val);
    ASSERT_EQ(true, actual_val);
}

