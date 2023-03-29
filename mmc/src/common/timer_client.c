#include "timer_client.h"


result_t timer_client_init(
        timer_client_t *timer_client,
        size_t sel4cp_channel_id_get_num_ticks,
        uint64_t *timer_driver_to_mmc_driver_numticks_buf
) {
    if (timer_client == NULL) {
        return result_err("NULL `timer_client` passed to timer_client_init().");
    }
    if (timer_driver_to_mmc_driver_numticks_buf == NULL) {
        return result_err("NULL `timer_driver_to_mmc_driver_numticks_buf` passed to timer_client_init().");
    }
    /* Save the sel4cp channel ID. */
    timer_client->sel4cp_channel_id_get_num_ticks = sel4cp_channel_id_get_num_ticks;
    /* Save the buffer used to obtain the number of ticks. */
    timer_client->timer_driver_to_mmc_driver_numticks_buf = timer_driver_to_mmc_driver_numticks_buf;
    return result_ok();
}


result_t timer_client_get_num_ticks(
        timer_client_t *timer_client,
        uint64_t *ret_val
) {
    if (timer_client == NULL) {
        return result_err("NULL `timer_client` passed to timer_client_get_num_ticks().");
    }
    if (ret_val == NULL) {
        return result_err("NULL `ret_val` passed to timer_client_get_num_ticks().");
    }
    /* Send a notification to `timer_driver`. */
    sel4cp_notify(timer_client->sel4cp_channel_id_get_num_ticks);
    /* Return the value stored in `timer_driver_to_mmc_driver_numticks_buf` */
    *ret_val = *timer_client->timer_driver_to_mmc_driver_numticks_buf;
    return result_ok();
}
