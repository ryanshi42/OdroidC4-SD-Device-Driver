#pragma once

#include <stdbool.h>

#include "oc4_uart_regs.h"

typedef struct oc4_uart oc4_uart_t;
struct oc4_uart {
    volatile oc4_uart_regs_t *regs;
    /* Will be `true` if user wants `\n` chars to be automatically converted to
     * `\r\n` and will be `false` otherwise. */
    bool auto_insert_carriage_return;
};

/**
 * Initialises the IMX Uart device.
 * @param oc4_uart
 * @param base_vaddr
 * @param auto_insert_carriage_return Set to `true` if you want `\n` chars to be
 * automatically converted to `\r\n` and set to `false` otherwise.
 * @return
 */
bool oc4_uart_init(
        oc4_uart_t *oc4_uart,
        uintptr_t base_vaddr,
        bool auto_insert_carriage_return
);

/**
 * Prints out a character using the imx uart device.
 * @param oc4_uart
 * @param c
 * @return
 */
int oc4_uart_put_char(
        oc4_uart_t *oc4_uart,
        int c
);

/**
 * Gets character from UART device.
 * @param oc4_uart
 * @return
 */
int oc4_uart_get_char(
        oc4_uart_t *oc4_uart
);
