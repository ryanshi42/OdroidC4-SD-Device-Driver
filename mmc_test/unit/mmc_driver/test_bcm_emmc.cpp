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
FAKE_VALUE_FUNC(result_t, bcm_emmc_regs_set_max_data_timeout, bcm_emmc_regs_t *)
FAKE_VALUE_FUNC(result_t, bcm_emmc_regs_enable_internal_clock, bcm_emmc_regs_t *)
FAKE_VALUE_FUNC(result_t, bcm_emmc_regs_is_data_lines_busy, bcm_emmc_regs_t *, bool *)
FAKE_VALUE_FUNC(result_t, bcm_emmc_regs_is_cmd_line_busy, bcm_emmc_regs_t *, bool *)
FAKE_VALUE_FUNC(result_t, bcm_emmc_regs_disable_sd_clock, bcm_emmc_regs_t *)
FAKE_VALUE_FUNC(result_t, bcm_emmc_regs_enable_sd_clock, bcm_emmc_regs_t *)
FAKE_VALUE_FUNC(result_t, bcm_emmc_regs_get_host_controller_spec_version, bcm_emmc_regs_t *, uint8_t *)
FAKE_VALUE_FUNC(result_t, bcm_emmc_regs_set_sd_clock_mode_as_divided, bcm_emmc_regs_t *)
FAKE_VALUE_FUNC(result_t, bcm_emmc_regs_set_sd_clock_divisor, bcm_emmc_regs_t *, uint16_t)
FAKE_VALUE_FUNC(result_t, bcm_emmc_regs_is_sd_clock_stable, bcm_emmc_regs_t *, bool *)
FAKE_VALUE_FUNC(result_t, bcm_emmc_regs_enable_interrupts, bcm_emmc_regs_t *)
FAKE_VALUE_FUNC(result_t, bcm_emmc_regs_mask_interrupt, bcm_emmc_regs_t *, uint32_t, bool *)
FAKE_VALUE_FUNC(result_t, bcm_emmc_regs_get_interrupt_raw32, bcm_emmc_regs_t *, uint32_t *)
FAKE_VALUE_FUNC(result_t, bcm_emmc_regs_set_interrupt_raw32, bcm_emmc_regs_t *, uint32_t)
FAKE_VALUE_FUNC(result_t, bcm_emmc_regs_is_cmd_timeout_err, bcm_emmc_regs_t *, bool *)
FAKE_VALUE_FUNC(result_t, bcm_emmc_regs_is_data_timeout_err, bcm_emmc_regs_t *, bool *)
FAKE_VALUE_FUNC(result_t, bcm_emmc_regs_is_any_err, bcm_emmc_regs_t *, bool *)
FAKE_VALUE_FUNC(result_t, bcm_emmc_regs_is_cmd_in_progress, bcm_emmc_regs_t *, bool *)
FAKE_VALUE_FUNC(result_t, bcm_emmc_regs_is_data_in_progress, bcm_emmc_regs_t *, bool *)
FAKE_VALUE_FUNC(result_t, bcm_emmc_regs_clear_interrupt, bcm_emmc_regs_t *)
FAKE_VALUE_FUNC(result_t, bcm_emmc_regs_set_arg1, bcm_emmc_regs_t *, uint32_t)
FAKE_VALUE_FUNC(result_t, bcm_emmc_regs_set_cmdtm, bcm_emmc_regs_t *, cmdtm_t)
FAKE_VALUE_FUNC(result_t, bcm_emmc_regs_get_resp0, bcm_emmc_regs_t *, uint32_t *)

/* Resets all Fakes for each unit test. */
class test_bcm_emmc_reset : public testing::Test {
protected:
    // You can define per-test set-up logic as usual.
    void SetUp() override {
        RESET_FAKE(bcm_emmc_regs_zero_control0);
        RESET_FAKE(bcm_emmc_regs_zero_control1);
        RESET_FAKE(bcm_emmc_regs_reset_host_circuit);
        RESET_FAKE(bcm_emmc_regs_is_host_circuit_reset);
        RESET_FAKE(bcm_emmc_regs_set_max_data_timeout);
        RESET_FAKE(bcm_emmc_regs_enable_internal_clock);
        RESET_FAKE(bcm_emmc_regs_is_data_lines_busy);
        RESET_FAKE(bcm_emmc_regs_is_cmd_line_busy);
        RESET_FAKE(bcm_emmc_regs_disable_sd_clock);
        RESET_FAKE(bcm_emmc_regs_enable_sd_clock);
        RESET_FAKE(bcm_emmc_regs_get_host_controller_spec_version);
        RESET_FAKE(bcm_emmc_regs_set_sd_clock_mode_as_divided);
        RESET_FAKE(bcm_emmc_regs_set_sd_clock_divisor);
        RESET_FAKE(bcm_emmc_regs_is_sd_clock_stable);
        RESET_FAKE(bcm_emmc_regs_enable_interrupts);
        RESET_FAKE(bcm_emmc_regs_mask_interrupt);
        RESET_FAKE(bcm_emmc_regs_get_interrupt_raw32);
        RESET_FAKE(bcm_emmc_regs_set_interrupt_raw32);
        RESET_FAKE(bcm_emmc_regs_is_cmd_timeout_err);
        RESET_FAKE(bcm_emmc_regs_is_data_timeout_err);
        RESET_FAKE(bcm_emmc_regs_is_any_err);
        RESET_FAKE(bcm_emmc_regs_is_cmd_in_progress);
        RESET_FAKE(bcm_emmc_regs_is_data_in_progress);
        RESET_FAKE(bcm_emmc_regs_clear_interrupt);
        RESET_FAKE(bcm_emmc_regs_set_arg1);
        RESET_FAKE(bcm_emmc_regs_set_cmdtm);
        RESET_FAKE(bcm_emmc_regs_get_resp0);
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

    bcm_emmc_regs_is_sd_clock_stable_fake.custom_fake = [](bcm_emmc_regs_t *regs, bool *ret_val) {
        *ret_val = true;
        return result_ok();
    };

    bcm_emmc_regs_t regs = {};
    bcm_emmc_t bcm_emmc = {};
    result_t res = bcm_emmc_init(&bcm_emmc, &regs, NULL);
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
    result_t res = bcm_emmc_init(&bcm_emmc, &regs, NULL);
    ASSERT_FALSE(result_is_ok(res));
    ASSERT_STREQ(
            "Host circuit did not reset in bcm_emmc_init().",
            result_get_last_err_msg(res)
    );
}

TEST(test_result, init_should_timeout_if_cmd_line_is_busy) {
    bcm_emmc_regs_is_host_circuit_reset_fake.custom_fake = [](bcm_emmc_regs_t *regs, bool *ret_val) {
        *ret_val = true;
        return result_ok();
    };

    bcm_emmc_regs_is_data_lines_busy_fake.custom_fake = [](bcm_emmc_regs_t *regs, bool *ret_val) {
        *ret_val = false;
        return result_ok();
    };

    bcm_emmc_regs_is_cmd_line_busy_fake.custom_fake = [](bcm_emmc_regs_t *regs, bool *ret_val) {
        *ret_val = true;
        return result_ok();
    };

    bcm_emmc_regs_t regs = {};
    bcm_emmc_t bcm_emmc = {};
    result_t res = bcm_emmc_init(&bcm_emmc, &regs, NULL);
    ASSERT_TRUE(result_is_err(res));
    ASSERT_STREQ(
            "Failed to set clock to low-speed setup frequency in bcm_emmc_init().",
            result_get_last_err_msg(res)
    );
}

TEST(test_result, init_should_timeout_if_data_lines_is_busy) {
    bcm_emmc_regs_is_host_circuit_reset_fake.custom_fake = [](bcm_emmc_regs_t *regs, bool *ret_val) {
        *ret_val = true;
        return result_ok();
    };

    bcm_emmc_regs_is_data_lines_busy_fake.custom_fake = [](bcm_emmc_regs_t *regs, bool *ret_val) {
        *ret_val = true;
        return result_ok();
    };

    bcm_emmc_regs_is_cmd_line_busy_fake.custom_fake = [](bcm_emmc_regs_t *regs, bool *ret_val) {
        *ret_val = false;
        return result_ok();
    };

    bcm_emmc_regs_t regs = {};
    bcm_emmc_t bcm_emmc = {};
    result_t res = bcm_emmc_init(&bcm_emmc, &regs, NULL);
    ASSERT_TRUE(result_is_err(res));
    ASSERT_STREQ(
            "Failed to set clock to low-speed setup frequency in bcm_emmc_init().",
            result_get_last_err_msg(res)
    );
}

TEST(test_result, init_should_timeout_if_cmd_and_data_lines_is_busy) {
    bcm_emmc_regs_is_host_circuit_reset_fake.custom_fake = [](bcm_emmc_regs_t *regs, bool *ret_val) {
        *ret_val = true;
        return result_ok();
    };

    bcm_emmc_regs_is_data_lines_busy_fake.custom_fake = [](bcm_emmc_regs_t *regs, bool *ret_val) {
        *ret_val = true;
        return result_ok();
    };

    bcm_emmc_regs_is_cmd_line_busy_fake.custom_fake = [](bcm_emmc_regs_t *regs, bool *ret_val) {
        *ret_val = true;
        return result_ok();
    };

    bcm_emmc_regs_t regs = {};
    bcm_emmc_t bcm_emmc = {};
    result_t res = bcm_emmc_init(&bcm_emmc, &regs, NULL);
    ASSERT_TRUE(result_is_err(res));
    ASSERT_STREQ(
            "Failed to set clock to low-speed setup frequency in bcm_emmc_init().",
            result_get_last_err_msg(res)
    );
}

