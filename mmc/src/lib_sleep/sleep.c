#include "sleep.h"

/* Global `sleep_data`. */
typedef struct sleep_data sleep_data_t;
struct sleep_data {
    /* Timer client. */
    timer_client_t timer_client;
};
sleep_data_t global_sleep_data = {0};

result_t sleep_init(timer_client_t *timer_client) {
    if (timer_client == NULL) {
        return result_err("NULL `timer_client` passed to sleep_init().");
    }
    global_sleep_data.timer_client = *timer_client;
    return result_ok();
}

#if !MMC_RPI3B_UNIT_TEST

int usleep(useconds_t usec) {
    timer_client_t *timer_client = &global_sleep_data.timer_client;
    /* Get the number of ticks at beginning. */
    uint64_t start_ticks = 0;
    result_t res = timer_client_get_num_ticks(timer_client, &start_ticks);
    if (result_is_err(res)) {
        return -1;
    }
    /* Compute the number of ticks at the end. */
    uint64_t end_ticks = start_ticks + usec;
    uint64_t curr_ticks = 0;
    do {
        /* Get current number of ticks. */
        res = timer_client_get_num_ticks(timer_client, &curr_ticks);
        if (result_is_err(res)) {
            return -1;
        }
        /* Keep looping while `curr_ticks` < `end_ticks`. */
    } while (curr_ticks < end_ticks);
    /* Return 0 for success. */
    return 0;
}

int sleep(unsigned int seconds) {
    usleep(seconds * 1000000);
    return 0;
}

int sleep_cyc(unsigned int cycles) {
    if (cycles) {
        while (cycles--) {
            asm volatile("nop");
        }
    }
    return 0;
}

#endif