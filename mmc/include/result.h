#pragma once

#include <stdbool.h>
#include <string.h>
#include <stdint.h>

/* Max number of error messages `result.h` can hold is 16. */
#define MAX_NUM_ERR_MSGS (2 << 3)

/* This macro requires `printf_init()` in `printf.c` to have been called first. */
#define result_printf(result) \
        printf("\n==========================\n"); \
        if (result_is_err(result)) { \
            printf("ERROR on %s:%d in %s().\n", __FILE__, __LINE__, __FUNCTION__); \
            printf("Printing %ld out of %ld error messages.\n", result_get_num_err_msgs(result), result_get_total_num_err(result)); \
            printf("=== Stack trace ===\n"); \
            for (size_t i = 0; i < result_get_num_err_msgs(result); i++) { \
                printf("%ld: %s\n", i, result_get_err_msg_at(result, i)); \
            } \
            if (result_get_num_err_msgs(result) < result_get_total_num_err(result)) { \
                printf("...\n"); \
            } \
        } else { \
            printf("SUCCESS!\n"); \
        } \
        printf("==========================\n"); \

#define result_get_last_err_msg(result) \
    result_get_err_msg_at(result, result_get_num_err_msgs(result) - 1)

typedef struct result result_t;
struct result {
    bool is_err;
    const char *err_msgs[MAX_NUM_ERR_MSGS];
    size_t num_err_msgs;
    /* `total_num_err` represents the total number of errors in this `result`,
     * which is >= `num_err_msgs`. This is required since `err_msgs` has a fixed
     * total capacity of `MAX_NUM_ERR_MSGS`. */
    size_t total_num_err;
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
 * Chains an error message to an error.
 * @param result
 * @param err_msg
 * @return
 */
result_t result_err_chain(result_t result, const char *err_msg);

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
 * Returns the error message stored in `err_msgs` at `index`.
 * @param result
 * @param index
 * @return
 */
const char *result_get_err_msg_at(result_t result, size_t index);

/**
 * Returns the error message stored in the error.
 * @param result
 * @param ret_buf The character buffer you wish to store the error message in.
 * @param buf_len The size of the character buffer you're using to store the error message.
 */
void result_get_err_msg(result_t result, char *ret_buf, size_t buf_len);

/**
 * Returns the number of error messages in `result`.
 * @param result
 * @return
 */
size_t result_get_num_err_msgs(result_t result);

/**
 * Returns the total number of errors represented by `result`.
 * @param result
 * @return
 */
size_t result_get_total_num_err(result_t result);
