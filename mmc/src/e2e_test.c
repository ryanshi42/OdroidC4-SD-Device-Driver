#include "e2e_test.h"

result_t e2e_test_sleep() {
    log_info("Starting e2e_test_sleep():");

    uint64_t start_ticks, finish_ticks, delta_ticks;
    start_ticks = clock_getticks();
    usleep(1000000);
    finish_ticks = clock_getticks();
    delta_ticks = finish_ticks - start_ticks;
    log_info("delta_ticks (usleep): %lu", delta_ticks);
    assert(delta_ticks - 1000000 <= 2000);

    start_ticks = clock_getticks();
    sleep_cyc(150);
    finish_ticks = clock_getticks();
    delta_ticks = finish_ticks - start_ticks;
    log_info("delta_ticks (sleep_cyc): %lu", delta_ticks);
    assert(delta_ticks <= 10);

    log_info("Finished e2e_test_sleep():");
    return result_ok();
}
