#include "gtest/gtest.h"
#include <fff/fff.h>

extern "C" {
#include "bcm_emmc.h"
}

DEFINE_FFF_GLOBALS;

FAKE_VALUE_FUNC(result_t, bcm_emmc_regs_zero_control0, bcm_emmc_regs_t *)
FAKE_VALUE_FUNC(result_t, bcm_emmc_regs_zero_control1, bcm_emmc_regs_t *)
FAKE_VALUE_FUNC(result_t, bcm_emmc_regs_reset_host_circuit, bcm_emmc_regs_t *)
FAKE_VALUE_FUNC(result_t, bcm_emmc_regs_is_host_circuit_reset, bcm_emmc_regs_t *, bool *)

/* Resets all Fakes for each unit test. */
class test_bcm_emmc_reset : public testing::Test {
protected:
    // You can define per-test set-up logic as usual.
    void SetUp() override {
        RESET_FAKE(bcm_emmc_regs_zero_control0);
        RESET_FAKE(bcm_emmc_regs_zero_control1);
        RESET_FAKE(bcm_emmc_regs_reset_host_circuit);
        RESET_FAKE(bcm_emmc_regs_is_host_circuit_reset);
    }

    // You can define per-test tear-down logic as usual.
    void TearDown() override {

    }
};

TEST(test_result, init_should_init_bcm_emmc) {
    bcm_emmc_regs_is_host_circuit_reset_fake.custom_fake = [](bcm_emmc_regs_t *regs, bool *ret_val) {
        *ret_val = true;
        return result_ok();
    };

    bcm_emmc_regs_t regs = {};
    bcm_emmc_t bcm_emmc = {};
    result_t res = bcm_emmc_init(&bcm_emmc, &regs);
    ASSERT_TRUE(result_is_ok(res));

    /* The `regs` variable should be set to `regs`. */
    ASSERT_EQ((uintptr_t) &regs, (uintptr_t) bcm_emmc.regs);
}

TEST(test_result, init_should_timeout_if_host_circuit_reset_fails) {
    bcm_emmc_regs_is_host_circuit_reset_fake.custom_fake = [](bcm_emmc_regs_t *regs, bool *ret_val) {
        *ret_val = false;
        return result_ok();
    };

    bcm_emmc_regs_t regs = {};
    bcm_emmc_t bcm_emmc = {};
    result_t res = bcm_emmc_init(&bcm_emmc, &regs);
    ASSERT_FALSE(result_is_ok(res));
    ASSERT_STREQ(
            "Host circuit did not reset in bcm_emmc_init().",
            result_get_last_err_msg(res)
    );
}

