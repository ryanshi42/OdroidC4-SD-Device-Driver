#include "gtest/gtest.h"
#include <fff/fff.h>

extern "C" {
#include "sdcard_data_v2.h"
}

DEFINE_FFF_GLOBALS;

FAKE_VALUE_FUNC(result_t, csd_init, csd_t *, uint32_t, uint32_t, uint32_t, uint32_t);
FAKE_VALUE_FUNC(result_t, csd_get_csd_structure, csd_t *, csd_structure_t *);
FAKE_VALUE_FUNC(result_t, csd_get_c_size, csd_t *, uint32_t *);
FAKE_VALUE_FUNC(result_t, csd_get_ver2_c_size, csd_t *, uint32_t *);
FAKE_VALUE_FUNC(result_t, csd_get_read_bl_len, csd_t *, uint8_t *);
FAKE_VALUE_FUNC(result_t, csd_get_write_bl_len, csd_t *, uint8_t *);

/* Resets all Fakes for each unit test. */
class TestSdcardDataV2 : public testing::Test {
protected:
    // You can define per-test set-up logic as usual.
    void SetUp() override {
        FFF_RESET_HISTORY();

        RESET_FAKE(csd_init);
        RESET_FAKE(csd_get_csd_structure);
        RESET_FAKE(csd_get_c_size);
        RESET_FAKE(csd_get_ver2_c_size);
        RESET_FAKE(csd_get_read_bl_len);
        RESET_FAKE(csd_get_write_bl_len);
    }

    // You can define per-test tear-down logic as usual.
    void TearDown() override {

    }
};

/* get_memory_capacity. */

TEST_F(TestSdcardDataV2, get_memory_capacity_should_return_correct_memory_capacity_given_csize) {
    csd_t csd = {};

    csd_get_ver2_c_size_fake.custom_fake = [](csd_t *csd, uint32_t *ret_val) -> result_t {
        *ret_val = 122111;
        return result_ok();
    };

    uint64_t ret_val = 0;
    result_t res = sdcard_data_v2_get_memory_capacity(&csd, &ret_val);
    ASSERT_EQ(result_is_ok(res), true);
    ASSERT_EQ(64021856256, ret_val);
}

/* get_num_blocks. */

TEST_F(TestSdcardDataV2, get_num_blocks_should_return_correct_num_blocks_given_csize) {
    csd_t csd = {};

    csd_get_ver2_c_size_fake.custom_fake = [](csd_t *csd, uint32_t *ret_val) -> result_t {
        *ret_val = 122111;
        return result_ok();
    };

    uint64_t ret_val = 0;
    result_t res = sdcard_data_v2_get_num_blocks(&csd, &ret_val);
    ASSERT_EQ(result_is_ok(res), true);
    ASSERT_EQ(125042688, ret_val);
}
