#pragma once

#include <stdbool.h>
#include <string.h>
#include <stdint.h>

#define MAX_NUM_ERR_MSGS (1 << 8)

typedef struct result result_t;
struct result {
    bool is_err;
    char *err_msgs[MAX_NUM_ERR_MSGS];
    size_t num_err_msgs;
};

/**
 * Creates a new Ok result.
 * @return
 */
result_t result_ok();

/**
 * Creates a new Error result.
 * @param err_msg
 * @return
 */
result_t result_err(const char *err_msg);

/**
 * Returns original result if result is Ok. Otherwise, it appends an error message to `err_msgs`.
 * @param result
 * @param err_msg
 * @return
 */
result_t result_ok_or(result_t result, const char *err_msg);

/**
 * Returns True if result is Ok and False otherwise.
 * @param result
 * @return
 */
bool result_is_ok(result_t result);

/**
 * Returns True if result is Error and False otherwise.
 * @param result
 * @return
 */
bool result_is_err(result_t result);

/**
 * Returns the error message stored in the error.
 * @param result
 * @param ret_err_msg The character buffer you wish to store the error message in.
 * @param buf_len The size of the character buffer you're using to store the error message.
 */
void result_get_err_msg(result_t result, char *ret_err_msg, size_t buf_len);

/**
 * Returns the number of error messages in `result`.
 * @param result
 * @return
 */
size_t result_get_num_err_msgs(result_t result);
