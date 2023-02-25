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
    wait_msec_st(usec);
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
