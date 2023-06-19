#include "mmc_driver.h"

/* Used for transporting chars from `mmc_driver` to `serial_client`. */
uintptr_t mmc_to_serial_client_putchar_buf;

/* Used for transporting num ticks from `timer_driver` to `mmc_driver`. */
uintptr_t timer_driver_to_mmc_driver_numticks_buf;

/* Base virtual address for the GPIO registers. */
uintptr_t gpio_base_vaddr;

/* Base virtual address for the EMMC registers. */
uintptr_t emmc_base_vaddr;

/* Shared memory region for passing requests from `fatfs` to `mmc_driver`. */
uintptr_t mmc_driver_request_queue;

/* Shared memory region for passing responses from `mmc_driver` to `fatfs`. */
uintptr_t mmc_driver_response_queue;

/* Shared memory region for passing data between `mmc_driver` and `fatfs`. */
uintptr_t mmc_driver_shared_data;

/* Global `timer_client`. */
timer_client_t global_timer_client = {0};

/* Global `sdcard`. */
sdcard_t global_sdcard = {0};

/* Global `sdhci_regs`. */
sdhci_regs_t global_sdhci_regs = {0};

void init(void) {
    result_t res;

    /* Initialise `printf`. The `log.h` library depends on `printf` being
     * initialised first. */
    printf_init(
            mmc_to_serial_client_putchar_buf,
            MMC_DRIVER_TO_SERIAL_CLIENT_PUTCHAR_CHANNEL
    );

    /* Initialise the `timer_client`, which is the library used to interface
     * with the `timer_driver` PD. */
    res = timer_client_init(
            &global_timer_client,
            MMC_DRIVER_TO_TIMER_DRIVER_GET_NUM_TICKS_CHANNEL,
            (uint64_t *) timer_driver_to_mmc_driver_numticks_buf
    );
    if (result_is_err(res)) {
        result_printf(res);
        return;
    }

    /* Initialise the clock library, which depends on the `timer_client` being
     * initialised first. */
    res = clock_init(&global_timer_client);
    if (result_is_err(res)) {
        result_printf(res);
        return;
    }

    /* Initialise sleep library, which depends on the `timer_client` being
     * initialised first. */
    res = sleep_init(&global_timer_client);
    if (result_is_err(res)) {
        result_printf(res);
        return;
    }

    /* Initialise the `sdcard` struct by clearing it to zero. */
    res = sdcard_init(&global_sdcard);
    if (result_is_err(res)) {
        result_printf(res);
        return;
    }

    /* Run E2E tests to verify sleep works properly, which our SD card driver
     * depends upon.*/
    mmc_driver_e2e_sleep();

    /* Initialise SDHCI registers. */
    res = sdhci_regs_init(
            &global_sdhci_regs,
            (void *) emmc_base_vaddr
    );
    if (result_is_err(res)) {
        result_printf(res);
        return;
    }

    /* Initialise and reset the Pi's SD card Host Controller. */
    res = oc4_emmc_init(
            &global_sdhci_regs,
            (bcm_gpio_regs_t *) gpio_base_vaddr
    );
    if (result_is_err(res)) {
        result_printf(res);
        return;
    }
    log_trace("Finished initialising SD Host Controller.");

    /* Initialise and identify the SD card. */
    sdhci_result_t sdhci_result;
    res = sdhci_card_init_and_id(
            &global_sdhci_regs,
            &global_sdcard,
            &sdhci_result
    );
    if (result_is_err(res)) {
        result_printf(res);
        return;
    }
    log_trace("Finished initialising and identifying the SD card.");

    /* Setting Bus Width to maximum possible value. */
    res = sdhci_set_max_bus_width(
            &global_sdhci_regs,
            &global_sdcard,
            &sdhci_result
    );
    if (result_is_err(res)) {
        result_printf(res);
        return;
    }
    log_trace("Finished setting SD bus width to maximum possible value.");

    /* Running E2E tests to verify our SD card driver works properly.*/
    res = mmc_driver_e2e_read_write_simple(
            &global_sdhci_regs,
            &global_sdcard
    );
    if (result_is_err(res)) {
        result_printf(res);
        return;
    }

    res = mmc_driver_e2e_read_write_multiple_blocks(
            &global_sdhci_regs,
            &global_sdcard
    );
    if (result_is_err(res)) {
        result_printf(res);
        return;
    }

    res = mmc_driver_e2e_sdcard_card_specific_data(&global_sdcard);
    if (result_is_err(res)) {
        result_printf(res);
        return;
    }
    log_info("Successfully finished E2E tests for MMC Driver.");
}

result_t mmc_driver_get_num_blocks(uint64_t *ret_val) {
    if (ret_val == NULL) {
        return result_err("NULL `ret_val` in mmc_driver_get_num_blocks().");
    }
    return sdcard_get_num_blocks(&global_sdcard, ret_val);
}

result_t mmc_driver_get_block_size(uint16_t *ret_val) {
    if (ret_val == NULL) {
        return result_err("NULL `ret_val` in mmc_driver_get_block_size().");
    }
    return sdcard_get_block_size(&global_sdcard, ret_val);
}

result_t mmc_driver_write_flush(void) {
    /* Our current write implementation waits for the write operation to
     * complete before returning to the client. Therefore, this flush operation
     * is presently a noop. However, if the write implementation changes to use
     * some form of write-back cache, this function should be implemented to
     * flush any cached writes to the SD card accordingly. */
    return result_ok();
}

result_t mmc_driver_write_blocks(
        size_t lba,
        size_t num_blocks,
        size_t block_size,
        char *src_buffer,
        size_t src_buffer_len
) {
    if (src_buffer == NULL) {
        return result_err("NULL `src_buffer` in mmc_driver_write_blocks().");
    }
    sdhci_result_t sdhci_result;
    return sdhci_write_blocks(
            &global_sdhci_regs,
            &global_sdcard,
            lba,
            num_blocks,
            block_size,
            src_buffer,
            src_buffer_len,
            &sdhci_result
    );
}

result_t mmc_driver_read_blocks(
        size_t lba,
        size_t num_blocks,
        size_t block_size,
        char *dst_buffer,
        size_t dst_buffer_len
) {
    if (dst_buffer == NULL) {
        return result_err("NULL `dst_buffer` in mmc_driver_read_blocks().");
    }
    sdhci_result_t sdhci_result;
    return sdhci_read_blocks(
            &global_sdhci_regs,
            &global_sdcard,
            lba,
            num_blocks,
            block_size,
            dst_buffer,
            dst_buffer_len,
            &sdhci_result
    );
}

void notified(sel4cp_channel ch) {
    switch(ch) {
        case MMC_DRIVER_TO_FATFS_REQUEST_CHANNEL: {
            blk_request_queue_t *request_queue = (blk_request_queue_t *) mmc_driver_request_queue;
            blk_response_queue_t *response_queue = (blk_response_queue_t *) mmc_driver_response_queue;
            /* Get the SD card's block size. */
            size_t block_size = 0;
            result_t res = mmc_driver_get_block_size((uint16_t *) &block_size);
            if (result_is_err(res)) {
                result_printf(res);
                return;
            }
            /* The while-loop continues to dequeue requests until there are no
             * requests left in the Requests queue. In our current setup (no
             * scheduling budget or period set), this while-loop is unnecessary
             * since this PD will never be preempted, which means there will
             * never be more than 1 request in the Request queue. However, this
             * while-loop becomes necessary if we eventually ascribe a
             * scheduling budget and period to this PD (which we do for the
             * ethernet driver), which would open the possibility for this PD to
             * be preempted before we've finished processing the buffer in the
             * Request queue. As such, by the time this PD is rescheduled, the
             * lower-priority `fatfs` PD may have added a new buffer to the
             * Request queue, which we may as well just dequeue and process once
             * we are restarted by the kernel. */
            blk_request_t request = {0};
            while (blk_request_queue_dequeue(
                    request_queue,
                    &request
            ) == OK_BLK_REQUEST_QUEUE) {
                /* Get `shared_data_buf` from `request`. */
                blk_shared_data_buf_t shared_data_buf = {0};
                if (blk_request_get_shared_data_buf(
                        &request,
                        &shared_data_buf
                ) != OK_BLK_REQUEST) {
                    log_error("Failed to get shared data buffer from request.");
                    break;
                }
                /* Get the size of the `shared_data_buf`. */
                size_t buf_size = 0;
                if (blk_shared_data_buf_get_buf_size(
                        &shared_data_buf,
                        &buf_size
                ) != OK_BLK_SHARED_DATA_BUF) {
                    log_error("Failed to get size of shared data buffer.");
                    break;
                }
                /* Get the virtual address of `shared_data_buf`. */
                uintptr_t buf_vaddr = 0;
                if (blk_shared_data_buf_get_buf_vaddr(
                        &shared_data_buf,
                        &buf_vaddr
                ) != OK_BLK_SHARED_DATA_BUF) {
                    log_error("Failed to get virtual address of shared data buffer.");
                    break;
                }
                /* Get the Request's operation. */
                blk_request_operation_t request_operation = 0;
                if (blk_request_get_operation(
                        &request,
                        &request_operation
                ) != OK_BLK_REQUEST) {
                    log_error("Failed to get request operation.");
                    break;
                }
                res = result_ok();
                /* We call a different `mmc_driver` function depending on the operation. */
                switch (request_operation) {
                    case GET_NUM_BLOCKS: {
                        /* Sanity check the buffer size. */
                        if (buf_size < sizeof(uint64_t)) {
                            log_error("Invalid Shared Data buffer size for `GET_NUM_BLOCKS`.");
                            break;
                        }
                        /* Get the number of blocks on the MMC. */
                        res = mmc_driver_get_num_blocks(
                                (uint64_t *) buf_vaddr
                        );
                        break;
                    }
                    case GET_BLOCK_SIZE: {
                        /* Sanity check the buffer size. */
                        if (buf_size < sizeof(size_t)) {
                            log_error("Invalid Shared Data buffer size for `GET_NUM_BLOCKS`.");
                            break;
                        }
                        *((size_t *) buf_vaddr) = block_size;
                        break;
                    }
                    case SYNC: {
                        res = mmc_driver_write_flush();
                        break;
                    }
                    case READ: {
                        /* Get number of blocks. */
                        size_t num_blocks = 0;
                        if (blk_request_get_num_blocks(
                                &request,
                                &num_blocks
                        ) != OK_BLK_REQUEST) {
                            log_error("Failed to get number of blocks from request.");
                            break;
                        }
                        /* Get lba. */
                        size_t lba = 0;
                        if (blk_request_get_lba(
                                &request,
                                &lba
                        ) != OK_BLK_REQUEST) {
                            log_error("Failed to get lba from request.");
                            break;
                        }
                        size_t const request_size = num_blocks * block_size;
                        /* Sanity check the buffer size. */
                        if (buf_size < request_size) {
                            log_error("Invalid Shared Data buffer size for `READ`.");
                            break;
                        }
                        res = mmc_driver_read_blocks(
                                lba,
                                num_blocks,
                                block_size,
                                (char *) buf_vaddr,
                                request_size
                        );
                        break;
                    }
                    case WRITE: {
                        /* Get number of blocks. */
                        size_t num_blocks = 0;
                        if (blk_request_get_num_blocks(
                                &request,
                                &num_blocks
                        ) != OK_BLK_REQUEST) {
                            log_error("Failed to get number of blocks from request.");
                            break;
                        }
                        /* Get lba. */
                        size_t lba = 0;
                        if (blk_request_get_lba(
                                &request,
                                &lba
                        ) != OK_BLK_REQUEST) {
                            log_error("Failed to get lba from request.");
                            break;
                        }
                        size_t const request_size = num_blocks * block_size;
                        /* Sanity check the buffer size. */
                        if (buf_size < request_size) {
                            log_error("Invalid Shared Data buffer size for `WRITE`.");
                            break;
                        }
                        res = mmc_driver_write_blocks(
                                lba,
                                num_blocks,
                                block_size,
                                (char *) buf_vaddr,
                                request_size
                        );
                        break;
                    }
                }
                blk_response_t response = {0};
                /* If there is an error, we send the full error message in `res`
                 * back to the client in an "error" Response. Otherwise, we send
                 * an "ok" Response back to the client. */
                if (result_is_err(res)) {
                    /* Get the error message associated with `res` and
                     * copy it into the Shared Data Buffer. */
                    result_get_err_msg(
                            res,
                            (char *) buf_vaddr,
                            buf_size
                    );
                    /* Initialise an "error" Response to be sent back to
                     * client. */
                    if (blk_response_init_error(
                            &response,
                            &shared_data_buf
                    ) != OK_BLK_RESPONSE) {
                        log_error("Failed to initialise error response.");
                    }
                } else {
                    /* Initialise an "ok" Response to be sent back to
                     * client. */
                    if (blk_response_init_ok(
                            &response,
                            &shared_data_buf
                    ) != OK_BLK_RESPONSE) {
                        log_error("Failed to initialise ok response.");
                        break;
                    }
                }
                /* Enqueue the `response` onto the `response_queue` for
                 * the client. */
                if (blk_response_queue_enqueue(
                        response_queue,
                        &response
                ) != OK_BLK_RESPONSE_QUEUE) {
                    log_error("Failed to enqueue response.");
                    break;
                }
            }
            break;
        }
        default:
            printf("MMC Driver: received notification on unexpected channel\n");
            break;
    }
}
