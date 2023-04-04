#include "gtest/gtest.h"
#include <fff/fff.h>

extern "C" {
#include "sdcard_data_v2.h"
}

DEFINE_FFF_GLOBALS;

FAKE_VALUE_FUNC(result_t, csd_get_ver2_c_size, csd_t *, uint32_t *);

/* get_memory_capacity. */

TEST(test_sdcard_data_v2, get_memory_capacity_should_return_correct_memory_capacity_given_csize) {
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

TEST(test_sdcard_data_v2, get_num_blocks_should_return_correct_num_blocks_given_csize) {
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
