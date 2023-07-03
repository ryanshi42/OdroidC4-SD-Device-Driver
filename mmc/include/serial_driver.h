#pragma once

#include <stdbool.h>

#include "oc4_uart.h"
#include "shared_ringbuffer.h"
#include "shared_dma.h"
#include "sel4cp_facade.h"

#define IRQ_59_CHANNEL (2)

#define SERIAL_DRIVER_TO_SERIAL_CLIENT_PRINTF_CHANNEL (4)
#define SERIAL_DRIVER_TO_SERIAL_CLIENT_GETCHAR_CHANNEL (6)

typedef struct serial_driver serial_driver_t;
struct serial_driver {
    /* UART device. */
    oc4_uart_t oc4_uart;
    /* Transaction ring buffer handle. This is a convenience `struct` that
     * contains all the pointers to the relevant Transmit "available" and "used"
     * buffers. */
    ring_handle_t tx_ring_buf_handle;
    /* Receive ring buffer handle. This is a convenience `struct` that
     * contains all the pointers to the relevant Receive "available" and "used"
     * buffers. */
    ring_handle_t rx_ring_buf_handle;
    /* This represents the number of characters we must return to the client.
     * This variable is initialised to 0. Each time the client calls
     * `getchar()`, this value is incremented by 1. We keep track of the number
     * of characters we need to retrieve for the client because we might get
     * preempted by the client PD, which may... */
    int num_chars_for_client;
};
