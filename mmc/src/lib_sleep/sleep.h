#pragma once

/* Temporarily using delays. */
#include "delays.h"
/* Temporarily directly including bcm_timer. */
#include "bcm_timer.h"

typedef unsigned int useconds_t;

/**
 * Initialises the sleep library.
 * @param timer_base_vaddr
 */
void sleep_init(uintptr_t timer_base_vaddr);

/**
* The usleep() function suspends execution of the calling thread for (at least)
* usec microseconds.  The sleep may be lengthened slightly by any system
* activity or by the time spent processing the call or by the granularity of
* system timers.
 * @param usec
 * @return The usleep() function returns 0 on success.  On error, -1 is
 * returned.
 */
int usleep(useconds_t usec);

/**
 * sleep() causes the calling thread to sleep either until the number of
 * real-time seconds specified in seconds have elapsed or until a signal arrives
 * which is not ignored.
 * @param seconds
 * @return Zero if the requested time has elapsed. On error, -1 is returned.
 */
int sleep(unsigned int seconds);

/**
 * sleep_cyc() causes the calling thread to sleep for the number of CPU cycles
 * specified in cycles.
 * @param cycles
 * @return Zero if the requested time has elapsed. On error, -1 is returned.
 */
int sleep_cyc(unsigned int cycles);
