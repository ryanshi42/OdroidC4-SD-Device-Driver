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
FAKE_VALUE_FUNC(result_t, sdcard_set_sdcard_data, sdcard_t *, uint32_t, uint32_t, uint32_t, uint32_t)
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
        RESET_FAKE(sdcard_set_sdcard_data);
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
    sdhci_regs_t sdhci_regs = {};
    result_t res = sdhci_get_sd_clock_divisor(
            &sdhci_regs,
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
    sdhci_regs_get_host_controller_spec_version_fake.custom_fake = [](sdhci_regs_t *regs, uint8_t *ret_val) {
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
        sdhci_regs_t sdhci_regs = {};
        result_t res = sdhci_get_sd_clock_divisor(
                &sdhci_regs,
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
    sdhci_regs_t sdhci_regs = {};
    sdhci_result_t sdhci_result;
    result_t res = sdhci_wait_for_interrupt(
            &sdhci_regs,
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
    };

    SET_CUSTOM_FAKE_SEQ(sdhci_regs_mask_interrupt, mask_interrupt_mocks, 2);

    sdhci_regs_t sdhci_regs = {};
    sdcard_t sdcard = {};
    sdhci_result_t sdhci_result;
    result_t res = sdhci_send_cmd(
            &sdhci_regs,
            IDX_GO_IDLE_STATE,
            0,
            &sdcard,
            &sdhci_result
    );
    if (result_is_err(res)) {
        result_printf(res);
    }
    ASSERT_TRUE(result_is_ok(res));

    ASSERT_EQ(fff.call_history[0], (void *) sdhci_regs_is_cmd_in_progress);
    ASSERT_EQ(fff.call_history[1], (void *) sdhci_regs_is_any_err);
    ASSERT_EQ(fff.call_history[2], (void *) sdhci_regs_clear_interrupt);
    ASSERT_EQ(fff.call_history[3], (void *) sdhci_regs_set_arg1);
    ASSERT_EQ(fff.call_history[4], (void *) sdhci_regs_set_cmdtm);
    ASSERT_EQ(fff.call_history[5], (void *) sdhci_regs_mask_interrupt);
    ASSERT_EQ(fff.call_history[6], (void *) sdhci_regs_get_interrupt_raw32);
    ASSERT_EQ(fff.call_history[7], (void *) sdhci_regs_mask_interrupt);
    ASSERT_EQ(fff.call_history[8], (void *) sdhci_regs_set_interrupt_raw32);
    ASSERT_EQ(fff.call_history[9], (void *) sdhci_regs_get_resp0);
}

TEST_F(TestSdhci, send_cmd_should_send_app_cmd_and_ignore_status_if_no_rca) {
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
    SET_CUSTOM_FAKE_SEQ(sdhci_regs_mask_interrupt, mask_interrupt_mocks, 4);

    sdhci_regs_t sdhci_regs = {};
    sdcard_t sdcard = {};
    sdhci_result_t sdhci_result;
    result_t res = sdhci_send_cmd(
            &sdhci_regs,
            IDX_APP_SEND_OP_COND,
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
    ASSERT_EQ(fff.call_history[1], (void *) sdhci_regs_is_cmd_in_progress);
}

TEST_F(TestSdhci, send_cmd_should_send_app_cmd_and_check_status_if_rca_present) {
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
    SET_CUSTOM_FAKE_SEQ(sdhci_regs_mask_interrupt, mask_interrupt_mocks, 4);

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
    sdhci_regs_get_resp0_fake.custom_fake = [](sdhci_regs_t *regs, uint32_t *ret_val) {
        /* This is just ~R1_ERRORS_MASK */
        *ret_val = ~0xfff9c004;
        return result_ok();
    };

    sdhci_regs_t sdhci_regs = {};
    sdcard_t sdcard = {};
    sdhci_result_t sdhci_result;
    result_t res = sdhci_send_cmd(
            &sdhci_regs,
            IDX_APP_SEND_OP_COND,
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
    /* Return True for `is_type_unknown`. */
    sdcard_is_type_unknown_fake.custom_fake = [](sdcard_t *sdcard, bool *ret_val) {
        *ret_val = true;
        return result_ok();
    };
    sdhci_regs_t sdhci_regs = {};
    sdcard_t sdcard = {};
    sdhci_result_t sdhci_result;
    char buf[512];
    result_t res = sdhci_transfer_blocks(
            &sdhci_regs,
            &sdcard,
            0,
            1,
            512,
            true,
            buf,
            sizeof(buf),
            &sdhci_result
    );
    ASSERT_TRUE(result_is_err(res));
    ASSERT_TRUE(sdhci_result == SD_NO_RESP);
}

TEST_F(TestSdhci, sdhci_transfer_blocks_should_reject_non_word_aligned_buffer_sizes) {
    sdhci_regs_t sdhci_regs = {};
    sdcard_t sdcard = {};
    sdhci_result_t sdhci_result;
    auto buf_sizes = std::vector<size_t>{1, 2, 3, 5, 6, 7, 9, 10, 11, 13, 14, 15};
    for (auto buf_size : buf_sizes) {
        char buf[512];
        result_t res = sdhci_transfer_blocks(
                &sdhci_regs,
                &sdcard,
                0,
                0,
                512,
                true,
                buf,
                buf_size,
                &sdhci_result
        );
        ASSERT_TRUE(result_is_err(res));
        ASSERT_TRUE(sdhci_result == SD_ERROR);
        ASSERT_STREQ(
                "Buffer length is not a multiple of 4 in sdhci_transfer_blocks().",
                result_get_last_err_msg(res)
        );
    }
}

TEST_F(TestSdhci, sdhci_transfer_blocks_should_reject_insufficient_buffer_sizes) {
    sdhci_regs_t sdhci_regs = {};
    sdcard_t sdcard = {};
    sdhci_result_t sdhci_result;
    size_t block_size = 512;
    auto buf_sizes = std::vector<size_t>{1, 2, 3, 5, 6, 7, 9, 10, 11, 13, 14, 15};
    for (size_t i = 1; i < 10; i++) {
        int delta = -1 * (int) sizeof(uint32_t);
        if (i % 2 == 0) {
            delta = 1 * (int) sizeof(uint32_t);
        }
        char buf[512];
        result_t res = sdhci_transfer_blocks(
                &sdhci_regs,
                &sdcard,
                0,
                i,
                block_size,
                true,
                buf,
                (i * block_size) + delta,
                &sdhci_result
        );
        ASSERT_TRUE(result_is_err(res));
        ASSERT_TRUE(sdhci_result == SD_ERROR);
        ASSERT_STREQ(
                "Buffer length is not equal to the number of blocks times the block size in sdhci_transfer_blocks().",
                result_get_last_err_msg(res)
        );
    }
}

TEST_F(TestSdhci, sdhci_transfer_blocks_should_correctly_write_a_single_block) {
    /* Return False for `is_type_unknown`. */
    sdcard_is_type_unknown_fake.custom_fake = [](sdcard_t *sdcard, bool *ret_val) {
        *ret_val = false;
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
    SET_CUSTOM_FAKE_SEQ(sdhci_regs_mask_interrupt, mask_interrupt_mocks, 6);

    sdhci_regs_t sdhci_regs = {};
    sdcard_t sdcard = {};
    sdhci_result_t sdhci_result;
    /* Using a smaller block size since max argument history length FFF provides is 50. */
    char buf[128];
    for (size_t i = 0; i < sizeof(buf) / sizeof(uint32_t); i++) {
        ((uint32_t *) buf)[i] = i;
    }
    result_t res = sdhci_transfer_blocks(
            &sdhci_regs,
            &sdcard,
            0,
            1,
            128,
            true,
            buf,
            sizeof(buf),
            &sdhci_result
    );
    ASSERT_TRUE(result_is_ok(res));
    ASSERT_TRUE(sdhci_result == SD_OK);

    for (size_t i = 0; i < sizeof(buf) / sizeof(uint32_t); i++) {
        ASSERT_EQ(sdhci_regs_set_data_fake.arg1_history[i], i);
    }
}

TEST_F(TestSdhci, sdhci_transfer_blocks_should_correctly_read_a_single_block) {
    /* Return False for `is_type_unknown`. */
    sdcard_is_type_unknown_fake.custom_fake = [](sdcard_t *sdcard, bool *ret_val) {
        *ret_val = false;
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
    SET_CUSTOM_FAKE_SEQ(sdhci_regs_mask_interrupt, mask_interrupt_mocks, 6);

    /* Our mock function for getting data from the data register. */
    sdhci_regs_get_data_fake.custom_fake = [](sdhci_regs_t *regs, uint32_t *ret_val) {
        /* We will return values from 0 to 31 to fill into the buffer. */
        *ret_val = sdhci_regs_get_data_fake.call_count - 1;
        return result_ok();
    };

    sdhci_regs_t sdhci_regs = {};
    sdcard_t sdcard = {};
    sdhci_result_t sdhci_result;
    /* Using a smaller block size since max argument history length FFF provides is 50. */
    char buf[128] = {0}; /* Buffer is empty to begin with. */

    result_t res = sdhci_transfer_blocks(
            &sdhci_regs,
            &sdcard,
            0,
            1,
            128,
            false,
            buf,
            sizeof(buf),
            &sdhci_result
    );
    ASSERT_TRUE(result_is_ok(res));
    ASSERT_TRUE(sdhci_result == SD_OK);

    /* Buffer should have values from 0 to 31. */
    for (size_t i = 0; i < sizeof(buf) / sizeof(uint32_t); i++) {
        ASSERT_EQ(((uint32_t *) buf)[i], i);
    }
}

TEST_F(TestSdhci, sdhci_transfer_blocks_should_correctly_write_multiple_blocks) {
    /* Return False for `is_type_unknown`. */
    sdcard_is_type_unknown_fake.custom_fake = [](sdcard_t *sdcard, bool *ret_val) {
        *ret_val = false;
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
    SET_CUSTOM_FAKE_SEQ(sdhci_regs_mask_interrupt, mask_interrupt_mocks, 10);

    sdhci_regs_t sdhci_regs = {};
    sdcard_t sdcard = {};
    sdhci_result_t sdhci_result;
    /* Using a smaller block size since max argument history length FFF provides is 50. */
    char buf[128];
    for (size_t i = 0; i < sizeof(buf) / sizeof(uint32_t); i++) {
        ((uint32_t *) buf)[i] = i;
    }
    result_t res = sdhci_transfer_blocks(
            &sdhci_regs,
            &sdcard,
            0,
            2,
            64,
            true,
            buf,
            sizeof(buf),
            &sdhci_result
    );
    ASSERT_TRUE(result_is_ok(res));
    ASSERT_TRUE(sdhci_result == SD_OK);

    for (size_t i = 0; i < sizeof(buf) / sizeof(uint32_t); i++) {
        ASSERT_EQ(sdhci_regs_set_data_fake.arg1_history[i], i);
    }
}

TEST_F(TestSdhci, sdhci_transfer_blocks_should_correctly_read_multiple_blocks) {
    /* Return False for `is_type_unknown`. */
    sdcard_is_type_unknown_fake.custom_fake = [](sdcard_t *sdcard, bool *ret_val) {
        *ret_val = false;
        return result_ok();
    };
    /* Return True for `sdcard_is_set_block_count_cmd_supported`. */
    sdcard_is_set_block_count_cmd_supported_fake.custom_fake = [](sdcard_t *sdcard, bool *ret_val) {
        *ret_val = true;
        return result_ok();
    };
    /* Return successful responses from all commands sent. */
    sdhci_regs_get_resp0_fake.custom_fake = [](sdhci_regs_t *regs, uint32_t *ret_val) {
        /* This is just ~R1_ERRORS_MASK, which means a successful response. */
        *ret_val = ~0xfff9c004;
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
    SET_CUSTOM_FAKE_SEQ(sdhci_regs_mask_interrupt, mask_interrupt_mocks, 8);

    /* Our mock function for getting data from the data register. */
    sdhci_regs_get_data_fake.custom_fake = [](sdhci_regs_t *regs, uint32_t *ret_val) {
        /* We will return values from 0 to 31 to fill into the buffer. */
        *ret_val = sdhci_regs_get_data_fake.call_count - 1;
        return result_ok();
    };

    sdhci_regs_t sdhci_regs = {};
    sdcard_t sdcard = {};
    sdhci_result_t sdhci_result;
    /* Using a smaller block size since max argument history length FFF provides is 50. */
    char buf[128] = {0}; /* Buffer is empty to begin with. */

    result_t res = sdhci_transfer_blocks(
            &sdhci_regs,
            &sdcard,
            0,
            2,
            64,
            false,
            buf,
            sizeof(buf),
            &sdhci_result
    );
    ASSERT_TRUE(result_is_ok(res));
    ASSERT_TRUE(sdhci_result == SD_OK);

    /* Buffer should have values from 0 to 31. */
    for (size_t i = 0; i < sizeof(buf) / sizeof(uint32_t); i++) {
        ASSERT_EQ(((uint32_t *) buf)[i], i);
    }
}


