#include "gtest/gtest.h"
#include <fff/fff.h>

extern "C" {
#include "sdcard.h"
}

DEFINE_FFF_GLOBALS;

FAKE_VALUE_FUNC(result_t, sdcard_data_init, sdcard_data_t *, uint32_t, uint32_t, uint32_t, uint32_t);
FAKE_VALUE_FUNC(result_t, sdcard_data_get_c_size, sdcard_data_t *, uint32_t*);
FAKE_VALUE_FUNC(result_t, sdcard_data_get_block_size, sdcard_data_t *, uint16_t*);
FAKE_VALUE_FUNC(result_t, sdcard_data_get_memory_capacity, sdcard_data_t *, uint64_t*);
FAKE_VALUE_FUNC(result_t, sdcard_data_get_num_blocks, sdcard_data_t *, uint64_t*);

/* Resets all Fakes for each unit test. */
class TestSdcard : public testing::Test {
protected:
    // You can define per-test set-up logic as usual.
    void SetUp() override {
        FFF_RESET_HISTORY();

        RESET_FAKE(sdcard_data_init);
        RESET_FAKE(sdcard_data_get_c_size);
        RESET_FAKE(sdcard_data_get_block_size);
        RESET_FAKE(sdcard_data_get_memory_capacity);
        RESET_FAKE(sdcard_data_get_num_blocks);
    }

    // You can define per-test tear-down logic as usual.
    void TearDown() override {

    }
};

/* set_ocr */

TEST_F(TestSdcard, sdcard_set_ocr_should_set_ocr) {
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

TEST_F(TestSdcard, has_rca_should_return_false_if_no_rca) {
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

TEST_F(TestSdcard, has_rca_should_return_true_if_rca_present) {
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

/* is_type_unknown */

TEST_F(TestSdcard, is_type_unknown_should_return_true_if_type_unknown) {
    sdcard_t sdcard = {};
    result_t res = sdcard_init(&sdcard);
    ASSERT_TRUE(result_is_ok(res));

    sdcard.type = SD_TYPE_UNKNOWN;

    bool is_type_unknown = false;
    res = sdcard_is_type_unknown(
            &sdcard,
            &is_type_unknown
    );
    ASSERT_TRUE(result_is_ok(res));
    ASSERT_TRUE(is_type_unknown);
}

TEST_F(TestSdcard, is_type_unknown_should_return_false_if_type_known) {
    sdcard_t sdcard = {};
    result_t res = sdcard_init(&sdcard);
    ASSERT_TRUE(result_is_ok(res));

    sdcard.type = SD_TYPE_2_SC;

    bool is_type_unknown = false;
    res = sdcard_is_type_unknown(
            &sdcard,
            &is_type_unknown
    );
    ASSERT_TRUE(result_is_ok(res));
    ASSERT_TRUE(!is_type_unknown);
}

/* get_memory_capacity. */

TEST_F(TestSdcard, get_memory_capacity_should_return_correct_memory_capacity_given_csize) {
    sdcard_t sdcard = {};

    sdcard_data_get_memory_capacity_fake.custom_fake = [](sdcard_data_t *sdcard_data, uint64_t *ret_val) {
        *ret_val = 64021856256;
        return result_ok();
    };

    uint64_t actual_capacity;
    result_t res = sdcard_get_memory_capacity(&sdcard, &actual_capacity);
    ASSERT_TRUE(result_is_ok(res));
    ASSERT_EQ(64021856256, actual_capacity);
}

/* get_num_blocks. */

TEST_F(TestSdcard, get_num_blocks_should_return_correct_num_blocks) {
    sdcard_t sdcard = {};

    sdcard_data_get_num_blocks_fake.custom_fake = [](sdcard_data_t *sdcard_data, uint64_t *ret_val) {
        *ret_val = 125042688;
        return result_ok();
    };

    uint64_t actual_capacity;
    result_t res = sdcard_get_num_blocks(&sdcard, &actual_capacity);
    ASSERT_TRUE(result_is_ok(res));
    ASSERT_EQ(125042688, actual_capacity);
}



