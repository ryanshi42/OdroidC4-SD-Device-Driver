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

/* Global `bcm_emmc`. */
bcm_emmc_t global_bcm_emmc = {0};

/* Global `timer_client`. */
timer_client_t global_timer_client = {0};

/* Global `sdcard`. */
sdcard_t global_sdcard = {0};

//// get the end of bss segment from linker
//extern unsigned char _end;

// do not use the first sector (lba 0), could render your card unbootable
// choose a sector which is unused by your partitions
#define COUNTER_SECTOR 1

void init(void) {
    char buf[512] = {0};
    // use the last 4 bytes on the second sector as a boot counter
    unsigned int *counter = (unsigned int*)(buf + 508);
//    unsigned int *counter = (unsigned int*)(&_end + 508);

    /* Initialise `printf`. */
    printf_init(
            mmc_to_serial_client_putchar_buf,
            MMC_DRIVER_TO_SERIAL_CLIENT_PUTCHAR_CHANNEL
    );

    /* Initialise `timer_client`. */
    result_t res_timer = timer_client_init(
            &global_timer_client,
            shared_dma,
            rx_avail_ring_buf,
            rx_used_ring_buf,
            MMC_DRIVER_TO_TIMER_DRIVER_GET_NUM_TICKS_CHANNEL
    );
    if (result_is_err(res_timer)) {
        result_printf(res_timer);
        return;
    }

    /* Initialise sleep library. */
    result_t res_sleep_init = sleep_init(&global_timer_client);
    if (result_is_err(res_sleep_init)) {
        result_printf(res_sleep_init);
        return;
    }

    /* Initialise sdcard. */
    result_t res_sdcard = sdcard_init(
            &global_sdcard
    );
    if (result_is_err(res_sdcard)) {
        result_printf(res_sdcard);
        return;
    }

    uint64_t start_ticks, finish_ticks, delta_ticks;
    start_ticks = clock_getticks();
    usleep(1000000);
    finish_ticks = clock_getticks();
    delta_ticks = finish_ticks - start_ticks;
    printf("delta_ticks: %llu\n", delta_ticks);
    assert(delta_ticks - 1000000 <= 2000);

    start_ticks = clock_getticks();
    sleep_cyc(150);
    finish_ticks = clock_getticks();
    delta_ticks = finish_ticks - start_ticks;
    printf("delta_ticks: %llu\n", delta_ticks);
    assert(delta_ticks <= 1);

    long r, cnt, ccs = 0;
    // GPIO_CD
    r = *GPFSEL4;
    r &= ~(7 << (7 * 3));
    *GPFSEL4 = r;
    *GPPUD = 2;
    wait_cycles(150);
    *GPPUDCLK1 = (1 << 15);
    wait_cycles(150);
    *GPPUD = 0;
    *GPPUDCLK1 = 0;
    r = *GPHEN1;
    r |= 1 << 15;
    *GPHEN1 = r;

    // GPIO_CLK, GPIO_CMD
    r = *GPFSEL4;
    r |= (7 << (8 * 3)) | (7 << (9 * 3));
    *GPFSEL4 = r;
    *GPPUD = 2;
    wait_cycles(150);
    *GPPUDCLK1 = (1 << 16) | (1 << 17);
    wait_cycles(150);
    *GPPUD = 0;
    *GPPUDCLK1 = 0;

    // GPIO_DAT0, GPIO_DAT1, GPIO_DAT2, GPIO_DAT3
    r = *GPFSEL5;
    r |= (7 << (0 * 3)) | (7 << (1 * 3)) | (7 << (2 * 3)) | (7 << (3 * 3));
    *GPFSEL5 = r;
    *GPPUD = 2;
    wait_cycles(150);
    *GPPUDCLK1 = (1 << 18) | (1 << 19) | (1 << 20) | (1 << 21);
    wait_cycles(150);
    *GPPUD = 0;
    *GPPUDCLK1 = 0;

    result_t res = bcm_emmc_init(
            &global_bcm_emmc,
            (bcm_emmc_regs_t *) emmc_base_vaddr
    );
    if (result_is_err(res)) {
        result_printf(res);
        return;
    }

    /* Initialise the SD card. */
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

    if (sd_init((bcm_emmc_regs_t *) emmc_base_vaddr, &global_sdcard) != SD_OK) {
        printf("Failed to initialise SD card.\n");
    }

    printf("Successfully initialised SD card.\n");
//    bool is_write_success = false;
//    bool is_read_success = false;
//    /* Initialise the block to 0. */
//    is_write_success = sd_writeblock(buf, COUNTER_SECTOR, 1);
    res = sdhci_write_blocks(
            (bcm_emmc_regs_t *) emmc_base_vaddr,
            &global_sdcard,
            COUNTER_SECTOR,
            1,
            buf,
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
                COUNTER_SECTOR,
                1,
                buf,
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
                COUNTER_SECTOR,
                1,
                buf,
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
            COUNTER_SECTOR,
            1,
            buf,
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
