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

result_t e2e_test_read_write_simple(
        bcm_emmc_regs_t *bcm_emmc_regs,
        sdcard_t *sdcard
) {
    log_info("Starting e2e_test_read_write_simple().");
    result_t res;
    sdhci_result_t sdhci_result;

    /* Initialise our buffer to zero. */
    char buf[512] = {0};
    size_t buf_len = sizeof(buf);
    size_t num_blocks = 1;
    size_t block_size = 512;
    /* The buffer length should the same size as `num_blocks` * `block_size`. */
    assert(buf_len == num_blocks * block_size);
    /* Do not use the first sector (lba 0), could render your card unbootable
     * choose a sector which is unused by your partitions */
    size_t lba_counter = 1;
    /* Using the last 4 bytes on the second sector as a counter */
    uint32_t *counter = (uint32_t *) (buf + 508);
    /* Write a single zeroed block to the SD card. */
    res = sdhci_write_blocks(
            bcm_emmc_regs,
            sdcard,
            lba_counter,
            num_blocks,
            block_size,
            buf,
            buf_len,
            &sdhci_result
    );
    if (result_is_err(res)) {
        log_info("Failed to write a single zeroed block to the SD card.");
        result_printf(res);
        return result_ok();
    }
    size_t num_iterations = 20;
    for (int i = 0; i < num_iterations; i++) {
        /* Read the block. */
        res = sdhci_read_blocks(
                bcm_emmc_regs,
                sdcard,
                lba_counter,
                num_blocks,
                block_size,
                buf,
                buf_len,
                &sdhci_result
        );
        if (result_is_err(res)) {
            log_info("Failed to read block from SD card on iteration %d. Counter was %d.", i, *counter);
            result_printf(res);
            return result_ok();
        }
        /* Increment the counter. */
        (*counter)++;
        /* Write the block to disk. */
        res = sdhci_write_blocks(
                bcm_emmc_regs,
                sdcard,
                lba_counter,
                num_blocks,
                block_size,
                buf,
                buf_len,
                &sdhci_result
        );
        if (result_is_err(res)) {
            log_info("Failed to write block to SD card on iteration %d. Counter is %d", i, *counter);
            result_printf(res);
            return result_ok();
        }
    }
    /* Read the block. */
    res = sdhci_read_blocks(
            bcm_emmc_regs,
            sdcard,
            lba_counter,
            num_blocks,
            block_size,
            buf,
            buf_len,
            &sdhci_result
    );
    if (result_is_err(res)) {
        log_info("Failed to read block from SD card.");
        result_printf(res);
        return result_ok();
    }
    log_info("Num iterations was %d and Counter was %d.", num_iterations, *counter);
    /* Assert the counter equals `num_iterations`. */
    assert(num_iterations == *counter);

    log_info("Finished e2e_test_read_write_simple().");
    return result_ok();
}

result_t e2e_test_read_write_multiple_blocks(
        bcm_emmc_regs_t *bcm_emmc_regs,
        sdcard_t *sdcard
) {
    log_info("Starting e2e_test_read_write_multiple_blocks().");
    result_t res;
    sdhci_result_t sdhci_result;

    /* Initialise our buffer to zero. We use the largest possible buffer without
     * overflowing the Core Platform's stack space, hence the strange buffer
     * size. */
    char buf[4608] = {0};
    size_t buf_len = sizeof(buf);
    size_t num_blocks = 9;
    size_t block_size = 512;
    /* The buffer length should the same size as `num_blocks` * `block_size`. */
    assert(buf_len == num_blocks * block_size);
    /* Do not use the first sector (lba 0), could render your card unbootable
     * choose a sector which is unused by your partitions */
    size_t lba = 2;

    /* Fill our buffer to all 'a'. */
    memset(buf, 'a', buf_len);

    /* Write our buffer to the SD card. */
    res = sdhci_write_blocks(
            bcm_emmc_regs,
            sdcard,
            lba,
            num_blocks,
            block_size,
            buf,
            buf_len,
            &sdhci_result
    );
    if (result_is_err(res)) {
        log_info("Failed to write block to the SD card.");
        result_printf(res);
        return result_ok();
    }
    /* Clear out our buffer. */
    memset(buf, 0, buf_len);
    /* Read the block into our cleared out buffer. */
    res = sdhci_read_blocks(
            bcm_emmc_regs,
            sdcard,
            lba,
            num_blocks,
            block_size,
            buf,
            buf_len,
            &sdhci_result
    );
    if (result_is_err(res)) {
        log_info("Failed to read blocks from SD card.");
        result_printf(res);
        return result_ok();
    }
    for (int i = 0; i < buf_len; i++) {
        /* Assert that the buffer is filled with 'a'. */
        assert(buf[i] == 'a');
    }
    log_info("Finished e2e_test_read_write_multiple_blocks().");
    return result_ok();
}

result_t e2e_test_sdcard_card_specific_data(sdcard_t *sdcard) {
    log_info("Starting e2e_test_sdcard_card_specific_data().");

    /* Get the memory capacity of SD card. */
    uint64_t memory_capacity = 0;
    result_t res = sdcard_get_memory_capacity(sdcard, &memory_capacity);
    if (result_is_err(res)) {
        log_info("Failed to get memory capacity of SD card.");
        result_printf(res);
        return result_ok();
    }
    log_info("Memory capacity of SD card is %ld bytes.", memory_capacity);
    assert(64021856256 == memory_capacity);

    /* Get number of blocks of SD card. */
    uint64_t num_blocks = 0;
    res = sdcard_get_num_blocks(sdcard, &num_blocks);
    if (result_is_err(res)) {
        log_info("Failed to get num blocks from SD card.");
        result_printf(res);
        return result_ok();
    }
    log_info("Num blocks on SD card is %ld.", num_blocks);
    assert(125042688 == num_blocks);

    /* Get block size of SD card. */
    uint16_t block_size = 0;
    res = sdcard_get_block_size(sdcard, &block_size);
    if (result_is_err(res)) {
        log_info("Failed to get block size from SD card.");
        result_printf(res);
        return result_ok();
    }
    log_info("Block size of SD card is %d bytes.", block_size);
    assert(512 == block_size);

    log_info("Finished e2e_test_sdcard_card_specific_data().");
    return result_ok();
}
