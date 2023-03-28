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

/**
 * Returns the number of 1MHz ticks since the system was booted.
 * @return
 */
uint64_t clock_getticks(void);
