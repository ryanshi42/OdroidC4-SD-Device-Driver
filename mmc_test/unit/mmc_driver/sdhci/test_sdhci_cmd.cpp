#include "gtest/gtest.h"

extern "C" {
#include "sdhci_cmd.h"
}

/* get_delay() */

TEST(test_sdhci_cmd, get_delay_should_return_delay) {
    sdhci_cmd_t sdhci_cmd = {
            .cmd_name = "GO_IDLE_STATE",
            .cmdtm = {
                    .CMD_RSPNS_TYPE = CMD_NO_RESP,
                    .CMD_INDEX = 0x00,
            },
            .delay = 1000,
    };
    size_t delay;
    result_t res = sdhci_cmd_get_delay(&sdhci_cmd, &delay);
    ASSERT_TRUE(result_is_ok(res));
    ASSERT_EQ(delay, 1000);
}

TEST(test_sdhci_cmd, get_delay_should_return_err_if_null_ret_val) {
    sdhci_cmd_t sdhci_cmd = {
            .cmd_name = "GO_IDLE_STATE",
            .cmdtm = {
                    .CMD_RSPNS_TYPE = CMD_NO_RESP,
                    .CMD_INDEX = 0x00,
            },
            .delay = 1000,
    };
    result res = sdhci_cmd_get_delay(&sdhci_cmd, NULL);
    ASSERT_TRUE(result_is_err(res));
}

/* get_cmd_rspns_type(). */

TEST(test_sdhci_cmd, get_cmd_rspns_type_should_return_cmd_rspns_type) {
    sdhci_cmd_t sdhci_cmd = {
            .cmd_name = "GO_IDLE_STATE",
            .cmdtm = {
                    .CMD_RSPNS_TYPE = CMD_NO_RESP,
                    .CMD_INDEX = 0x00,
            },
            .delay = 1000,
    };
    cmd_rspns_type_t cmd_rspns_type;
    result_t res = sdhci_cmd_get_cmd_rspns_type(&sdhci_cmd, &cmd_rspns_type);
    ASSERT_TRUE(result_is_ok(res));
    ASSERT_EQ(cmd_rspns_type, CMD_NO_RESP);
}

TEST(test_sdhci_cmd, get_cmd_rspns_type_should_return_err_if_null_ret_val) {
    sdhci_cmd_t sdhci_cmd = {
            .cmd_name = "GO_IDLE_STATE",
            .cmdtm = {
                    .CMD_RSPNS_TYPE = CMD_NO_RESP,
                    .CMD_INDEX = 0x00,
            },
            .delay = 1000,
    };
    result_t res = sdhci_cmd_get_cmd_rspns_type(&sdhci_cmd, NULL);
    ASSERT_TRUE(result_is_err(res));
}

/* get_cmd_index(). */

TEST(test_sdhci_cmd, get_cmd_index_should_return_cmd_index) {
    sdhci_cmd_t sdhci_cmd = {
            .cmd_name = "GO_IDLE_STATE",
            .cmdtm = {
                    .CMD_RSPNS_TYPE = CMD_NO_RESP,
                    .CMD_INDEX = 0x01,
            },
            .delay = 1000,
    };
    cmd_index_t cmd_index;
    result_t res = sdhci_cmd_get_cmd_index(&sdhci_cmd, &cmd_index);
    ASSERT_TRUE(result_is_ok(res));
    ASSERT_EQ(cmd_index, 0x01);
}

TEST(test_sdhci_cmd, get_cmd_index_should_return_err_if_null_ret_val) {
    sdhci_cmd_t sdhci_cmd = {
            .cmd_name = "GO_IDLE_STATE",
            .cmdtm = {
                    .CMD_RSPNS_TYPE = CMD_NO_RESP,
                    .CMD_INDEX = 0x00,
            },
            .delay = 1000,
    };
    result_t res = sdhci_cmd_get_cmd_index(&sdhci_cmd, NULL);
    ASSERT_TRUE(result_is_err(res));
}



