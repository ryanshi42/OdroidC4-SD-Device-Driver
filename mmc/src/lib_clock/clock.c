#include "clock.h"

clock_data_t global_clock_data = {0};

result_t clock_init(timer_client_t *timer_client) {
    if (timer_client == NULL) {
        return result_err("NULL timer_client passed to clock_init().");
    }
    global_clock_data.timer_client = timer_client;
    return result_ok();
}

//
//int clock_gettime(clockid_t clock_id, struct timespec *tp) {
//    memset(tp, 0, sizeof(struct timespec));
//    if (clock_id != CLOCK_MONOTONIC) {
//        return -1;
//    }
//    unsigned int micros = get_system_timer();
//    tp->tv_sec = micros / US_IN_S;
//    tp->tv_nsec = (micros % US_IN_S) * NS_IN_US;
//    return 0;
//}

uint64_t clock_getticks(void) {
    uint64_t num_ticks = 0;
    result_t result = timer_client_get_num_ticks(
            global_clock_data.timer_client,
            &num_ticks
    );
    if (result_is_err(result)) {
        return 0;
    }
    return num_ticks;
}

