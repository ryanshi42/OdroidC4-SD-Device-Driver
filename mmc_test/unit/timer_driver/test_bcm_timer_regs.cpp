#include "gtest/gtest.h"

extern "C" {
#include "bcm_timer_regs.h"
}

TEST(test_bcm_timer_regs, regs_should_have_the_correct_offset) {
    /* Create a new register struct on the stack. */
    bcm_timer_regs_t regs = {};
    /* Below is the base register. */
    uintptr_t base = (uintptr_t) &regs;

    ASSERT_EQ(base, (uintptr_t) &regs.control_status);
    ASSERT_EQ(base + 0x4, (uintptr_t) &regs.counter_lo);
    ASSERT_EQ(base + 0x8, (uintptr_t) &regs.counter_hi);
    ASSERT_EQ(base + 0xc, (uintptr_t) &regs.compare0);
    ASSERT_EQ(base + 0x10, (uintptr_t) &regs.compare1);
    ASSERT_EQ(base + 0x14, (uintptr_t) &regs.compare2);
    ASSERT_EQ(base + 0x18, (uintptr_t) &regs.compare3);
}


