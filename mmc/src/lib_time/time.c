#include "time.h"

int clock_gettime(clockid_t clock_id, struct timespec *tp) {
    memset(tp, 0, sizeof(struct timespec));
    if (clock_id != CLOCK_MONOTONIC) {
        return -1;
    }
    unsigned int micros = get_system_timer();
    tp->tv_sec = micros / US_IN_S;
    tp->tv_nsec = (micros % US_IN_S) * NS_IN_US;
    return 0;
}

uint64_t clock_getticks(void) {
    return get_system_timer();
}

