#include "gtest/gtest.h"

extern "C" {
#include "result.h"
}

/* ok() */

TEST(test_result, ok_should_create_new_ok) {
    result_t result = result_ok();
    ASSERT_FALSE(result_is_err(result));
    ASSERT_TRUE(result_is_ok(result));
    char actual_msg[100] = {0};
    result_get_err_msg(result, actual_msg, sizeof(actual_msg));
    ASSERT_STREQ("", actual_msg);
    ASSERT_EQ(0, result_get_num_err_msgs(result));
}

/* err() */

TEST(test_result, err_should_create_new_error) {
    result_t result = result_err("NULL pointer exception");
    ASSERT_TRUE(result_is_err(result));
    ASSERT_FALSE(result_is_ok(result));
    char actual_msg[100] = {0};
    result_get_err_msg(result, actual_msg, sizeof(actual_msg));
    ASSERT_STREQ("NULL pointer exception\n", actual_msg);
    ASSERT_EQ(1, result_get_num_err_msgs(result));
}

/* is_err() */

TEST(test_result, is_err_should_return_false_for_non_errors) {
    ASSERT_FALSE(result_is_err(result_ok()));
}

TEST(test_result, is_err_should_return_true_for_errors) {
    ASSERT_TRUE(result_is_err(result_err("This is an error.")));
}

/* is_ok() */

TEST(test_result, is_ok_should_return_true_for_ok) {
    ASSERT_TRUE(result_is_ok(result_ok()));
}

TEST(test_result, is_ok_should_return_false_for_errors) {
    ASSERT_FALSE(result_is_ok(result_err("This is an error.")));
}

/* ok_or() */

TEST(test_result, ok_or_should_append_new_errors) {
    result_t result = result_err("NULL pointer exception");
    result = result_ok_or(result, "Illegal Argument Exception.");
    char actual_msg[100] = {0};
    result_get_err_msg(result, actual_msg, sizeof(actual_msg));
    ASSERT_STREQ("NULL pointer exception\nIllegal Argument Exception.\n", actual_msg);
    ASSERT_EQ(2, result_get_num_err_msgs(result));
}

TEST(test_result, ok_or_should_not_overflow_max_num_msgs) {
    result_t result = result_err("NULL pointer exception");
    for (int i = 0; i < MAX_NUM_ERR_MSGS; i++) {
        ASSERT_EQ(1 + i, result_get_num_err_msgs(result));
        result = result_ok_or(result, "Illegal Argument Exception.");
    }
    ASSERT_EQ(MAX_NUM_ERR_MSGS, result_get_num_err_msgs(result));
    for (int i = 0; i < MAX_NUM_ERR_MSGS; i++) {
        ASSERT_EQ(MAX_NUM_ERR_MSGS, result_get_num_err_msgs(result));
        result = result_ok_or(result, "Illegal Argument Exception.");
    }
}

/* get_err_msg() */

TEST(test_result, get_err_msg_should_not_segfault_given_null_char_buf) {
    result_t result = result_err("NULL pointer exception");
    result_get_err_msg(result, NULL, 100);
}

TEST(test_result, get_err_msg_should_accept_char_buf_of_size_zero) {
    result_t result = result_err("NULL pointer exception");
    char actual_msg[100] = {0};
    result_get_err_msg(result, actual_msg, 0);
    ASSERT_STREQ("", actual_msg);
}

TEST(test_result, get_err_msg_should_zero_char_buf) {
    result_t result = result_err("a");
    result = result_ok_or(result, "b");
    char actual_msg[100] = {'c'};
    result_get_err_msg(result, actual_msg, sizeof(actual_msg));
    ASSERT_STREQ("a\nb\n", actual_msg);
}

TEST(test_result, get_err_msg_should_not_write_new_line_if_no_space) {
    result_t result = result_err("a");
    result = result_ok_or(result, "b");
    char actual_msg[4] = {0};
    result_get_err_msg(result, actual_msg, sizeof(actual_msg));
    ASSERT_STREQ("a\nb", actual_msg);
}

TEST(test_result, get_err_msg_should_not_write_new_msg_if_no_space) {
    result_t result = result_err("a");
    result = result_ok_or(result, "b");
    char actual_msg[3] = {0};
    result_get_err_msg(result, actual_msg, sizeof(actual_msg));
    ASSERT_STREQ("a\n", actual_msg);
}

TEST(test_result, get_err_msg_should_not_write_new_msg_if_no_space_long) {
    result_t result = result_err("NULL pointer exception.");
    result = result_ok_or(result, "Illegal Argument Exception.");
    char actual_msg[25] = {0};
    result_get_err_msg(result, actual_msg, sizeof(actual_msg));
    ASSERT_STREQ("NULL pointer exception.\n", actual_msg);
}

/* err_chain() */

TEST(test_result, err_chain_should_chain_err_msgs) {
    result_t result = result_err("NULL pointer exception.");
    ASSERT_EQ(1, result_get_num_err_msgs(result));
    result = result_err_chain(result, "Illegal Argument Exception.");
    ASSERT_EQ(2, result_get_num_err_msgs(result));
    char actual_msg[100] = {0};
    result_get_err_msg(result, actual_msg, sizeof(actual_msg));
    ASSERT_STREQ("NULL pointer exception.\nIllegal Argument Exception.\n", actual_msg);
}

TEST(test_result, err_chain_should_not_overflow_err_msgs) {
    result_t result = result_err("NULL pointer exception");
    for (int i = 0; i < MAX_NUM_ERR_MSGS; i++) {
        ASSERT_EQ(1 + i, result_get_num_err_msgs(result));
        ASSERT_EQ(1 + i, result_get_total_num_err(result));
        result = result_err_chain(result, "Illegal Argument Exception.");
    }
    ASSERT_EQ(MAX_NUM_ERR_MSGS, result_get_num_err_msgs(result));
    ASSERT_EQ(MAX_NUM_ERR_MSGS + 1, result_get_total_num_err(result));
    for (int i = 0; i < MAX_NUM_ERR_MSGS; i++) {
        ASSERT_EQ(MAX_NUM_ERR_MSGS, result_get_num_err_msgs(result));
        ASSERT_EQ(MAX_NUM_ERR_MSGS + 1 + i, result_get_total_num_err(result));
        result = result_err_chain(result, "Illegal Argument Exception.");
    }
}




