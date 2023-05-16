#include "fatfs.h"

/* Used for transporting chars from `fatfs` to `serial_client`. */
uintptr_t fatfs_to_serial_client_putchar_buf;

/* Shared memory region for passing requests from `fatfs` to `mmc_driver`. */
uintptr_t mmc_driver_request_queue;

/* Shared memory region for passing responses from `mmc_driver` to `fatfs`. */
uintptr_t mmc_driver_response_queue;

/* Shared memory region for passing data between `mmc_driver` and `fatfs`. */
uintptr_t mmc_driver_shared_data;

/* The global MMC driver client that uses the sDDF data structures (above) to
 * interact with the `mmc_driver`. */
mmc_driver_client_t global_mmc_driver_client = {0};

/* Global static space allocated for the heap. */
uint8_t heap[0x100000] = {0};

result_t fatfs_get_cluster_size_in_bytes(
        mmc_driver_client_t *mmc_driver_client,
        size_t *ret_val
) {
    /* Get the size of the cluster in blocks from FatFs. */
    FRESULT res_f_mount;
    FATFS fs = {0};
    res_f_mount = f_mount(&fs, "", 1);
    if (res_f_mount != FR_OK) {
        return result_err("Failed to get cluster size from FatFs.");
    }
    size_t const cluster_size_in_blocks = fs.csize;
    /* Get the size of each block in bytes using `global_mmc_driver_client`. */
    uint16_t block_size_in_bytes = 0;
    result_t res = mmc_driver_client_get_block_size(
            mmc_driver_client,
            &block_size_in_bytes
    );
    *ret_val = cluster_size_in_blocks * block_size_in_bytes;
    return result_ok_or(res, "Failed to get cluster size in bytes.");
}

void init(void) {
    /* Initialise `printf`. The `log.h` library depends on `printf` being
     * initialised first. */
    printf_init(
            fatfs_to_serial_client_putchar_buf,
            FATFS_TO_SERIAL_CLIENT_PUTCHAR_CHANNEL
    );
    log_trace("Starting init() in FatFs Protection Domain...");

    /* Initialising the heap. */
    malloc_addblock(heap, sizeof(heap));

    /* We're initialising all the following data structures required to
     * interface with our MMC driver here so there is no need to re-initialise
     * these data structures in the MMC driver. */

    blk_request_queue_t *request_queue = (blk_request_queue_t *) mmc_driver_request_queue;
    blk_request_queue_result_t request_queue_init_result = blk_request_queue_init(
            request_queue,
            SEL4CP_MAX_PAGE_SIZE
    );
    if (request_queue_init_result != OK_BLK_REQUEST_QUEUE) {
        log_error("Failed to initialise `mmc_driver_request_queue` with code %d.", request_queue_init_result);
        return;
    }

    blk_response_queue_t *response_queue = (blk_response_queue_t *) mmc_driver_response_queue;
    blk_response_queue_result_t response_queue_init_result = blk_response_queue_init(
            response_queue,
            SEL4CP_MAX_PAGE_SIZE
    );
    if (response_queue_init_result != OK_BLK_RESPONSE_QUEUE) {
        log_error("Failed to initialise `mmc_driver_response_queue` with code %d.", response_queue_init_result);
        return;
    }

    /* We create a Shared Data queue with buffers sized by
     * `MAX_FAT_CLUSTER_SIZE`. However, this is only so that we can compute the
     * actual cluster size of the file system below using
     * `fatfs_get_cluster_size_in_bytes()`. We later create a Shared Data queue
     * with buffers sized by the actual cluster size of the file system and save
     * this to `global_mmc_driver_client`. */
    blk_shared_data_queue_t shared_data_queue = {0};
    blk_shared_data_queue_result_t shared_data_queue_init_result = blk_shared_data_queue_init(
            &shared_data_queue,
            mmc_driver_shared_data,
            SEL4CP_MAX_PAGE_SIZE,
            MAX_FAT_CLUSTER_SIZE
    );
    if (shared_data_queue_init_result != OK_BLK_SHARED_DATA_QUEUE) {
        log_error("Failed to initialise `mmc_driver_shared_data_queue` with code %d.", shared_data_queue_init_result);
        return;
    }

    /* Initialise the `mmc_driver_client` library that FatFs relies on. */
    result_t res;
    res = mmc_driver_client_init(
            &global_mmc_driver_client,
            FATFS_TO_MMC_DRIVER_REQUEST_CHANNEL,
            request_queue,
            response_queue,
            &shared_data_queue
    );
    if (result_is_err(res)) {
        result_printf(res);
        return;
    }

    /* Initialises FatFs by passing it the `mmc_driver_client`, which uses sDDF
     * data structures to interact with the `mmc_driver`. */
    disk_sddf_init(&global_mmc_driver_client);

    /* Get the size of the FatFs I/O cluster in bytes. */
    size_t cluster_size_in_bytes = 0;
    res = fatfs_get_cluster_size_in_bytes(
            &global_mmc_driver_client,
            &cluster_size_in_bytes
    );
    if (result_is_err(res)) {
        result_printf(res);
        return;
    }
    log_trace("Cluster size in bytes: 0x%lx", cluster_size_in_bytes);

    /* Now we create a `shared_data_queue` with buffers sized as
     * `cluster_size_in_bytes`. */
    shared_data_queue_init_result = blk_shared_data_queue_init(
            &shared_data_queue,
            mmc_driver_shared_data,
            SEL4CP_MAX_PAGE_SIZE,
            cluster_size_in_bytes
    );
    if (shared_data_queue_init_result != OK_BLK_SHARED_DATA_QUEUE) {
        log_error("Failed to initialise `mmc_driver_shared_data_queue` with code %d.", shared_data_queue_init_result);
        return;
    }
    /* Set the `shared_data_queue` field in `global_mmc_driver_client` to our
     * new `shared_data_queue`.  */
    res = mmc_driver_client_set_shared_data_queue(
            &global_mmc_driver_client,
            &shared_data_queue
    );
    if (result_is_err(res)) {
        result_printf(res);
        return;
    }

    /* Obtain the capacity of `shared_data_queue` to print out.*/
    size_t capacity = 0;
    blk_shared_data_queue_result_t capacity_result = blk_shared_data_queue_capacity(
            &shared_data_queue,
            &capacity
    );
    if (capacity_result != OK_BLK_SHARED_DATA_QUEUE) {
        log_error("Failed to get capacity of `mmc_driver_shared_data_queue` with code %d.", capacity_result);
        return;
    }
    log_trace("Shared data queue capacity: %ld", capacity);

    /* TODO: Remove this second init(). */
    disk_sddf_init(&global_mmc_driver_client);

    /* End-to-end tests to verify FatFs functionality. */

    res = fatfs_e2e_diskio_test();
    if (result_is_err(res)) {
        result_printf(res);
        return;
    }

    res = fatfs_e2e_write_close_read_simple();
    if (result_is_err(res)) {
        result_printf(res);
        return;
    }

    res = fatfs_e2e_write_fsync_read_close_simple();
    if (result_is_err(res)) {
        result_printf(res);
        return;
    }

    res = fatfs_e2e_write_read_custom(2 * cluster_size_in_bytes);
    if (result_is_err(res)) {
        result_printf(res);
        return;
    }

    res = fatfs_e2e_write_read_custom(10 * cluster_size_in_bytes);
    if (result_is_err(res)) {
        result_printf(res);
        return;
    }

    log_trace("Finished running E2E tests in MMC Driver Protection Domain!");
}

void notified(sel4cp_channel ch) {
    (void) ch;
}



