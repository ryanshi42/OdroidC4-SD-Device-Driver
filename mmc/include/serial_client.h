#pragma once


#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "shared_ringbuffer.h"
#include "shared_dma.h"
#include "sel4cp_facade.h"

#define SERIAL_CLIENT_TO_SERIAL_DRIVER_PRINTF_CHANNEL (3)
#define SERIAL_CLIENT_TO_SERIAL_DRIVER_GETCHAR_CHANNEL (5)
#define SERIAL_CLIENT_TO_MMC_PUTCHAR_CHANNEL (8)
#define SERIAL_CLIENT_TO_TIMER_DRIVER_PUTCHAR_CHANNEL (10)

typedef struct serial_client serial_client_t;
struct serial_client {
    /* Transmit ring buffer handle. This is a convenience `struct` that contains
     * all the pointers to the relevant Transmit "available" and "used" buffers.
     * */
    ring_handle_t tx_ring_buf_handle;
    /* Receive ring buffer handle. This is a convenience `struct` that contains
     * all the pointers to the relevant Receive "available" and "used" buffers. */
    ring_handle_t rx_ring_buf_handle;
};


