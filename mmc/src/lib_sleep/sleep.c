#include "sleep.h"

/* Global `bcm_timer`. */
bcm_timer_t global_bcm_timer = {0};

void sleep_init(uintptr_t timer_base_vaddr) {
    /* Initialise timer. */
    bcm_timer_init(
            &global_bcm_timer,
            timer_base_vaddr
    );
}

int usleep(useconds_t usec) {
    /* Get the number of ticks at beginning. */
    uint64_t start_ticks = 0;
    result_t res = bcm_timer_get_num_ticks(&global_bcm_timer, &start_ticks);
    if (result_is_err(res)) {
        return -1;
    }
    /* Compute the number of ticks at the end. */
    uint64_t end_ticks = start_ticks + usec;
    uint64_t curr_ticks = 0;
    do {
        /* Get current number of ticks. */
        res = bcm_timer_get_num_ticks(&global_bcm_timer, &curr_ticks);
        if (result_is_err(res)) {
            return -1;
        }
        /* Keep looping while `curr_ticks` < `end_ticks`. */
    } while (curr_ticks < end_ticks);
    /* Return 0 for success. */
    return 0;
}

int sleep(unsigned int seconds) {
    wait_msec_st(seconds * 1000000);
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
