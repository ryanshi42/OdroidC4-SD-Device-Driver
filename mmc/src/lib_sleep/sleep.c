#include "sleep.h"

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
