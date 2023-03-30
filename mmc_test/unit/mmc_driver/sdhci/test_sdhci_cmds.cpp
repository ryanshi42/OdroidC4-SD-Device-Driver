#include "gtest/gtest.h"

extern "C" {
#include "sdhci_cmds.h"
}

TEST(test_sdhci_cmds, is_app_cmd_should_return_true_if_cmd_is_app_cmd) {
    size_t app_cmd_idx[] = {
            IDX_SET_BUS_WIDTH,
            IDX_SD_STATUS,
            IDX_SEND_NUM_WRBL,
            IDX_SEND_NUM_ERS,
            IDX_APP_SEND_OP_COND,
            IDX_SET_CLR_DET,
            IDX_SEND_SCR,
    };
    for (size_t i = 0; i < sizeof(app_cmd_idx) / sizeof(app_cmd_idx[0]); i++) {
        bool is_app_cmd;
        sdhci_cmds_is_app_cmd(app_cmd_idx[i], &is_app_cmd);
        ASSERT_TRUE(is_app_cmd);
    }
}

TEST(test_sdhci_cmds, is_app_cmd_should_return_false_if_cmd_is_not_app_cmd) {
    size_t app_cmd_idx[] = {
            IDX_GO_IDLE_STATE,
            IDX_ALL_SEND_CID,
            IDX_SEND_REL_ADDR,
            IDX_SET_DSR,
            IDX_SWITCH_FUNC,
            IDX_CARD_SELECT,
            IDX_SEND_IF_COND,
            IDX_SEND_CSD,
            IDX_SEND_CID,
            IDX_VOLTAGE_SWITCH,
            IDX_STOP_TRANS,
            IDX_SEND_STATUS,
            IDX_GO_INACTIVE,
            IDX_SET_BLOCKLEN,
            IDX_READ_SINGLE,
            IDX_READ_MULTI,
            IDX_SEND_TUNING,
            IDX_SPEED_CLASS,
            IDX_SET_BLOCKCNT,
            IDX_WRITE_SINGLE,
            IDX_WRITE_MULTI,
            IDX_PROGRAM_CSD,
            IDX_SET_WRITE_PR,
            IDX_CLR_WRITE_PR,
            IDX_SND_WRITE_PR,
            IDX_ERASE_WR_ST,
            IDX_ERASE_WR_END,
            IDX_ERASE,
            IDX_LOCK_UNLOCK,
            IDX_APP_CMD,
            IDX_APP_CMD_RCA,
            IDX_GEN_CMD,
    };
    for (size_t i = 0; i < sizeof(app_cmd_idx) / sizeof(app_cmd_idx[0]); i++) {
        bool is_app_cmd;
        sdhci_cmds_is_app_cmd(app_cmd_idx[i], &is_app_cmd);
        ASSERT_FALSE(is_app_cmd);
    }
}

/* get_cmd */

TEST(test_sdhci_cmds, get_cmd_should_return_the_correct_cmd) {
    sdhci_cmd_t *cmd;
    for (size_t i = 0; i < IDX_SEND_SCR; i++) {
        result_t result = sdhci_cmds_get_cmd(
                i,
                &cmd
        );
        ASSERT_TRUE(result_is_ok(result));
    }
}

TEST(test_sdhci_cmds, get_cmd_should_return_err_if_cmd_idx_is_invalid) {
    sdhci_cmd_t *cmd;
    for (size_t i = IDX_SEND_SCR + 1; i < IDX_SEND_SCR * 2; i++) {
        result_t result = sdhci_cmds_get_cmd(
                i,
                &cmd
        );
        ASSERT_TRUE(result_is_err(result));
    }
}

/* is_app_cmd */

TEST(test_sdhci_cmds, is_app_cmd_should_return_the_correct_cmd) {
    bool is_app_cmd;
    for (size_t i = 0; i < IDX_SEND_SCR; i++) {
        result_t result = sdhci_cmds_is_app_cmd(
                i,
                &is_app_cmd
        );
        ASSERT_TRUE(result_is_ok(result));
    }
}

TEST(test_sdhci_cmds, is_app_cmd_should_return_err_if_cmd_idx_is_invalid) {
    bool is_app_cmd;
    for (size_t i = IDX_SEND_SCR + 1; i < IDX_SEND_SCR * 2; i++) {
        result_t result = sdhci_cmds_is_app_cmd(
                i,
                &is_app_cmd
        );
        ASSERT_TRUE(result_is_err(result));
    }
}
