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
FAKE_VALUE_FUNC(result_t, bcm_emmc_regs_get_resp1, bcm_emmc_regs_t *, uint32_t *)
FAKE_VALUE_FUNC(result_t, bcm_emmc_regs_get_resp2, bcm_emmc_regs_t *, uint32_t *)
FAKE_VALUE_FUNC(result_t, bcm_emmc_regs_get_resp3, bcm_emmc_regs_t *, uint32_t *)
FAKE_VALUE_FUNC(result_t, bcm_emmc_regs_set_block_size, bcm_emmc_regs_t *, uint32_t)
FAKE_VALUE_FUNC(result_t, bcm_emmc_regs_set_block_count, bcm_emmc_regs_t *, uint32_t)
FAKE_VALUE_FUNC(result_t, bcm_emmc_regs_is_read_ready, bcm_emmc_regs_t *, bool *)
FAKE_VALUE_FUNC(result_t, bcm_emmc_regs_is_write_ready, bcm_emmc_regs_t *, bool *)
FAKE_VALUE_FUNC(result_t, bcm_emmc_regs_get_data, bcm_emmc_regs_t *, uint32_t *)
FAKE_VALUE_FUNC(result_t, bcm_emmc_regs_set_data, bcm_emmc_regs_t *, uint32_t)
FAKE_VALUE_FUNC(result_t, bcm_emmc_regs_set_bus_width_4, bcm_emmc_regs_t *, bool)

FAKE_VALUE_FUNC(result_t, sdcard_init, sdcard_t *)
FAKE_VALUE_FUNC(result_t, sdcard_set_ocr_raw32, sdcard_t *, uint32_t)
FAKE_VALUE_FUNC(result_t, sdcard_get_rca, sdcard_t *, uint32_t *)
FAKE_VALUE_FUNC(result_t, sdcard_set_rca, sdcard_t *, uint32_t)
FAKE_VALUE_FUNC(result_t, sdcard_set_status, sdcard_t *, uint32_t)
FAKE_VALUE_FUNC(result_t, sdcard_has_rca, sdcard_t *, bool *)
FAKE_VALUE_FUNC(result_t, sdcard_mask_status, sdcard_t *, uint32_t, bool*)
FAKE_VALUE_FUNC(result_t, sdcard_is_app_cmd_accepted, sdcard_t *, bool *)
FAKE_VALUE_FUNC(result_t, sdcard_has_powered_up, sdcard_t *, bool *)
FAKE_VALUE_FUNC(result_t, sdcard_is_voltage_3v3, sdcard_t *, bool *)
FAKE_VALUE_FUNC(result_t, sdcard_is_high_capacity, sdcard_t *, bool *)
FAKE_VALUE_FUNC(result_t, sdcard_set_cid, sdcard_t *, uint32_t, uint32_t, uint32_t, uint32_t)
FAKE_VALUE_FUNC(result_t, sdcard_set_type, sdcard_t *, sdcard_type_t)
FAKE_VALUE_FUNC(result_t, sdcard_set_scr_raw32_lo, sdcard_t *, uint32_t)
FAKE_VALUE_FUNC(result_t, sdcard_set_scr_raw32_hi, sdcard_t *, uint32_t)
FAKE_VALUE_FUNC(result_t, sdcard_is_bus_width_4, sdcard_t *, bool *)
FAKE_VALUE_FUNC(result_t, sdcard_is_bus_width_1, sdcard_t *, bool *)
FAKE_VALUE_FUNC(result_t, sdcard_is_type_unknown, sdcard_t *, bool *)
FAKE_VALUE_FUNC(result_t, sdcard_is_type_standard_capacity, sdcard_t *, bool *)
FAKE_VALUE_FUNC(result_t, sdcard_is_type_high_capacity, sdcard_t *, bool *)
FAKE_VALUE_FUNC(result_t, sdcard_is_set_block_count_cmd_supported, sdcard_t *, bool *)

/* Resets all Fakes for each unit test. */
class TestSdhci : public testing::Test {
protected:
    // You can define per-test set-up logic as usual.
    void SetUp() override {
        FFF_RESET_HISTORY();

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
        RESET_FAKE(bcm_emmc_regs_get_resp1);
        RESET_FAKE(bcm_emmc_regs_get_resp2);
        RESET_FAKE(bcm_emmc_regs_get_resp3);
        RESET_FAKE(bcm_emmc_regs_set_block_size);
        RESET_FAKE(bcm_emmc_regs_set_block_count);
        RESET_FAKE(bcm_emmc_regs_is_read_ready);
        RESET_FAKE(bcm_emmc_regs_is_write_ready);
        RESET_FAKE(bcm_emmc_regs_get_data);
        RESET_FAKE(bcm_emmc_regs_set_data);
        RESET_FAKE(bcm_emmc_regs_set_bus_width_4);

        RESET_FAKE(sdcard_init);
        RESET_FAKE(sdcard_set_ocr_raw32);
        RESET_FAKE(sdcard_get_rca);
        RESET_FAKE(sdcard_set_rca);
        RESET_FAKE(sdcard_set_status);
        RESET_FAKE(sdcard_has_rca);
        RESET_FAKE(sdcard_mask_status);
        RESET_FAKE(sdcard_is_app_cmd_accepted);
        RESET_FAKE(sdcard_has_powered_up);
        RESET_FAKE(sdcard_is_voltage_3v3);
        RESET_FAKE(sdcard_is_high_capacity);
        RESET_FAKE(sdcard_set_cid);
        RESET_FAKE(sdcard_set_type);
        RESET_FAKE(sdcard_set_scr_raw32_lo);
        RESET_FAKE(sdcard_set_scr_raw32_hi);
        RESET_FAKE(sdcard_is_bus_width_4);
        RESET_FAKE(sdcard_is_bus_width_1);
        RESET_FAKE(sdcard_is_type_unknown);
        RESET_FAKE(sdcard_is_type_standard_capacity);
        RESET_FAKE(sdcard_is_type_high_capacity);
        RESET_FAKE(sdcard_is_set_block_count_cmd_supported);
    }

    // You can define per-test tear-down logic as usual.
    void TearDown() override {

    }
};

int find_call_history_idx(void *func) {
    for (int i = 0; i < 50; i++) {
        if (fff.call_history[i] == func) {
            return i;
        }
    }
    return -1;
}

/* get_sd_clock_divisor */

TEST_F(TestSdhci, get_sd_clock_divisor_should_return_err_if_freq_is_zero) {
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

TEST_F(TestSdhci, get_sd_clock_divisor_should_return_correct_divisors_for_version_3) {
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

TEST_F(TestSdhci, wait_for_interrupt_should_timeout) {
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

TEST_F(TestSdhci, wait_for_interrupt_should_return_with_error_if_error) {
    /* TODO: Test for error case. */
}

/* sdhci_send_cmd */

TEST_F(TestSdhci, send_cmd_should_send_non_app_cmds) {
    result_t(*mask_interrupt_mocks[])(bcm_emmc_regs_t * regs, uint32_t
    mask, bool * ret_val) = {
        /* Mask interrupt should return true the first time, which exits us out of the while-loop immediately. */
        [](bcm_emmc_regs_t *regs, uint32_t mask, bool *ret_val) {
            *ret_val = true;
            return result_ok();
        },
        /* Mask interrupt returns false the second time for the not an error case. */
        [](bcm_emmc_regs_t *regs, uint32_t mask, bool *ret_val) {
            *ret_val = false;
            return result_ok();
        },
    };

    SET_CUSTOM_FAKE_SEQ(bcm_emmc_regs_mask_interrupt, mask_interrupt_mocks, 2);

    bcm_emmc_regs_t bcm_emmc_regs = {};
    sdcard_t sdcard = {};
    sdhci_result_t sdhci_result;
    result_t res = sdhci_send_cmd(
            &bcm_emmc_regs,
            IX_GO_IDLE_STATE,
            0,
            &sdcard,
            &sdhci_result
    );
    if (result_is_err(res)) {
        result_printf(res);
    }
    ASSERT_TRUE(result_is_ok(res));

    ASSERT_EQ(fff.call_history[0], (void *) bcm_emmc_regs_is_cmd_in_progress);
    ASSERT_EQ(fff.call_history[1], (void *) bcm_emmc_regs_is_any_err);
    ASSERT_EQ(fff.call_history[2], (void *) bcm_emmc_regs_clear_interrupt);
    ASSERT_EQ(fff.call_history[3], (void *) bcm_emmc_regs_set_arg1);
    ASSERT_EQ(fff.call_history[4], (void *) bcm_emmc_regs_set_cmdtm);
    ASSERT_EQ(fff.call_history[5], (void *) bcm_emmc_regs_mask_interrupt);
    ASSERT_EQ(fff.call_history[6], (void *) bcm_emmc_regs_get_interrupt_raw32);
    ASSERT_EQ(fff.call_history[7], (void *) bcm_emmc_regs_mask_interrupt);
    ASSERT_EQ(fff.call_history[8], (void *) bcm_emmc_regs_set_interrupt_raw32);
    ASSERT_EQ(fff.call_history[9], (void *) bcm_emmc_regs_get_resp0);
}

TEST_F(TestSdhci, send_cmd_should_send_app_cmd_and_ignore_status_if_no_rca) {
    result_t(*mask_interrupt_mocks[])(bcm_emmc_regs_t * regs, uint32_t
    mask, bool * ret_val) = {
        /* Mask interrupt should return true the first time, which exits us out of the while-loop immediately. */
        [](bcm_emmc_regs_t *regs, uint32_t mask, bool *ret_val) {
            *ret_val = true;
            return result_ok();
        },
                /* Mask interrupt returns false the second time for the not an error case. */
                [](bcm_emmc_regs_t *regs, uint32_t mask, bool *ret_val) {
                    *ret_val = false;
                    return result_ok();
                },
                [](bcm_emmc_regs_t *regs, uint32_t mask, bool *ret_val) {
                    *ret_val = true;
                    return result_ok();
                },
                /* Mask interrupt returns false the second time for the not an error case. */
                [](bcm_emmc_regs_t *regs, uint32_t mask, bool *ret_val) {
                    *ret_val = false;
                    return result_ok();
                },
    };
    SET_CUSTOM_FAKE_SEQ(bcm_emmc_regs_mask_interrupt, mask_interrupt_mocks, 4);

    bcm_emmc_regs_t bcm_emmc_regs = {};
    sdcard_t sdcard = {};
    sdhci_result_t sdhci_result;
    result_t res = sdhci_send_cmd(
            &bcm_emmc_regs,
            IX_APP_SEND_OP_COND,
            0x51ff8000,
            &sdcard,
            &sdhci_result
    );
    if (result_is_err(res)) {
        result_printf(res);
    }
    ASSERT_TRUE(result_is_ok(res));

    /* We enter our if-statement in sdhci_send_cmd(). */
    ASSERT_EQ(fff.call_history[0], (void *) sdcard_has_rca);
    /* We immediately leave our if-statement in sdhci_send_cmd(). */
    ASSERT_EQ(fff.call_history[1], (void *) bcm_emmc_regs_is_cmd_in_progress);
}

TEST_F(TestSdhci, send_cmd_should_send_app_cmd_and_check_status_if_rca_present) {
    result_t(*mask_interrupt_mocks[])(bcm_emmc_regs_t * regs, uint32_t
    mask, bool * ret_val) = {
        /* Mask interrupt should return true the first time, which exits us out of the while-loop immediately. */
        [](bcm_emmc_regs_t *regs, uint32_t mask, bool *ret_val) {
            *ret_val = true;
            return result_ok();
        },
                /* Mask interrupt returns false the second time for the not an error case. */
                [](bcm_emmc_regs_t *regs, uint32_t mask, bool *ret_val) {
                    *ret_val = false;
                    return result_ok();
                },
                [](bcm_emmc_regs_t *regs, uint32_t mask, bool *ret_val) {
                    *ret_val = true;
                    return result_ok();
                },
                /* Mask interrupt returns false the second time for the not an error case. */
                [](bcm_emmc_regs_t *regs, uint32_t mask, bool *ret_val) {
                    *ret_val = false;
                    return result_ok();
                },
    };
    SET_CUSTOM_FAKE_SEQ(bcm_emmc_regs_mask_interrupt, mask_interrupt_mocks, 4);

    /* Make RCA present */
    sdcard_has_rca_fake.custom_fake = [](sdcard_t *sdcard, bool *ret_val) {
        *ret_val = true;
        return result_ok();
    };

    /* Make the APP_CMD accepted. */
    sdcard_is_app_cmd_accepted_fake.custom_fake = [](sdcard_t *sdcard, bool *ret_val) {
        *ret_val = true;
        return result_ok();
    };

    /* We must also return a response that is not the error response. */
    bcm_emmc_regs_get_resp0_fake.custom_fake = [](bcm_emmc_regs_t *regs, uint32_t *ret_val) {
        /* This is just ~R1_ERRORS_MASK */
        *ret_val = ~0xfff9c004;
        return result_ok();
    };

    bcm_emmc_regs_t bcm_emmc_regs = {};
    sdcard_t sdcard = {};
    sdhci_result_t sdhci_result;
    result_t res = sdhci_send_cmd(
            &bcm_emmc_regs,
            IX_APP_SEND_OP_COND,
            0x51ff8000,
            &sdcard,
            &sdhci_result
    );
    if (result_is_err(res)) {
        result_printf(res);
    }
    ASSERT_TRUE(result_is_ok(res));

    /* We enter our if-statement in sdhci_send_cmd(). */
    ASSERT_EQ(fff.call_history[0], (void *) sdcard_has_rca);
    /* We should obtain the RCA. */
    ASSERT_EQ(fff.call_history[1], (void *) sdcard_get_rca);
    /* We should eventually call `sdcard_set_status`. */
    int sdcard_set_status_idx = find_call_history_idx((void *) sdcard_set_status);
    ASSERT_NE(sdcard_set_status_idx, -1);
    /* We should eventually call `sdcard_is_app_cmd_accepted`. */
    int sdcard_is_app_cmd_accepted_idx = find_call_history_idx((void *) sdcard_is_app_cmd_accepted);
    ASSERT_NE(sdcard_is_app_cmd_accepted_idx, -1);
    /* `sdcard_is_app_cmd_accepted` should be called after `sdcard_set_status` */
    printf("sdcard_set_status_idx: %d", sdcard_set_status_idx);
    printf("sdcard_is_app_cmd_accepted_idx: %d", sdcard_is_app_cmd_accepted_idx);
    ASSERT_TRUE(sdcard_is_app_cmd_accepted_idx > sdcard_set_status_idx);
}

/* sdhci_transfer_blocks */

TEST_F(TestSdhci, sdhci_transfer_blocks_should_return_err_if_sdcard_type_is_unknown) {
    /* Return True for `is_type_unknown`.. */
    sdcard_is_type_unknown_fake.custom_fake = [](sdcard_t *sdcard, bool *ret_val) {
        *ret_val = true;
        return result_ok();
    };
    bcm_emmc_regs_t bcm_emmc_regs = {};
    sdcard_t sdcard = {};
    sdhci_result_t sdhci_result;
    char buf[512];
    result_t res = sdhci_transfer_blocks(
            &bcm_emmc_regs,
            &sdcard,
            0,
            0,
            512,
            true,
            buf,
            sizeof(buf),
            &sdhci_result
    );
    ASSERT_TRUE(result_is_err(res));
    ASSERT_TRUE(sdhci_result == SD_NO_RESP);
}

