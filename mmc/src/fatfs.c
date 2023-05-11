#include "fatfs.h"

/* Used for transporting chars from `fatfs` to `serial_client`. */
uintptr_t fatfs_to_serial_client_putchar_buf;

/* Shared memory region for passing requests from `fatfs` to `mmc_driver`. */
uintptr_t mmc_driver_request_queue;

/* Shared memory region for passing responses from `mmc_driver` to `fatfs`. */
uintptr_t mmc_driver_response_queue;

/* Shared memory region for passing data between `mmc_driver` and `fatfs`. */
uintptr_t mmc_driver_shared_data;

/* The global shared data queue that manages the buffers within the
 * `mmc_driver_shared_data` shared memory region. */
blk_shared_data_queue_t shared_data_queue;

void init(void) {
    /* Initialise `printf`. The `log.h` library depends on `printf` being
     * initialised first. */
    printf_init(
            fatfs_to_serial_client_putchar_buf,
            FATFS_TO_SERIAL_CLIENT_PUTCHAR_CHANNEL
    );
    log_trace("Starting init() in FatFs Protection Domain...");

    /* We're initialising all the following data structures required to
     * interface with our MMC driver here so there is no need to re-initialise
     * these data structures in the MMC driver. */

    blk_request_queue_result_t request_queue_init_result = blk_request_queue_init(
            (blk_request_queue_t *) mmc_driver_request_queue,
            SEL4CP_MAX_PAGE_SIZE
    );
    if (request_queue_init_result != OK_BLK_REQUEST_QUEUE) {
        log_error("Failed to initialise `mmc_driver_request_queue` with code %d.", request_queue_init_result);
        return;
    }

    blk_response_queue_result_t response_queue_init_result = blk_response_queue_init(
            (blk_response_queue_t *) mmc_driver_response_queue,
            SEL4CP_MAX_PAGE_SIZE
    );
    if (response_queue_init_result != OK_BLK_RESPONSE_QUEUE) {
        log_error("Failed to initialise `mmc_driver_response_queue` with code %d.", response_queue_init_result);
        return;
    }

    blk_shared_data_queue_result_t shared_data_queue_init_result = blk_shared_data_queue_init(
            &shared_data_queue,
            mmc_driver_shared_data,
            SEL4CP_MAX_PAGE_SIZE,
            FAT_CLUSTER_SIZE /* TODO: Figure out how to obtain this info dynamically. */
    );
    if (shared_data_queue_init_result != OK_BLK_SHARED_DATA_QUEUE) {
        log_error("Failed to initialise `mmc_driver_shared_data_queue` with code %d.", shared_data_queue_init_result);
        return;
    }

    /* TODO: E2E test the state of the above data structures. */
}

void notified(sel4cp_channel ch) {

}



