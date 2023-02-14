#pragma once

#include <stdlib.h>
#include <stdint.h>
#include <sel4cp.h>

typedef struct uart uart_t;
struct uart {
    /* The shared buffer to send characters to `serial_client.c`. */
    char *shared_putchar_buf;
    /* Channel ID for `putchar`. */
    size_t sel4cp_channel_id_putchar;
};

//void uart_init();
void uart_init(
        uintptr_t shared_putchar_buf,
        size_t sel4cp_channel_id_putchar
);
void uart_send(unsigned int c);
char uart_getc();
void uart_puts(char *s);
void uart_hex(unsigned int d);
void uart_dump(void *ptr);
