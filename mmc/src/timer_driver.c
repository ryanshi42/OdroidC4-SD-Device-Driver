#include "timer_driver.h"

/* Used for transporting chars from `timer_driver` to `serial_client`. */
uintptr_t timer_driver_to_serial_client_putchar_buf;

uintptr_t timer_base_vaddr;

/* Used for transporting num ticks from `timer_driver` to `mmc_driver`. */
uintptr_t timer_driver_to_mmc_driver_numticks_buf;

/* Global `timer_driver`. */
timer_driver_t global_timer_driver = {0};

result_t timer_driver_init(timer_driver_t *timer_driver, uintptr_t base_vaddr) {
    /* Initialise the timer. */
    result_t res = bcm_timer_init(
            &timer_driver->bcm_timer,
            base_vaddr
    );
    if (result_is_err(res)) {
        return result_ok_or(res, "Failed to initialise timer in timer_driver_init().");
    }
    return result_ok();
}

void init(void) {
    /* Initialise `printf`. */
    printf_init(
            timer_driver_to_serial_client_putchar_buf,
            TIMER_DRIVER_TO_SERIAL_CLIENT_PUTCHAR_CHANNEL
    );
    /* Initialise the `timer_driver`. */
    result_t result = timer_driver_init(
            &global_timer_driver,
            timer_base_vaddr
    );
    /* Print out any errors. */
    if (result_is_err(result)) {
        result_printf(result);
        return;
    }
}

seL4_MessageInfo_t protected(sel4cp_channel ch, sel4cp_msginfo msginfo) {
    return sel4cp_msginfo_new(0, 0);
}

void notified(sel4cp_channel channel) {
    timer_driver_t *timer_driver = &global_timer_driver;
    switch(channel) {
        case TIMER_DRIVER_TO_MMC_DRIVER_GET_NUM_TICKS_CHANNEL: {
            uint64_t num_ticks;
            result_t res = bcm_timer_get_num_ticks(&timer_driver->bcm_timer, &num_ticks);
            if (result_is_err(res)) {
                result_printf(res);
                return;
            }
            /* Copy the number of ticks into the buffer. */
            *(uint64_t *) timer_driver_to_mmc_driver_numticks_buf = num_ticks;
            break;
        }
        default:
            printf("Timer Driver: received notification on unexpected channel\n");
            break;
    }
}
