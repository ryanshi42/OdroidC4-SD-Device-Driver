#include "mmc_driver.h"

/* Used for transporting chars from `mmc_driver` to `serial_client`. */
uintptr_t mmc_to_serial_client_putchar_buf;

/* Used for transporting num ticks from `timer_driver` to `mmc_driver`. */
uintptr_t timer_driver_to_mmc_driver_numticks_buf;

/* Base virtual address for the GPIO registers. */
uintptr_t gpio_base_vaddr;

/* Base virtual address for the EMMC registers. */
uintptr_t emmc_base_vaddr;

/* Global `timer_client`. */
timer_client_t global_timer_client = {0};

/* Global `sdcard`. */
sdcard_t global_sdcard = {0};

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
    e2e_test_sleep();

    /* Initialise and reset the Pi's SD card Host Controller. */
    res = bcm_emmc_init(
            (bcm_emmc_regs_t *) emmc_base_vaddr,
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
            (bcm_emmc_regs_t *) emmc_base_vaddr,
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
            (bcm_emmc_regs_t *) emmc_base_vaddr,
            &global_sdcard,
            &sdhci_result
    );
    if (result_is_err(res)) {
        result_printf(res);
        return;
    }
    log_trace("Finished setting SD bus width to maximum possible value.");

    /* Running E2E tests to verify our SD card driver works properly.*/
    res = e2e_test_read_write_simple(
            (bcm_emmc_regs_t *) emmc_base_vaddr,
            &global_sdcard
    );
    if (result_is_err(res)) {
        result_printf(res);
        return;
    }
    res = e2e_test_read_write_multiple_blocks(
            (bcm_emmc_regs_t *) emmc_base_vaddr,
            &global_sdcard
    );
    if (result_is_err(res)) {
        result_printf(res);
        return;
    }
    res = e2e_test_sdcard_card_specific_data(&global_sdcard);
    if (result_is_err(res)) {
        result_printf(res);
        return;
    }
    log_info("Successfully finished E2E tests.");

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

    log_trace("Finished running E2E tests in MMC Driver Protection Domain!");
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
            (bcm_emmc_regs_t *) emmc_base_vaddr,
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
            (bcm_emmc_regs_t *) emmc_base_vaddr,
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

}
