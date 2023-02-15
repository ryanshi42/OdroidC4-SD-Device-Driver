#include "result.h"

result_t result_ok() {
    return (result_t) {
            .err_msgs = {0},
            .is_err = false
    };
}

result_t result_err(char *err_msg) {
    result_t result = {
            .err_msgs = {0},
            .is_err = true
    };
    result.err_msgs[0] = err_msg;
    return result;
}

bool result_is_ok(result_t result) {
    return !result.is_err;
}

bool result_is_err(result_t result) {
    return !result_is_ok(result);
}

char *result_get_err_msg(result_t result) {
    if (result.err_msgs[0]) {
        return result.err_msgs[0];
    }
    return "";
}
