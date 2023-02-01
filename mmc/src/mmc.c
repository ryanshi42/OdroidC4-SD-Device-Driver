#include <stdint.h>
#include <sel4cp.h>
#include "mmc.h"
#include "uart.h"

/* Used for transporting chars from `uart` to `serial_client`. */
uintptr_t mmc_to_serial_client_putchar_buf;

void init(void) {
    /* Initialise the UART. */
    uart_init(
            mmc_to_serial_client_putchar_buf,
            MMC_TO_SERIAL_CLIENT_PUTCHAR_CHANNEL
    );
    uart_puts("Successfully initialised UART in MMC PD.\n");
}

void notified(sel4cp_channel ch) {

}