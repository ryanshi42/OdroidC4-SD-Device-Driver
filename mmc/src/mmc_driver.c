#include "mmc_driver.h"

/* Used for transporting chars from `mmc_driver` to `serial_client`. */
uintptr_t mmc_to_serial_client_putchar_buf;

uintptr_t shared_dma;

/* `rx_` stands for "Receive". The following two Receive buffers are to assist
 * with transporting data from `timer_driver` to `mmc_driver`. */

/* In the SDDF's design-documentation, `rx_avail_ring_buf` is referred to as the
 * "Transmit-Free" (TxF) ring buffer. `rx_avail_ring_buf` holds all the buffers
 * in the `shared_dma` data region that are ready to be reused for transporting
 * new data from `timer_driver` to `mmc_driver`. */
uintptr_t rx_avail_ring_buf;

/* In the SDDF's design-documentation, `rx_used_ring_buf` is referred to as the
 * "Transmit-Available" (TxA) ring buffer. `rx_used_ring_buf` holds all the
 * buffers in the `shared_dma` data region that currently hold data sent by
 * `timer_driver` for the `mmc_driver` PD to process.  */
uintptr_t rx_used_ring_buf;

/* Base virtual address for the GPIO registers. We've deliberately set this
 * value to 0x3f200000 in `mmc.system`, which is the Physical Address the GPIO
 * registers are actually mapped to. */
uintptr_t gpio_base_vaddr;

/* Base virtual address for the Timer registers. We've deliberately set this
 * value to 0x3f003000 in `mmc.system`, which is the Physical Address the Timer
 * registers are actually mapped to. */
uintptr_t timer_base_vaddr;

/* Base virtual address for the EMMC registers. We've deliberately set this
 * value to 0x3f300000 in `mmc.system`, which is the Physical Address the EMMC
 * registers are actually mapped to. */
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
            shared_dma,
            rx_avail_ring_buf,
            rx_used_ring_buf,
            MMC_DRIVER_TO_TIMER_DRIVER_GET_NUM_TICKS_CHANNEL
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

    /* Run E2E tests to verify sleep works.*/
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

    char buf[512] = {0};
    /* Using the last 4 bytes on the second sector as a boot counter */
    unsigned int *counter = (unsigned int *) (buf + 508);
    size_t block_size = 512;
    /* Do not use the first sector (lba 0), could render your card unbootable
     * choose a sector which is unused by your partitions */
    size_t counter_sector = 1;
    /* Initialise the block to 0. */
    res = sdhci_write_blocks(
            (bcm_emmc_regs_t *) emmc_base_vaddr,
            &global_sdcard,
            counter_sector,
            1,
            block_size,
            buf,
            sizeof(buf),
            &sdhci_result
    );
    if (result_is_err(res)) {
        printf("Failed to write 0 to SD card.\n");
        result_printf(res);
        return;
    }
    for (int i = 0; i < 0x3; i++) {
        /* Read the block. */
        res = sdhci_read_blocks(
                (bcm_emmc_regs_t *) emmc_base_vaddr,
                &global_sdcard,
                counter_sector,
                1,
                block_size,
                buf,
                sizeof(buf),
                &sdhci_result
        );
        if (result_is_err(res)) {
            printf("Failed to read SD card.\n");
            return;
        }
        /* Increment the block. */
        (*counter)++;
        /* Write the block to disk. */
        res = sdhci_write_blocks(
                (bcm_emmc_regs_t *) emmc_base_vaddr,
                &global_sdcard,
                counter_sector,
                1,
                block_size,
                buf,
                sizeof(buf),
                &sdhci_result
        );
        if (result_is_err(res)) {
            printf("Failed to write 0x%lx to SD card.\n", (uintptr_t) *counter);
            return;
        }
    }
    /* Finally, read the block and print it out. */
    /* Read the block. */
    res = sdhci_read_blocks(
            (bcm_emmc_regs_t *) emmc_base_vaddr,
            &global_sdcard,
            counter_sector,
            1,
            block_size,
            buf,
            sizeof(buf),
            &sdhci_result
    );
    if (result_is_err(res)) {
        printf("Failed to read SD card.\n");
        return;
    }
    printf("Boot counter 0x%lx written to SD card.\n", (uintptr_t) *counter);
}

void notified(sel4cp_channel ch) {

}
