#include "gtest/gtest.h"

extern "C" {
#include "scr.h"
}

/* get_bus_width() */

TEST(test_result, get_bus_width_should_get_bus_width) {
    scr_t scr = {};
    result_t res;
    /* 0b 0011 1000 0000 1100 0101 0000 0010 */
    res = scr_set_raw32_lo(&scr, 0x0380c502);
    ASSERT_TRUE(result_is_ok(res));

    bus_width_t bus_width;
    res = scr_get_bus_width(&scr, &bus_width);
    ASSERT_TRUE(result_is_ok(res));
    ASSERT_EQ(0b0101, bus_width);
}

/* scr_is_bus_width_1_supported() */

TEST(test_result, scr_is_bus_width_1_supported_should_get_bus_width_1_supported) {
    scr_t scr = {};
    result_t res;
    /* 0b 0011 1000 0000 1100 0101 0000 0010 */
    res = scr_set_raw32_lo(&scr, 0x0380c502);
    ASSERT_TRUE(result_is_ok(res));

    bool is_supported;
    res = scr_is_bus_width_1_supported(&scr, &is_supported);
    ASSERT_TRUE(result_is_ok(res));
    ASSERT_TRUE(is_supported);
}

/* scr_is_bus_width_4_supported() */

TEST(test_result, scr_is_bus_width_4_supported_should_get_bus_width_4_supported) {
    scr_t scr = {};
    result_t res;
    /* 0b 0011 1000 0000 1100 0101 0000 0010 */
    res = scr_set_raw32_lo(&scr, 0x0380c502);
    ASSERT_TRUE(result_is_ok(res));

    bool is_supported;
    res = scr_is_bus_width_4_supported(&scr, &is_supported);
    ASSERT_TRUE(result_is_ok(res));
    ASSERT_TRUE(is_supported);
}

/* scr_is_set_block_count_cmd_supported */

TEST(test_result, scr_is_set_block_count_cmd_supported_should_return_true_if_supported) {
    scr_t scr = {};
    result_t res;
    /* 0b 10 0000 0000 0000 0000 0000 0000 */
    res = scr_set_raw32_lo(&scr, 0b10000000000000000000000000);
    ASSERT_TRUE(result_is_ok(res));

    bool is_supported;
    res = scr_is_set_block_count_cmd_supported(&scr, &is_supported);
    ASSERT_TRUE(result_is_ok(res));
    ASSERT_TRUE(is_supported);
}

TEST(test_result, scr_is_set_block_count_cmd_supported_should_return_false_otherwise) {
    scr_t scr = {};
    result_t res;
    res = scr_set_raw32_lo(&scr, 0x0);
    ASSERT_TRUE(result_is_ok(res));

    bool is_supported;
    res = scr_is_set_block_count_cmd_supported(&scr, &is_supported);
    ASSERT_TRUE(result_is_ok(res));
    ASSERT_TRUE(!is_supported);
}

TEST(test_result, scr_is_speed_class_control_cmd_supported_should_return_true_if_supported) {
    scr_t scr = {};
    result_t res;
    /* 0b 1 0000 0000 0000 0000 0000 0000 */
    res = scr_set_raw32_lo(&scr, 0b1000000000000000000000000);
    ASSERT_TRUE(result_is_ok(res));

    bool is_supported;
    res = scr_is_speed_class_control_cmd_supported(&scr, &is_supported);
    ASSERT_TRUE(result_is_ok(res));
    ASSERT_TRUE(is_supported);
}

TEST(test_result, scr_is_speed_class_control_cmd_supported_should_return_false_otherwise) {
    scr_t scr = {};
    result_t res;
    res = scr_set_raw32_lo(&scr, 0x0);
    ASSERT_TRUE(result_is_ok(res));

    bool is_supported;
    res = scr_is_speed_class_control_cmd_supported(&scr, &is_supported);
    ASSERT_TRUE(result_is_ok(res));
    ASSERT_TRUE(!is_supported);
}


