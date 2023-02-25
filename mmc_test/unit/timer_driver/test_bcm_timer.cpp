#include "gtest/gtest.h"
#include <fff/fff.h>

extern "C" {
#include "bcm_timer.h"
}

DEFINE_FFF_GLOBALS;

FAKE_VALUE_FUNC(
        result_t,
        bcm_timer_regs_get_counter_lo,
        bcm_timer_regs_t *,
        uint32_t *
);

FAKE_VALUE_FUNC(
        result_t,
        bcm_timer_regs_get_counter_hi,
        bcm_timer_regs_t *,
        uint32_t *
);

FAKE_VALUE_FUNC(
        bcm_timer_regs_t *,
        bcm_timer_regs_get,
        uintptr_t
);

/* Resets all Fakes for each unit test. */
class test_bcm_timer_reset : public testing::Test {
protected:
    // You can define per-test set-up logic as usual.
    void SetUp() override {
        RESET_FAKE(bcm_timer_regs_get_counter_lo);
        RESET_FAKE(bcm_timer_regs_get_counter_hi);
        RESET_FAKE(bcm_timer_regs_get);
    }

    // You can define per-test tear-down logic as usual.
    void TearDown() override {

    }
};

TEST(test_bcm_timer, init_should_init_bcm_timer) {
    bcm_timer_regs_get_fake.custom_fake = [](uintptr_t base_vaddr) {
        return (bcm_timer_regs_t *) base_vaddr;
    };

    /* Initialise `regs` to something non-zero. */
    bcm_timer_regs_t regs = {};
    memset((void *) &regs, 0xFF, sizeof(regs));

    /* Initialise bcm_timer_init. */
    bcm_timer_t bcm_timer = {};
    result_t res = bcm_timer_init(&bcm_timer, (uintptr_t) &regs);
    ASSERT_TRUE(result_is_ok(res));

    /* The `regs` variable should be set to `regs`. */
    ASSERT_EQ((uintptr_t) &regs, (uintptr_t) bcm_timer.regs);
}

/* get_num_ticks. */

TEST(test_bcm_timer, get_num_ticks_should_return_counter_hi_and_lo_concatenated) {

    bcm_timer_regs_get_counter_lo_fake.custom_fake = [](bcm_timer_regs_t *regs, uint32_t *ret_val) {
        *ret_val = 0x1;
        return result_ok();
    };

    bcm_timer_regs_get_counter_hi_fake.custom_fake = [](bcm_timer_regs_t *regs, uint32_t *ret_val) {
        *ret_val = 0x2;
        return result_ok();
    };

    bcm_timer_t bcm_timer = {};

    uint64_t actual_ticks;
    bcm_timer_get_num_ticks(&bcm_timer, &actual_ticks);

    /* Should return the correct timer value. */
    ASSERT_EQ((0x2l << 32) | 0x1, actual_ticks);
}

TEST(test_bcm_timer, get_num_ticks_should_return_latest_counter_hi_and_lo_values) {

    bcm_timer_regs_get_counter_lo_fake.custom_fake = [](bcm_timer_regs_t *regs, uint32_t *ret_val) {
        *ret_val = 0x1;
        return result_ok();
    };

    result_t(*counter_hi_mocks[])(bcm_timer_regs_t * , uint32_t * ) = {
            /* The first time `bcm_timer_regs_get_counter_hi` is called, return 0x2. */
            [](bcm_timer_regs_t *regs, uint32_t *ret_val) {
                *ret_val = 0x2;
                return result_ok();
            },
            /* The second time `bcm_timer_regs_get_counter_hi` is called, return 0x2. */
            [](bcm_timer_regs_t *regs, uint32_t *ret_val) {
                *ret_val = 0x3;
                return result_ok();
            }
    };

    SET_CUSTOM_FAKE_SEQ(bcm_timer_regs_get_counter_hi, counter_hi_mocks, 2);

    bcm_timer_t bcm_timer = {};

    uint64_t actual_ticks;
    bcm_timer_get_num_ticks(&bcm_timer, &actual_ticks);

    /* Should return the correct timer value. */
    ASSERT_EQ((0x3l << 32) | 0x1, actual_ticks);
}


