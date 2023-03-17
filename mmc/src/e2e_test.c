#include "e2e_test.h"

result_t e2e_test_sleep() {
    log_info("Starting e2e_test_sleep().");

    uint64_t start_ticks, finish_ticks, delta_ticks;
    start_ticks = clock_getticks();
    usleep(1000000);
    finish_ticks = clock_getticks();
    delta_ticks = finish_ticks - start_ticks;
    log_info("delta_ticks (usleep): %lu", delta_ticks);
    assert(delta_ticks - 1000000 <= 2000);

    start_ticks = clock_getticks();
    sleep_cyc(150);
    finish_ticks = clock_getticks();
    delta_ticks = finish_ticks - start_ticks;
    log_info("delta_ticks (sleep_cyc): %lu", delta_ticks);
    assert(delta_ticks <= 10);

    log_info("Finished e2e_test_sleep().");
    return result_ok();
}

result_t e2e_read_write_simple(
        bcm_emmc_regs_t *bcm_emmc_regs,
        sdcard_t *sdcard
) {
    log_info("Starting e2e_read_write_simple().");
    result_t res;
    sdhci_result_t sdhci_result;

    char buf[512] = {0};
    /* Using the last 4 bytes on the second sector as a boot counter */
    unsigned int *counter = (unsigned int *) (buf + 508);
    size_t block_size = 512;
    /* Do not use the first sector (lba 0), could render your card unbootable
     * choose a sector which is unused by your partitions */
    size_t counter_sector = 1;
    /* Initialise the block to 0. */
    res = sdhci_write_blocks(
            bcm_emmc_regs,
            sdcard,
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
        return result_ok();
    }
    for (int i = 0; i < 0x3; i++) {
        /* Read the block. */
        res = sdhci_read_blocks(
                bcm_emmc_regs,
                sdcard,
                counter_sector,
                1,
                block_size,
                buf,
                sizeof(buf),
                &sdhci_result
        );
        if (result_is_err(res)) {
            printf("Failed to read SD card.\n");
            return result_ok();
        }
        /* Increment the block. */
        (*counter)++;
        /* Write the block to disk. */
        res = sdhci_write_blocks(
                bcm_emmc_regs,
                sdcard,
                counter_sector,
                1,
                block_size,
                buf,
                sizeof(buf),
                &sdhci_result
        );
        if (result_is_err(res)) {
            printf("Failed to write 0x%lx to SD card.\n", (uintptr_t) *counter);
            return result_ok();
        }
    }
    /* Finally, read the block and print it out. */
    /* Read the block. */
    res = sdhci_read_blocks(
            bcm_emmc_regs,
            sdcard,
            counter_sector,
            1,
            block_size,
            buf,
            sizeof(buf),
            &sdhci_result
    );
    if (result_is_err(res)) {
        printf("Failed to read SD card.\n");
        return result_ok();
    }
    printf("Boot counter 0x%lx written to SD card.\n", (uintptr_t) *counter);

    log_info("Finished e2e_read_write_simple().");
    return result_ok();
}


