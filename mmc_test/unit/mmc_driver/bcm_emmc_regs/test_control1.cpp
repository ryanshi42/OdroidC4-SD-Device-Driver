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

/* set_data_tounit */

TEST(test_control1, set_data_tounit_should_set_data_tounit_only) {
    control1_t control1 = {};
    memset((void *) &control1, 0, sizeof(control1));
    result_t res = control1_set_data_tounit(&control1, 0b1111);
    ASSERT_TRUE(result_is_ok(res));
    uint8_t actual_val;
    control1_get_data_tounit(&control1, &actual_val);
    ASSERT_EQ(0b1111, actual_val);
}

TEST(test_control1, set_data_tounit_should_prevent_setting_too_large_value) {
    control1_t control1 = {};
    memset((void *) &control1, 0, sizeof(control1));
    result_t res = control1_set_data_tounit(&control1, 0b1111 + 1);
    ASSERT_TRUE(result_is_err(res));
    uint8_t actual_val;
    control1_get_data_tounit(&control1, &actual_val);
    ASSERT_EQ(0, actual_val);
}

/* set_clk_freq_ms2 */

TEST(test_control1, set_clk_freq_ms2_should_return_error_if_value_too_large) {
    control1_t control1 = {};
    memset((void *) &control1, 0, sizeof(control1));
    result_t res = control1_set_clk_freq_ms2(&control1, 0b11 + 1);
    ASSERT_TRUE(result_is_err(res));
    ASSERT_STREQ(
            "Invalid `val` passed to control1_set_clk_freq_ms2().",
            result_get_last_err_msg(res)
    );
}

TEST(test_control1, set_clk_freq_ms2_should_succeed_for_value_less_than_equal_0b11) {
    control1_t control1 = {};
    memset((void *) &control1, 0, sizeof(control1));
    for (size_t i = 0; i <= 0b11; i++) {
        result_t res = control1_set_clk_freq_ms2(&control1, i);
        ASSERT_TRUE(result_is_ok(res));
    }
}

/* set_clk_freq8 */

TEST(test_control1, set_clk_freq8_should_succeed_for_value_less_than_equal_0b11111111) {
    control1_t control1 = {};
    memset((void *) &control1, 0, sizeof(control1));
    for (size_t i = 0; i <= 0b11111111; i++) {
        result_t res = control1_set_clk_freq8(&control1, i);
        ASSERT_TRUE(result_is_ok(res));
    }
}


