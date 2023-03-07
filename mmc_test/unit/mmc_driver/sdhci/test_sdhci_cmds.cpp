#include "gtest/gtest.h"

extern "C" {
#include "sdhci_cmds.h"
}

TEST(test_sdhci_cmds, is_app_cmd_should_return_true_if_cmd_is_app_cmd) {
    size_t app_cmd_idx[] = {
            IX_SET_BUS_WIDTH,
            IX_SD_STATUS,
            IX_SEND_NUM_WRBL,
            IX_SEND_NUM_ERS,
            IX_APP_SEND_OP_COND,
            IX_SET_CLR_DET,
            IX_SEND_SCR,
    };
    for (size_t i = 0; i < sizeof(app_cmd_idx) / sizeof(app_cmd_idx[0]); i++) {
        bool is_app_cmd;
        sdhci_cmds_is_app_cmd(app_cmd_idx[i], &is_app_cmd);
        ASSERT_TRUE(is_app_cmd);
    }
}

TEST(test_sdhci_cmds, is_app_cmd_should_return_false_if_cmd_is_not_app_cmd) {
    size_t app_cmd_idx[] = {
            IX_GO_IDLE_STATE,
            IX_ALL_SEND_CID,
            IX_SEND_REL_ADDR,
            IX_SET_DSR,
            IX_SWITCH_FUNC,
            IX_CARD_SELECT,
            IX_SEND_IF_COND,
            IX_SEND_CSD,
            IX_SEND_CID,
            IX_VOLTAGE_SWITCH,
            IX_STOP_TRANS,
            IX_SEND_STATUS,
            IX_GO_INACTIVE,
            IX_SET_BLOCKLEN,
            IX_READ_SINGLE,
            IX_READ_MULTI,
            IX_SEND_TUNING,
            IX_SPEED_CLASS,
            IX_SET_BLOCKCNT,
            IX_WRITE_SINGLE,
            IX_WRITE_MULTI,
            IX_PROGRAM_CSD,
            IX_SET_WRITE_PR,
            IX_CLR_WRITE_PR,
            IX_SND_WRITE_PR,
            IX_ERASE_WR_ST,
            IX_ERASE_WR_END,
            IX_ERASE,
            IX_LOCK_UNLOCK,
            IX_APP_CMD,
            IX_APP_CMD_RCA,
            IX_GEN_CMD,
    };
    for (size_t i = 0; i < sizeof(app_cmd_idx) / sizeof(app_cmd_idx[0]); i++) {
        bool is_app_cmd;
        sdhci_cmds_is_app_cmd(app_cmd_idx[i], &is_app_cmd);
        ASSERT_FALSE(is_app_cmd);
    }
}
