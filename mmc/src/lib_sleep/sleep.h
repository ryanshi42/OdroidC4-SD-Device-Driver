#pragma once

#include "timer_client.h"
#include "result.h"

/**
 * Initialises the sleep library.
 * @param timer_base_vaddr
 */
result_t sleep_init(timer_client_t *timer_client);

#if MMC_RPI3B_UNIT_TEST

#include <unistd.h>

#else

/**
* The usleep() function suspends execution of the calling thread for (at least)
* usec microseconds.  The sleep may be lengthened slightly by any system
* activity or by the time spent processing the call or by the granularity of
* system timers.
 * @param usec
 * @return The usleep() function returns 0 on success.  On error, -1 is
 * returned.
 */
int usleep(unsigned int usec);

/**
 * sleep() causes the calling thread to sleep either until the number of
 * real-time seconds specified in seconds have elapsed or until a signal arrives
 * which is not ignored.
 * @param seconds
 * @return Zero if the requested time has elapsed. On error, -1 is returned.
 */
int sleep(unsigned int seconds);

#endif

/**
 * sleep_cyc() causes the calling thread to sleep for the number of CPU cycles
 * specified in cycles.
 * @param cycles
 * @return Zero if the requested time has elapsed. On error, -1 is returned.
 */
int sleep_cyc(unsigned int cycles);
