#include "result.h"

result_t result_ok() {
    return (result_t) {
            .is_err = false,
            .err_msgs = {0},
            .num_err_msgs = 0,
    };
}

result_t result_err(const char *err_msg) {
    result_t result = {
            .is_err = true,
            .err_msgs = {0},
            .num_err_msgs = 1,
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

void result_get_err_msg(result_t result, char *ret_err_msg, size_t buf_len) {
    if (ret_err_msg == NULL || buf_len == 0) {
        return;
    }
    /* Zero out the buffer. */
    memset(ret_err_msg, 0, buf_len);
    if (result.num_err_msgs == 0) {
        return;
    }
    /* Iterate through the buffer. */
    size_t offset = 0;
    for (size_t i = 0; i < result.num_err_msgs; i++) {
        const char *err_msg = result.err_msgs[i];
        /* The following length does not include the NULL terminator. */
        size_t err_msg_len = strlen(err_msg);
        if (offset + err_msg_len < buf_len) {
            memcpy(
                    ret_err_msg + offset,
                    err_msg,
                    err_msg_len
            );
            offset += err_msg_len;
            if (offset + 1 < buf_len) {
                ret_err_msg[offset] = '\n';
                offset++;
            } else {
                break;
            }
        } else {
            break;
        }
    }
}

result_t result_ok_or(result_t result, const char *err_msg) {
    if (result_is_err(result)) {
        if (result.num_err_msgs < MAX_NUM_ERR_MSGS) {
            /* Set the next value in `err_msgs` to our new error message string. */
            result.err_msgs[result.num_err_msgs] = err_msg;
            /* Increment the `num_err_msgs`. */
            result.num_err_msgs++;
        }
    }
    return result;
}

size_t result_get_num_err_msgs(result_t result) {
    return result.num_err_msgs;
}
