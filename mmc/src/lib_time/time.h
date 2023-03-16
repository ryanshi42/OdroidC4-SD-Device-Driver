#pragma once

#include <stdint.h>
#include "string.h"
#include "timer_client.h"
#include "result.h"

typedef struct clock_data clock_data_t;
struct clock_data {
    timer_client_t *timer_client;
};

/**
 * Initialises the clock.
 * @param timer_client
 * @return
 */
result_t clock_init(timer_client_t *timer_client);

//#define CLOCK_MONOTONIC          1
//
///* seconds */
//#define SEC_IN_MINUTE   (60)
//
///* milliseconds */
//#define MS_IN_S         (1000)
//#define MS_IN_MINUTE    (MS_IN_S * SEC_IN_MINUTE)   /* =6e4 */
//
///* microseconds */
//#define US_IN_MS        (1000)
//#define US_IN_S         (US_IN_MS * MS_IN_S)
//#define US_IN_MINUTE    (US_IN_MS * MS_IN_MINUTE)   /* =6e7 */
//
///* nanoseconds */
//#define NS_IN_US        (1000)
//#define NS_IN_MS        (NS_IN_US * US_IN_MS)
//#define NS_IN_S         (NS_IN_US * US_IN_S)
//#define NS_IN_MINUTE    (NS_IN_US * US_IN_MINUTE)   /* =6e10 > 2^32 (=4e9) */
//
//typedef int64_t time_t;
//typedef int clockid_t;
//struct timespec {
//    time_t tv_sec;
//    long tv_nsec;
//};
//
///**
// * Returns the current time.
// * @param clock_id
// * @param tp
// * @return
// */
//int clock_gettime(clockid_t clock_id, struct timespec *tp);

/**
 * Returns the number of 1MHz ticks since the system was booted.
 * @return
 */
uint64_t clock_getticks(void);
