#include "mmc_driver.h"

/* Used for transporting chars from `uart` to `serial_client`. */
uintptr_t mmc_to_serial_client_putchar_buf;

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

    /* Initialise printf. */
    printf_init(
            mmc_to_serial_client_putchar_buf,
            MMC_TO_SERIAL_CLIENT_PUTCHAR_CHANNEL
    );

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

    result_t res = bcm_emmc_init(&global_bcm_emmc, emmc_base_vaddr);
    if (result_is_err(res)) {
        printf("ERROR: failed to initialize EMMC\n");
        return;
    }

    if (sd_init() != SD_OK) {
        printf("Failed to initialise SD card.\n");
    }

    printf("Successfully initialised SD card.\n");
    bool is_write_success = false;
    bool is_read_success = false;
    /* Initialise the block to 0. */
    is_write_success = sd_writeblock(buf, COUNTER_SECTOR, 1);
    if (!is_write_success) {
        printf("Failed to write 0 to SD card.\n");
        return;
    }
    for (int i = 0; i < 0x3; i++) {
        /* Read the block. */
        is_read_success = sd_readblock(COUNTER_SECTOR, buf, 1);
        if (!is_read_success) {
            printf("Failed to read SD card.\n");
            return;
        }
        /* Increment the block. */
        (*counter)++;
        /* Write the block to disk. */
        is_write_success = sd_writeblock(buf, COUNTER_SECTOR, 1);
        if (!is_write_success) {
            printf("Failed to write 0x%lx to SD card.\n", (uintptr_t) *counter);
            return;
        }
    }
    /* Finally, read the block and print it out. */
    is_read_success = sd_readblock(COUNTER_SECTOR, buf, 1);
    if (!is_read_success) {
        printf("Failed to read SD card.\n");
        return;
    }
    printf("Boot counter 0x%lx written to SD card.\n", (uintptr_t) *counter);
}

void notified(sel4cp_channel ch) {

}
