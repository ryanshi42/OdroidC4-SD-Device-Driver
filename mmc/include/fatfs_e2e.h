#pragma once

#include "result.h"
#include "diskio_test.h"
#include "ff.h"
#include "assert.h"
#include "log.h"

/**
 * Tests the low-level diskio.c glue functions that call into our MMC driver.
 * @return
 */
result_t fatfs_e2e_diskio_test(void);

/**
 * Simple read/write test.
 * @return
 */
result_t fatfs_e2e_write_close_read_simple(void);

/**
 * Tests fsync.
 * @return
 */
result_t fatfs_e2e_write_fsync_read_close_simple(void);
