#include "gtest/gtest.h"

extern "C" {
#include "sdcard.h"
}

/* get_sd_clock_divisor */

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
