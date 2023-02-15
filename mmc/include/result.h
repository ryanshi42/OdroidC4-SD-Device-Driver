#pragma once

#include <stdbool.h>
#include <string.h>

#define MAX_NUM_ERR_MSGS (1 << 8)

typedef struct result result_t;
struct result {
    bool is_err;
    char *err_msgs[MAX_NUM_ERR_MSGS];
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
result_t result_err(char *err_msg);

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
 * Returns the error message if the result is an error.
 * @param result
 * @return
 */
char *result_get_err_msg(result_t result);
