#include "gtest/gtest.h"

extern "C" {
#include "cmdtm.h"
}

/* get_cmd_rspns_type() */

TEST(test_cmdtm, get_cmd_rspns_type_should_return_cmd_rspns_type) {
    cmdtm_t cmdtm = {};
    cmdtm.CMD_RSPNS_TYPE = CMD_NO_RESP;
    cmdtm.CMD_INDEX = 0x01;

    cmd_rspns_type_t cmd_rspns_type;
    result_t res = cmdtm_get_cmd_rspns_type(&cmdtm, &cmd_rspns_type);
    ASSERT_TRUE(result_is_ok(res));
    ASSERT_EQ(CMD_NO_RESP, cmd_rspns_type);
}

TEST(test_cmdtm, get_cmd_rspns_type_should_return_err_if_null_ret_val) {
    cmdtm_t cmdtm = {};
    cmdtm.CMD_RSPNS_TYPE = CMD_NO_RESP;
    cmdtm.CMD_INDEX = 0x01;

    result_t res = cmdtm_get_cmd_rspns_type(&cmdtm, NULL);
    ASSERT_TRUE(result_is_err(res));
}

/* get_cmd_index() */

TEST(test_cmdtm, get_cmd_index_should_return_cmd_index) {
    cmdtm_t cmdtm = {};
    cmdtm.CMD_RSPNS_TYPE = CMD_NO_RESP;
    cmdtm.CMD_INDEX = 0x01;

    cmd_index_t cmd_index;
    result_t res = cmdtm_get_cmd_index(&cmdtm, &cmd_index);
    ASSERT_TRUE(result_is_ok(res));
    ASSERT_EQ(0x01, cmd_index);
}

TEST(test_cmdtm, get_cmd_index_should_return_err_if_null_ret_val) {
    cmdtm_t cmdtm = {};
    cmdtm.CMD_RSPNS_TYPE = CMD_NO_RESP;
    cmdtm.CMD_INDEX = 0x01;

    result_t res = cmdtm_get_cmd_index(&cmdtm, NULL);
    ASSERT_TRUE(result_is_err(res));
}

