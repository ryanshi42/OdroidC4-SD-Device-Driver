#include "gtest/gtest.h"
#include <fff/fff.h>

extern "C" {
#include "bcm_emmc.h"
}

DEFINE_FFF_GLOBALS;

FAKE_VALUE_FUNC(result_t, sdhci_regs_zero_control0, sdhci_regs_t *)
FAKE_VALUE_FUNC(result_t, sdhci_regs_zero_control1, sdhci_regs_t *)
FAKE_VALUE_FUNC(result_t, sdhci_regs_reset_host_circuit, sdhci_regs_t *)
FAKE_VALUE_FUNC(result_t, sdhci_regs_is_host_circuit_reset, sdhci_regs_t *, bool *)
FAKE_VALUE_FUNC(result_t, sdhci_regs_set_max_data_timeout, sdhci_regs_t *)
FAKE_VALUE_FUNC(result_t, sdhci_regs_enable_internal_clock, sdhci_regs_t *)
FAKE_VALUE_FUNC(result_t, sdhci_regs_is_data_lines_busy, sdhci_regs_t *, bool *)
FAKE_VALUE_FUNC(result_t, sdhci_regs_is_cmd_line_busy, sdhci_regs_t *, bool *)
FAKE_VALUE_FUNC(result_t, sdhci_regs_disable_sd_clock, sdhci_regs_t *)
FAKE_VALUE_FUNC(result_t, sdhci_regs_enable_sd_clock, sdhci_regs_t *)
FAKE_VALUE_FUNC(result_t, sdhci_regs_get_host_controller_spec_version, sdhci_regs_t *, uint8_t *)
FAKE_VALUE_FUNC(result_t, sdhci_regs_set_sd_clock_mode_as_divided, sdhci_regs_t *)
FAKE_VALUE_FUNC(result_t, sdhci_regs_set_sd_clock_divisor, sdhci_regs_t *, uint16_t)
FAKE_VALUE_FUNC(result_t, sdhci_regs_is_sd_clock_stable, sdhci_regs_t *, bool *)
FAKE_VALUE_FUNC(result_t, sdhci_regs_enable_interrupts, sdhci_regs_t *)
FAKE_VALUE_FUNC(result_t, sdhci_regs_mask_interrupt, sdhci_regs_t *, uint32_t, bool *)
FAKE_VALUE_FUNC(result_t, sdhci_regs_get_interrupt_raw32, sdhci_regs_t *, uint32_t *)
FAKE_VALUE_FUNC(result_t, sdhci_regs_set_interrupt_raw32, sdhci_regs_t *, uint32_t)
FAKE_VALUE_FUNC(result_t, sdhci_regs_is_cmd_timeout_err, sdhci_regs_t *, bool *)
FAKE_VALUE_FUNC(result_t, sdhci_regs_is_data_timeout_err, sdhci_regs_t *, bool *)
FAKE_VALUE_FUNC(result_t, sdhci_regs_is_any_err, sdhci_regs_t *, bool *)
FAKE_VALUE_FUNC(result_t, sdhci_regs_is_cmd_in_progress, sdhci_regs_t *, bool *)
FAKE_VALUE_FUNC(result_t, sdhci_regs_is_data_in_progress, sdhci_regs_t *, bool *)
FAKE_VALUE_FUNC(result_t, sdhci_regs_clear_interrupt, sdhci_regs_t *)
FAKE_VALUE_FUNC(result_t, sdhci_regs_set_arg1, sdhci_regs_t *, uint32_t)
FAKE_VALUE_FUNC(result_t, sdhci_regs_set_cmdtm, sdhci_regs_t *, cmdtm_t)
FAKE_VALUE_FUNC(result_t, sdhci_regs_get_resp0, sdhci_regs_t *, uint32_t *)
FAKE_VALUE_FUNC(result_t, sdhci_regs_get_resp1, sdhci_regs_t *, uint32_t *)
FAKE_VALUE_FUNC(result_t, sdhci_regs_get_resp2, sdhci_regs_t *, uint32_t *)
FAKE_VALUE_FUNC(result_t, sdhci_regs_get_resp3, sdhci_regs_t *, uint32_t *)
FAKE_VALUE_FUNC(result_t, sdhci_regs_set_block_size, sdhci_regs_t *, uint32_t)
FAKE_VALUE_FUNC(result_t, sdhci_regs_set_block_count, sdhci_regs_t *, uint32_t)
FAKE_VALUE_FUNC(result_t, sdhci_regs_is_read_ready, sdhci_regs_t *, bool *)
FAKE_VALUE_FUNC(result_t, sdhci_regs_is_write_ready, sdhci_regs_t *, bool *)
FAKE_VALUE_FUNC(result_t, sdhci_regs_get_data, sdhci_regs_t *, uint32_t *)
FAKE_VALUE_FUNC(result_t, sdhci_regs_set_data, sdhci_regs_t *, uint32_t)
FAKE_VALUE_FUNC(result_t, sdhci_regs_set_bus_width_4, sdhci_regs_t *, bool)

/* Resets all Fakes for each unit test. */
class TestBcmEmmc : public testing::Test {
protected:
    // You can define per-test set-up logic as usual.
    void SetUp() override {
        FFF_RESET_HISTORY();

        RESET_FAKE(sdhci_regs_zero_control0);
        RESET_FAKE(sdhci_regs_zero_control1);
        RESET_FAKE(sdhci_regs_reset_host_circuit);
        RESET_FAKE(sdhci_regs_is_host_circuit_reset);
        RESET_FAKE(sdhci_regs_set_max_data_timeout);
        RESET_FAKE(sdhci_regs_enable_internal_clock);
        RESET_FAKE(sdhci_regs_is_data_lines_busy);
        RESET_FAKE(sdhci_regs_is_cmd_line_busy);
        RESET_FAKE(sdhci_regs_disable_sd_clock);
        RESET_FAKE(sdhci_regs_enable_sd_clock);
        RESET_FAKE(sdhci_regs_get_host_controller_spec_version);
        RESET_FAKE(sdhci_regs_set_sd_clock_mode_as_divided);
        RESET_FAKE(sdhci_regs_set_sd_clock_divisor);
        RESET_FAKE(sdhci_regs_is_sd_clock_stable);
        RESET_FAKE(sdhci_regs_enable_interrupts);
        RESET_FAKE(sdhci_regs_mask_interrupt);
        RESET_FAKE(sdhci_regs_get_interrupt_raw32);
        RESET_FAKE(sdhci_regs_set_interrupt_raw32);
        RESET_FAKE(sdhci_regs_is_cmd_timeout_err);
        RESET_FAKE(sdhci_regs_is_data_timeout_err);
        RESET_FAKE(sdhci_regs_is_any_err);
        RESET_FAKE(sdhci_regs_is_cmd_in_progress);
        RESET_FAKE(sdhci_regs_is_data_in_progress);
        RESET_FAKE(sdhci_regs_clear_interrupt);
        RESET_FAKE(sdhci_regs_set_arg1);
        RESET_FAKE(sdhci_regs_set_cmdtm);
        RESET_FAKE(sdhci_regs_get_resp0);
        RESET_FAKE(sdhci_regs_get_resp1);
        RESET_FAKE(sdhci_regs_get_resp2);
        RESET_FAKE(sdhci_regs_get_resp3);
        RESET_FAKE(sdhci_regs_set_block_size);
        RESET_FAKE(sdhci_regs_set_block_count);
        RESET_FAKE(sdhci_regs_is_read_ready);
        RESET_FAKE(sdhci_regs_is_write_ready);
        RESET_FAKE(sdhci_regs_get_data);
        RESET_FAKE(sdhci_regs_set_data);
        RESET_FAKE(sdhci_regs_set_bus_width_4);
    }

    // You can define per-test tear-down logic as usual.
    void TearDown() override {

    }
};

TEST_F(TestBcmEmmc, init_should_init_bcm_emmc) {
    sdhci_regs_is_host_circuit_reset_fake.custom_fake = [](sdhci_regs_t *regs, bool *ret_val) {
        *ret_val = true;
        return result_ok();
    };

    sdhci_regs_is_sd_clock_stable_fake.custom_fake = [](sdhci_regs_t *regs, bool *ret_val) {
        *ret_val = true;
        return result_ok();
    };

    result_t(*mask_interrupt_mocks[])(sdhci_regs_t * regs, uint32_t
    mask, bool * ret_val) = {
        /* Mask interrupt should return true the first time, which exits us out of the while-loop immediately. */
        [](sdhci_regs_t *regs, uint32_t mask, bool *ret_val) {
            *ret_val = true;
            return result_ok();
        },
                /* Mask interrupt returns false the second time for the not an error case. */
                [](sdhci_regs_t *regs, uint32_t mask, bool *ret_val) {
                    *ret_val = false;
                    return result_ok();
                },
                [](sdhci_regs_t *regs, uint32_t mask, bool *ret_val) {
                    *ret_val = true;
                    return result_ok();
                },
                /* Mask interrupt returns false the second time for the not an error case. */
                [](sdhci_regs_t *regs, uint32_t mask, bool *ret_val) {
                    *ret_val = false;
                    return result_ok();
                },
    };

    sdhci_regs_get_resp0_fake.custom_fake = [](sdhci_regs_t *regs, uint32_t *ret_val) {
        *ret_val = 0x000001AA;
        return result_ok();
    };

    SET_CUSTOM_FAKE_SEQ(sdhci_regs_mask_interrupt, mask_interrupt_mocks, 4);

    sdhci_regs_t emmc_regs = {};
    bcm_gpio_regs_t gpio_regs = {};
    result_t res = bcm_emmc_init(&emmc_regs, &gpio_regs);
    result_printf(res);
    ASSERT_TRUE(result_is_ok(res));

}

TEST_F(TestBcmEmmc, init_should_timeout_if_host_circuit_reset_fails) {
    sdhci_regs_is_host_circuit_reset_fake.custom_fake = [](sdhci_regs_t *regs, bool *ret_val) {
        *ret_val = false;
        return result_ok();
    };

    sdhci_regs_t emmc_regs = {};
    bcm_gpio_regs_t gpio_regs = {};
    result_t res = bcm_emmc_init(&emmc_regs, &gpio_regs);
    ASSERT_FALSE(result_is_ok(res));
    ASSERT_STREQ(
            "Host circuit did not reset in bcm_emmc_init().",
            result_get_last_err_msg(res)
    );
}

TEST_F(TestBcmEmmc, init_should_timeout_if_cmd_line_is_busy) {
    sdhci_regs_is_host_circuit_reset_fake.custom_fake = [](sdhci_regs_t *regs, bool *ret_val) {
        *ret_val = true;
        return result_ok();
    };

    sdhci_regs_is_data_lines_busy_fake.custom_fake = [](sdhci_regs_t *regs, bool *ret_val) {
        *ret_val = false;
        return result_ok();
    };

    sdhci_regs_is_cmd_line_busy_fake.custom_fake = [](sdhci_regs_t *regs, bool *ret_val) {
        *ret_val = true;
        return result_ok();
    };

    sdhci_regs_t emmc_regs = {};
    bcm_gpio_regs_t gpio_regs = {};
    result_t res = bcm_emmc_init(&emmc_regs, &gpio_regs);
    ASSERT_TRUE(result_is_err(res));
    ASSERT_STREQ(
            "Failed to set clock to low-speed setup frequency in bcm_emmc_init().",
            result_get_last_err_msg(res)
    );
}

TEST_F(TestBcmEmmc, init_should_timeout_if_data_lines_is_busy) {
    sdhci_regs_is_host_circuit_reset_fake.custom_fake = [](sdhci_regs_t *regs, bool *ret_val) {
        *ret_val = true;
        return result_ok();
    };

    sdhci_regs_is_data_lines_busy_fake.custom_fake = [](sdhci_regs_t *regs, bool *ret_val) {
        *ret_val = true;
        return result_ok();
    };

    sdhci_regs_is_cmd_line_busy_fake.custom_fake = [](sdhci_regs_t *regs, bool *ret_val) {
        *ret_val = false;
        return result_ok();
    };

    sdhci_regs_t emmc_regs = {};
    bcm_gpio_regs_t gpio_regs = {};
    result_t res = bcm_emmc_init(&emmc_regs, &gpio_regs);
    ASSERT_TRUE(result_is_err(res));
    ASSERT_STREQ(
            "Failed to set clock to low-speed setup frequency in bcm_emmc_init().",
            result_get_last_err_msg(res)
    );
}

TEST_F(TestBcmEmmc, init_should_timeout_if_cmd_and_data_lines_is_busy) {
    sdhci_regs_is_host_circuit_reset_fake.custom_fake = [](sdhci_regs_t *regs, bool *ret_val) {
        *ret_val = true;
        return result_ok();
    };

    sdhci_regs_is_data_lines_busy_fake.custom_fake = [](sdhci_regs_t *regs, bool *ret_val) {
        *ret_val = true;
        return result_ok();
    };

    sdhci_regs_is_cmd_line_busy_fake.custom_fake = [](sdhci_regs_t *regs, bool *ret_val) {
        *ret_val = true;
        return result_ok();
    };

    sdhci_regs_t emmc_regs = {};
    bcm_gpio_regs_t gpio_regs = {};
    result_t res = bcm_emmc_init(&emmc_regs, &gpio_regs);
    ASSERT_TRUE(result_is_err(res));
    ASSERT_STREQ(
            "Failed to set clock to low-speed setup frequency in bcm_emmc_init().",
            result_get_last_err_msg(res)
    );
}

