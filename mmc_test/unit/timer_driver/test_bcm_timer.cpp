#include "gtest/gtest.h"

extern "C" {
#include "bcm_timer.h"
}

TEST(test_bcm_timer, init_should_init_bcm_timer) {
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



