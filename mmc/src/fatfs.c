#include "fatfs.h"

/* Used for transporting chars from `fatfs` to `serial_client`. */
uintptr_t fatfs_to_serial_client_putchar_buf;

void init(void) {

    /* Initialise `printf`. The `log.h` library depends on `printf` being
     * initialised first. */
    printf_init(
            fatfs_to_serial_client_putchar_buf,
            FATFS_TO_SERIAL_CLIENT_PUTCHAR_CHANNEL
    );

    log_trace("Starting init() in FatFS Protection Domain...");
}

void notified(sel4cp_channel ch) {

}
