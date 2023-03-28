#include "gtest/gtest.h"

extern "C" {
#include "result_prod.h"
}

/* We have to write all our tests for `result_prod` in one unit test since we're
 * using a single global to store the result's data. Multiple unit tests are
 * usually run in parallel, which would lead to race conditions for
 * `result_prod`. */

TEST(test_result, result_prod_test) {
    /* ok_should_create_new_ok */
    result_t result = result_ok();
    ASSERT_FALSE(result_is_err(result));
    ASSERT_TRUE(result_is_ok(result));

    char actual_msg[100] = {0};
    result_get_err_msg(result, actual_msg, sizeof(actual_msg));
    ASSERT_STREQ("", actual_msg);
    ASSERT_EQ(0, result_get_num_err_msgs(result));

    /* err_should_create_new_error */
    result = result_err("NULL pointer exception");
    ASSERT_TRUE(result_is_err(result));
    ASSERT_FALSE(result_is_ok(result));
    result_get_err_msg(result, actual_msg, sizeof(actual_msg));
    ASSERT_STREQ("NULL pointer exception\n", actual_msg);
    ASSERT_EQ(1, result_get_num_err_msgs(result));

    /* printf_should_printf_error_message */
    result = result_err_chain(result, "Illegal Argument Exception.");
    result = result_err_chain(result, "Illegal Argument Exception 2.");
    testing::internal::CaptureStdout();
    result_printf(result);
    std::string output = testing::internal::GetCapturedStdout();
    /* Printing out for myself. */
    std::cout << output << std::endl;
    ASSERT_TRUE(output.find("Printing 3 out of 3 error messages.") != std::string::npos);
    ASSERT_TRUE(output.find("NULL pointer exception") != std::string::npos);
    ASSERT_TRUE(output.find("Illegal Argument Exception.") != std::string::npos);
    ASSERT_TRUE(output.find("Illegal Argument Exception 2.") != std::string::npos);
    ASSERT_TRUE(output.find("...") == std::string::npos);
}
