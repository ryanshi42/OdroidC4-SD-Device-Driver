#include "gtest/gtest.h"

extern "C" {
#include "sdhci_resp_r1.h"
}

TEST(test_sdhci_resp_r1, has_error_should_return_false_if_resp_has_no_error) {
    uint32_t resp = 0x00000000;
    bool ret_val;
    result_t res = sdhci_resp_r1_has_error(resp, &ret_val);
    ASSERT_TRUE(result_is_ok(res));
    ASSERT_FALSE(ret_val);
}

TEST(test_sdhci_resp_r1, has_error_should_return_false_if_resp_has_error) {
    auto responses = std::vector<uint32_t>{
            0b0100,
            0b100000000000000,
            0b1000000000000000
    };
    for (auto const resp : responses) {
        bool ret_val;
        result_t res = sdhci_resp_r1_has_error(resp, &ret_val);
        ASSERT_TRUE(result_is_ok(res));
        ASSERT_TRUE(ret_val);
    }
}
