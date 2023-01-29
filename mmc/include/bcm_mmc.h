#pragma once

#include <stdbool.h>

#include "bcm_mmc_regs.h"

typedef struct bcm_uart bcm_uart_t;
volatile struct bcm_uart {
    volatile bcm_uart_regs_t *regs;
    /* Will be `true` if user wants `\n` chars to be automatically converted to
     * `\r\n` and will be `false` otherwise. */
    bool auto_insert_carriage_return;
};

/**
 * Initialises the IMX Uart device.
 * @param bcm_uart
 * @param base_vaddr
 * @param auto_insert_carriage_return Set to `true` if you want `\n` chars to be
 * automatically converted to `\r\n` and set to `false` otherwise.
 * @return
 */
bool bcm_uart_init(
        bcm_uart_t *bcm_uart,
        uintptr_t base_vaddr,
        bool auto_insert_carriage_return
);

/**
 * Prints out a character using the imx uart device.
 * @param bcm_uart
 * @param c
 * @return
 */
int bcm_uart_put_char(
        bcm_uart_t *bcm_uart,
        int c
);

/**
 * Gets character from UART device.
 * @param bcm_uart
 * @return
 */
int bcm_uart_get_char(
        bcm_uart_t *bcm_uart
);
