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

char* itoa(int i, char b[]){
    char const digit[] = "0123456789";
    char* p = b;
    if(i<0){
        *p++ = '-';
        i *= -1;
    }
    int shifter = i;
    do{ //Move to where representation ends
        ++p;
        shifter = shifter/10;
    }while(shifter);
    *p = '\0';
    do{ //Move back, inserting digits as u go
        *--p = digit[i%10];
        i = i/10;
    }while(i);
    return b;
}

int usleep(unsigned int usec) {
    return 0;
    //TODO: Fix the usleep function later when I am bothered to fix the timer driver
    timer_client_t *timer_client = &global_sleep_data.timer_client;
    /* Get the number of ticks at beginning. */
    uint64_t start_ticks = 0;
    result_t res = timer_client_get_num_ticks(timer_client, &start_ticks);
    if (result_is_err(res)) {
        return -1;
    }
    char snum[10];

    sel4cp_dbg_puts("\n");
    sel4cp_dbg_puts(itoa(start_ticks, snum));
    sel4cp_dbg_puts("\n");
    /* Compute the number of ticks at the end. */
    uint64_t end_ticks = start_ticks + usec;

    sel4cp_dbg_puts("\n");
    sel4cp_dbg_puts(itoa(end_ticks, snum));
    sel4cp_dbg_puts("\n");

    uint64_t curr_ticks = 0;
    do {
        /* Get current number of ticks. */
        res = timer_client_get_num_ticks(timer_client, &curr_ticks);
        if (result_is_err(res)) {
            return -1;
        }
        // sel4cp_dbg_puts("\n");
        // sel4cp_dbg_puts(itoa(start_ticks, snum));
        // sel4cp_dbg_puts("\n");

        /* Keep looping while `curr_ticks` < `end_ticks`. */
    } while (curr_ticks < end_ticks);
    /* Return 0 for success. */
    return 0;
}

int sleep(unsigned int seconds) {
    usleep(seconds * 1000000);
    return 0;
}

#endif

int sleep_cyc(unsigned int cycles) {
    if (cycles) {
        while (cycles--) {
            asm volatile("nop");
        }
    }
    return 0;
}

