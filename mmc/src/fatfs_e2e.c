#include "fatfs_e2e.h"

result_t fatfs_e2e_diskio_test(void) {
    log_info("Starting fatfs_e2e_diskio_test().");

    int rc;
    DWORD buff[FF_MAX_SS];  /* Working buffer (4 sector in size) */
    /* Check function/compatibility of the physical drive #0 */
    rc = test_diskio(0, 3, buff, sizeof buff);
    if (rc) {
        log_info("Sorry the function/compatibility test failed. (rc=%d)\nFatFs will not work with this disk driver.", rc);
    }
    assert(rc == 0);
    log_info("Congratulations! The disk driver works well.");

    log_info("Finished fatfs_e2e_diskio_test().");
    return result_ok();
}

result_t fatfs_e2e_write_close_read_simple(void) {
    log_info("Starting fatfs_e2e_read_write_simple().");

    FATFS fs;
    FRESULT res;
    FIL fp = {0};
    const char *path = "fatfs_e2e_write_close_read_simple.txt";
    char str[] = "Hello World!";
    size_t str_len = sizeof(str);

    res = f_mount(&fs, "", 0);
    if (res != FR_OK) {
        log_info("Error mounting FS with res of %d.", res);
    }
    assert(FR_OK == res);

    res = f_open(&fp, path, FA_WRITE | FA_CREATE_ALWAYS);
    if (res != FR_OK) {
        log_info("Error opening file with res of %d.", res);
    }
    assert(FR_OK == res);

    unsigned int bytes_written;
    res = f_write(&fp, str, str_len, (UINT *) &bytes_written);
    if (res != FR_OK) {
        log_info("Error writing to file with res of %d.", res);
    }
    assert(FR_OK == res);
    log_info("Wrote %d bytes to file.", bytes_written);
    assert(str_len == bytes_written);

    res = f_close(&fp);
    if (res != FR_OK) {
        log_info("Error closing file with res of %d.", res);
    }
    assert(FR_OK == res);

    res = f_open(&fp, path, FA_READ);
    if (res != FR_OK) {
        log_info("Error opening file with res of %d.", res);
    }
    assert(FR_OK == res);

    unsigned int bytes_read;
    char read_buf[100] = {0};
    res = f_read(&fp, read_buf, str_len, (UINT *) &bytes_read);
    if (res != FR_OK) {
        log_info("Error reading file with res of %d.", res);
    }
    assert(FR_OK == res);
    log_info("Read %d bytes from file.", bytes_read);
    assert(str_len == bytes_read);
    log_info("Read string: %s", read_buf);

    res = f_close(&fp);
    if (res != FR_OK) {
        log_info("Error closing file with res of %d.", res);
    }
    assert(FR_OK == res);

    log_info("Finished fatfs_e2e_read_write_simple().");
    return result_ok();
}

result_t fatfs_e2e_write_fsync_read_close_simple(void) {
    log_info("Starting fatfs_e2e_write_fsync_read_close_simple().");

    FATFS fs;
    FRESULT res;
    FIL fp = {0};
    char const *path = "fatfs_e2e_write_fsync_read_close_simple.txt";
    char const str[] = "Hello World!";
    size_t const str_len = sizeof(str);

    res = f_mount(&fs, "", 0);
    if (res != FR_OK) {
        log_info("Error mounting FS with res of %d.", res);
    }
    assert(FR_OK == res);

    res = f_open(&fp, path, FA_WRITE | FA_READ | FA_CREATE_ALWAYS);
    if (res != FR_OK) {
        log_info("Error opening file with res of %d.", res);
    }
    assert(FR_OK == res);

    unsigned int bytes_written;
    res = f_write(&fp, str, str_len, (UINT *) &bytes_written);
    if (res != FR_OK) {
        log_info("Error writing to file with res of %d.", res);
    }
    assert(FR_OK == res);
    log_info("Wrote %d bytes to file.", bytes_written);
    assert(str_len == bytes_written);

    res = f_sync(&fp);
    if (res != FR_OK) {
        log_info("Error syncing file with res of %d.", res);
    }
    assert(FR_OK == res);

    /* Move file pointer to beginning of file. */
    res = f_lseek(&fp, 0);
    if (res != FR_OK) {
        log_info("Error seeking file with res of %d.", res);
    }

    unsigned int bytes_read;
    char read_buf[100] = {0};
    res = f_read(&fp, read_buf, str_len, (UINT *) &bytes_read);
    if (res != FR_OK) {
        log_info("Error reading file with res of %d.", res);
    }
    assert(FR_OK == res);
    log_info("Read %d bytes from file.", bytes_read);
    assert(str_len == bytes_read);
    log_info("Read string: %s", read_buf);

    res = f_close(&fp);
    if (res != FR_OK) {
        log_info("Error closing file with res of %d.", res);
    }
    assert(FR_OK == res);

    log_info("Finished fatfs_e2e_write_fsync_read_close_simple().");
    return result_ok();
}

result_t fatfs_e2e_write_read_large(size_t cluster_size_in_bytes) {
    log_info("Starting fatfs_e2e_write_read_large().");

    FATFS fs;
    FRESULT res = f_mount(&fs, "", 0);
    if (res != FR_OK) {
        log_info("Error mounting FS with res of %d.", res);
    }
    assert(FR_OK == res);

    /* Create a buffer twice the size of a cluster. */
    size_t const buf_size = 2 * cluster_size_in_bytes * sizeof(char);
    char *buf_write = (char *) malloc(buf_size);
    assert(buf_write != NULL);
    /* Make the buffer all 'a'. */
    memset(buf_write, 'a', buf_size);

    /* Create a new file for testing. */
    FIL fp = {0};
    char const *path = "fatfs_e2e_write_read_large.txt";
    res = f_open(&fp, path, FA_WRITE | FA_READ | FA_CREATE_ALWAYS);
    if (res != FR_OK) {
        log_info("Error opening file with res of %d.", res);
    }
    assert(FR_OK == res);

    /* Write the entire buffer to the file. */
    unsigned int bytes_written;
    res = f_write(&fp, buf_write, buf_size, (UINT *) &bytes_written);
    if (res != FR_OK) {
        log_info("Error writing to file with res of %d.", res);
    }
    assert(FR_OK == res);
    log_info("Wrote %d bytes to file.", bytes_written);
    assert(buf_size == bytes_written);

    /* Move file pointer to beginning of file. */
    res = f_lseek(&fp, 0);
    if (res != FR_OK) {
        log_info("Error seeking file with res of %d.", res);
    }

    /* We allocate another buffer for reading. */
    char *buf_read = (char *) malloc(buf_size);
    assert(buf_read != NULL);
    /* Make the buffer all 'b'. */
    memset(buf_read, 'b', buf_size);

    /* Now we try to read the buffer we just wrote. */
    unsigned int bytes_read;
    res = f_read(&fp, buf_read, buf_size, (UINT *) &bytes_read);
    if (res != FR_OK) {
        log_info("Error reading file with res of %d.", res);
    }
    assert(FR_OK == res);
    log_info("Read %d bytes from file.", bytes_read);
    assert(buf_size == bytes_read);

    /* We must also check the buffers are identical. */
    assert(memcmp(buf_write, buf_read, buf_size) == 0);

    /* Then we close the file. */
    res = f_close(&fp);
    if (res != FR_OK) {
        log_info("Error closing file with res of %d.", res);
    }
    assert(FR_OK == res);

    log_info("Finished fatfs_e2e_write_read_large().");
    return result_ok();
}

