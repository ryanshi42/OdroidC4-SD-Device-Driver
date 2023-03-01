#include "gtest/gtest.h"
#include <fff/fff.h>

extern "C" {
#include "bcm_emmc_regs.h"
}

DEFINE_FFF_GLOBALS;

FAKE_VALUE_FUNC(result_t, control1_set_raw32, control1_t *, uint32_t);
FAKE_VALUE_FUNC(result_t, control1_get_raw32, control1_t *, uint32_t *);
FAKE_VALUE_FUNC(result_t, control1_set_srst_hc, control1_t *, bool);
FAKE_VALUE_FUNC(result_t, control1_get_srst_hc, control1_t *, bool *);
FAKE_VALUE_FUNC(result_t, control1_get_srst_cmd, control1_t *, bool *);
FAKE_VALUE_FUNC(result_t, control1_get_srst_data, control1_t *,bool *);

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

/* is_host_circuit_reset */

TEST(test_bcm_emmc_regs, is_host_circuit_reset_should_return_true_if_host_circuit_is_reset) {
    bcm_emmc_regs_t regs = {};

    control1_get_srst_hc_fake.custom_fake = [](control1_t *control1, bool *ret_val) {
        *ret_val = false;
        return result_ok();
    };

    bool is_host_circuit_reset;
    result_t res = bcm_emmc_regs_is_host_circuit_reset(&regs, &is_host_circuit_reset);
    ASSERT_TRUE(result_is_ok(res));
    ASSERT_TRUE(is_host_circuit_reset);
}

TEST(test_bcm_emmc_regs, is_host_circuit_reset_should_return_false_if_host_circuit_is_reset) {
    bcm_emmc_regs_t regs = {};

    control1_get_srst_hc_fake.custom_fake = [](control1_t *control1, bool *ret_val) {
        *ret_val = true;
        return result_ok();
    };

    bool is_host_circuit_reset;
    result_t res = bcm_emmc_regs_is_host_circuit_reset(&regs, &is_host_circuit_reset);
    ASSERT_TRUE(result_is_ok(res));
    ASSERT_FALSE(is_host_circuit_reset);
}



