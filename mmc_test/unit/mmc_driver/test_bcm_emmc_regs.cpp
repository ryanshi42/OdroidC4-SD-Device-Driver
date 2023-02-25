#include "gtest/gtest.h"

extern "C" {
#include "bcm_emmc_regs.h"
}

TEST(test_bcm_emmc_regs, registers_should_have_the_correct_offset) {
    /* Create a new register struct on the stack. */
    bcm_emmc_regs_t regs = {};
    /* Below is the base register. */
    uintptr_t base = (uintptr_t) &regs;

    ASSERT_EQ(base, (uintptr_t) &regs.arg2);
    ASSERT_EQ(base + 0x4, (uintptr_t) &regs.blksizecnt);
    ASSERT_EQ(base + 0x8, (uintptr_t) &regs.arg1);
    ASSERT_EQ(base + 0xc, (uintptr_t) &regs.cmdtm);
    ASSERT_EQ(base + 0x10, (uintptr_t) &regs.resp0);
    ASSERT_EQ(base + 0x14, (uintptr_t) &regs.resp1);
    ASSERT_EQ(base + 0x18, (uintptr_t) &regs.resp2);
    ASSERT_EQ(base + 0x1c, (uintptr_t) &regs.resp3);
    ASSERT_EQ(base + 0x20, (uintptr_t) &regs.data);
    ASSERT_EQ(base + 0x20, (uintptr_t) &regs.data);
    ASSERT_EQ(base + 0x24, (uintptr_t) &regs.status);
    ASSERT_EQ(base + 0x28, (uintptr_t) &regs.control0);
    ASSERT_EQ(base + 0x2c, (uintptr_t) &regs.control1);
    ASSERT_EQ(base + 0x30, (uintptr_t) &regs.interrupt);
    ASSERT_EQ(base + 0x34, (uintptr_t) &regs.irpt_mask);
    ASSERT_EQ(base + 0x38, (uintptr_t) &regs.irpt_en);
    ASSERT_EQ(base + 0x3c, (uintptr_t) &regs.control2);
    ASSERT_EQ(base + 0x50, (uintptr_t) &regs.force_irpt);
    ASSERT_EQ(base + 0x70, (uintptr_t) &regs.boot_timeout);
    ASSERT_EQ(base + 0x74, (uintptr_t) &regs.dbg_sel);
    ASSERT_EQ(base + 0x80, (uintptr_t) &regs.exrdfifo_cfg);
    ASSERT_EQ(base + 0x84, (uintptr_t) &regs.exrdfifo_en);
    ASSERT_EQ(base + 0x88, (uintptr_t) &regs.tune_step);
    ASSERT_EQ(base + 0x8c, (uintptr_t) &regs.tune_steps_std);
    ASSERT_EQ(base + 0x90, (uintptr_t) &regs.tune_steps_ddr);
    ASSERT_EQ(base + 0xf0, (uintptr_t) &regs.spi_int_spt);
    ASSERT_EQ(base + 0xfc, (uintptr_t) &regs.slot_isr_ver);
}

/* control0_zero */

TEST(test_bcm_emmc_regs, zero_control0_should_zero_control0) {
    bcm_emmc_regs_t regs = {};
    /* Set `regs` to 1. */
    memset((void *) &regs, 0xFF, sizeof(regs));
    /* `control0` is not 0. */
    uint32_t control0;
    control0_get_raw32(&regs.control0, &control0);
    ASSERT_EQ(0xFFFFFFFF, control0);
    /* Zero out `control0`. */
    result_t res = bcm_emmc_regs_zero_control0(&regs);
    /* Should be successful. */
    ASSERT_TRUE(result_is_ok(res));
    /* Assert the setting was successful. */
    control0_get_raw32(&regs.control0, &control0);
    ASSERT_EQ(0, control0);
}

/* regs_get */

TEST(test_bcm_emmc_regs, get_should_return_the_correct_value) {
    bcm_emmc_regs_t regs = {};

    bcm_emmc_regs_t *regs_ptr = bcm_emmc_regs_get((uintptr_t) &regs);
    ASSERT_EQ((uintptr_t) &regs, (uintptr_t) regs_ptr);
}



