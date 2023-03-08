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
class test_sdhci_reset : public testing::Test {
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

/* get_sd_clock_divisor */

TEST(test_sdhci, get_sd_clock_divisor_should_return_err_if_freq_is_zero) {
    uint32_t divisor = 0;
    bcm_emmc_regs_t bcm_emmc_regs = {};
    result_t res = sdhci_get_sd_clock_divisor(
            &bcm_emmc_regs,
            0,
            &divisor
    );
    ASSERT_TRUE(result_is_err(res));
    ASSERT_STREQ(
            "Zero `freq` passed to sdhci_get_sd_clock_divisor().",
            result_get_last_err_msg(res)
    );
}

TEST(test_sdhci, get_sd_clock_divisor_should_return_correct_divisors_for_version_3) {
    bcm_emmc_regs_get_host_controller_spec_version_fake.custom_fake = [](bcm_emmc_regs_t *regs, uint8_t *ret_val) {
        *ret_val = 3;
        return result_ok();
    };
    auto input_outputs = std::vector<std::pair<uint32_t, uint32_t>>{
            /* sd.c: sd_clk divisor 0x00000068, shift 0x00000006 */
            {400000, 105},
            /* sd.c: sd_clk divisor 0x00000002, shift 0x00000000 */
            {25000000, 4},
    };
    for (auto input_output : input_outputs) {
        uint32_t divisor = 0;
        bcm_emmc_regs_t bcm_emmc_regs = {};
        result_t res = sdhci_get_sd_clock_divisor(
                &bcm_emmc_regs,
                input_output.first,
                &divisor
        );
        ASSERT_TRUE(result_is_ok(res));
        ASSERT_EQ(input_output.second, divisor);
    }
}

/* wait_for_interrupt. */

TEST(test_sdhci, wait_for_interrupt_should_timeout) {
    uint32_t interrupt_mask = INT_CMD_DONE;
    bcm_emmc_regs_t bcm_emmc_regs = {};
    sdhci_result_t sdhci_result;
    result_t res = sdhci_wait_for_interrupt(
            &bcm_emmc_regs,
            interrupt_mask,
            &sdhci_result
    );
    ASSERT_TRUE(result_is_err(res));
    ASSERT_TRUE(sdhci_result == SD_TIMEOUT);
}

TEST(test_sdhci, wait_for_interrupt_should_return_with_error_if_error) {
    /* TODO: Test for error case. */
}

