#include "gtest/gtest.h"

extern "C" {
#include "bcm_emmc.h"
}

TEST(test_result, init_should_init_bcm_emmc) {
    /* Initialise `regs` to something non-zero. */
    bcm_emmc_regs_t regs = {};
    memset((void *) &regs, 0xFF, sizeof(regs));
    uint32_t control0;
    control0_get_raw32(&regs.control0, &control0);
    ASSERT_EQ(0xFFFFFFFF, control0);

    /* Initialise bcm_emmc_init. */
    bcm_emmc_t bcm_emmc = {};
    result_t res = bcm_emmc_init(&bcm_emmc, (uintptr_t) &regs);
    ASSERT_TRUE(result_is_ok(res));

    /* The `regs` variable should be set to `regs`. */
    ASSERT_EQ((uintptr_t) &regs, (uintptr_t) bcm_emmc.regs);

    /* The `control0` register should be zeroed. */
    control0_get_raw32(&regs.control0, &control0);
    ASSERT_EQ(0, control0);
}


