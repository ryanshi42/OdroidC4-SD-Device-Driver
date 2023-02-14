#include <stdint.h>
#include <sel4cp.h>
#include "mmc.h"
#include "uart.h"
#include "sd.h"

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
    /* Initialise the UART. */
    uart_init(
            mmc_to_serial_client_putchar_buf,
            MMC_TO_SERIAL_CLIENT_PUTCHAR_CHANNEL
    );
    uart_puts("Successfully initialised UART in MMC PD.\n");

    if (sd_init() != SD_OK) {
        uart_puts("Failed to initialise SD card.\n");
    }

    uart_puts("Successfully initialised SD card.\n");
    bool is_write_success = false;
    bool is_read_success = false;
    /* Initialise the block to 0. */
    is_write_success = sd_writeblock(buf, COUNTER_SECTOR, 1);
    if (!is_write_success) {
        uart_puts("Failed to write 0 to SD card.\n");
        return;
    }
    for (int i = 0; i < 0x3; i++) {
        /* Read the block. */
        is_read_success = sd_readblock(COUNTER_SECTOR, buf, 1);
        if (!is_read_success) {
            uart_puts("Failed to read SD card.\n");
            return;
        }
        /* Increment the block. */
        (*counter)++;
        /* Write the block to disk. */
        is_write_success = sd_writeblock(buf, COUNTER_SECTOR, 1);
        if (!is_write_success) {
            uart_puts("Failed to write ");
            uart_hex(*counter);
            uart_puts(" to SD card.\n");
            return;
        }
    }
    /* Finally, read the block and print it out. */
    is_read_success = sd_readblock(COUNTER_SECTOR, buf, 1);
    if (!is_read_success) {
        uart_puts("Failed to read SD card.\n");
        return;
    }
    uart_puts("Boot counter ");
    uart_hex(*counter);
    uart_puts(" written to SD card.\n");
}

void notified(sel4cp_channel ch) {

}
