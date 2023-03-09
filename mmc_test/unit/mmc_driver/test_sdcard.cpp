#include "gtest/gtest.h"

extern "C" {
#include "sdcard.h"
}

/* set_ocr */

TEST(test_sdcard, sdcard_set_ocr_should_set_ocr) {
    sdcard_t sdcard = {};
    result_t res = sdcard_set_ocr_raw32(
            &sdcard,
            0x00FF00FF
    );
    ASSERT_TRUE(result_is_ok(res));
    uint32_t sdcard_ocr = sdcard.ocr.raw32;
    ASSERT_EQ(0x00FF00FF, sdcard_ocr);
}

/* has_rca */

TEST(test_sdcard, has_rca_should_return_false_if_no_rca) {
    sdcard_t sdcard = {};
    result_t res = sdcard_init(&sdcard);
    ASSERT_TRUE(result_is_ok(res));
    bool has_rca = false;
    res = sdcard_has_rca(
            &sdcard,
            &has_rca
    );
    ASSERT_TRUE(result_is_ok(res));
    ASSERT_FALSE(has_rca);
}

TEST(test_sdcard, has_rca_should_return_true_if_rca_present) {
    sdcard_t sdcard = {};
    result_t res = sdcard_init(&sdcard);
    ASSERT_TRUE(result_is_ok(res));

    res = sdcard_set_rca(
            &sdcard,
            0x1234
    );
    ASSERT_TRUE(result_is_ok(res));

    bool has_rca = false;
    res = sdcard_has_rca(
            &sdcard,
            &has_rca
    );
    ASSERT_TRUE(result_is_ok(res));
    ASSERT_TRUE(has_rca);
}
