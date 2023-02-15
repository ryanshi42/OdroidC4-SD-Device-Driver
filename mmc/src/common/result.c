#include "result.h"

result_t result_ok() {
    return (result_t) {
            .err_msg = {0},
            .is_err = false
    };
}

result_t result_err(char *err_msg) {
    result_t result = {
            .err_msg = {0},
            .is_err = true
    };
    /* The following length includes the NULL terminator. */
    size_t err_msg_len = strlen(err_msg) + 1;
    if (err_msg_len > ERR_MSG_MAX_LEN) {
        /* The following ensures we always leave space for the NULL terminator. */
        err_msg_len = ERR_MSG_MAX_LEN - 1;
    }
    /* Copy the error message into the `result`. */
    memcpy(
            result.err_msg,
            err_msg,
            err_msg_len
    );
    return result;
}

bool result_is_ok(result_t result) {
    return !result.is_err;
}

bool result_is_err(result_t result) {
    return !result_is_ok(result);
}

char *result_get_err_msg(result_t *result) {
    return result->err_msg;
}
