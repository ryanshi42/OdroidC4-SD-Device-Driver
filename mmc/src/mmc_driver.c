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
    log_info("Successfully finished E2E tests.");

    int rc;
    DWORD buff[FF_MAX_SS];  /* Working buffer (4 sector in size) */

    /* Check function/compatibility of the physical drive #0 */
    rc = test_diskio(0, 3, buff, sizeof buff);

    if (rc) {
        printf("Sorry the function/compatibility test failed. (rc=%d)\nFatFs will not work with this disk driver.\n", rc);
    } else {
        printf("Congratulations! The disk driver works well.\n");
    }
}

void notified(sel4cp_channel ch) {

}
