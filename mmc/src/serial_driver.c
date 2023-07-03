#include "serial_driver.h"

/* This will be automatically populated with the Virtual Address that
 * corresponds to the Physical Address of the UART device by the seL4CP tool. */
uintptr_t uart_base_vaddr;

/* Refer to `serial_client.c` for explanation. */
uintptr_t shared_dma;
/* Refer to `serial_client.c` for explanation. */
uintptr_t tx_avail_ring_buf;
/* Refer to `serial_client.c` for explanation. */
uintptr_t tx_used_ring_buf;
/* Refer to `serial_client.c` for explanation. */
uintptr_t rx_avail_ring_buf;
/* Refer to `serial_client.c` for explanation. */
uintptr_t rx_used_ring_buf;

serial_driver_t global_serial_driver = {};

/**
 * Initialises the serial driver. Called by this PD's `init` function.
 * @param serial_driver
 * @param oc4_uart_base_vaddr
 * @param auto_insert_carriage_return
 * @return
 */
static int serial_driver_init(
        serial_driver_t *serial_driver,
        uintptr_t oc4_uart_base_vaddr,
        bool auto_insert_carriage_return
);

/**
 * Sends character to the UART device.
 * @param serial_driver
 * @param ch
 */
static void serial_driver_put_char(serial_driver_t *serial_driver, int ch);

/**
 * Simple getter for `num_chars_for_client`.
 * @param serial_driver
 * @return The value of `num_chars_for_client` variable.
 */
static int serial_driver_get_num_chars_for_client(serial_driver_t *serial_driver);

/**
 * Increments `num_chars_for_client`.
 * @param serial_driver
 * @return The new value of `num_chars_for_client` or -1 for error..
 */
static int serial_driver_inc_num_chars_for_client(serial_driver_t *serial_driver);

/**
 * Decrements `num_chars_for_client`.
 * @param serial_driver
 * @return The new value of `num_chars_for_client` or -1 for error.
 */
static int serial_driver_dec_num_chars_for_client(serial_driver_t *serial_driver);

static int serial_driver_init(
        serial_driver_t *serial_driver,
        uintptr_t oc4_uart_base_vaddr,
        bool auto_insert_carriage_return
) {
    /* Initialise our `tx_ring_buf_handle`, which is just a convenience struct
     * where all relevant Transmit ring buffers are located. */
    ring_init(
            &serial_driver->tx_ring_buf_handle,
            (ring_buffer_t *) tx_avail_ring_buf,
            (ring_buffer_t *) tx_used_ring_buf,
            NULL, /* "An optional function pointer to signal the other
            component" according to README. */
            0 /* The purpose of this is to initialise read/write indices. We set
            this param to 0 since the `serial_client` PD sets this to 1. Only
            one side of the shared memory region needs to do this as per the
            README. */
    );
    /* Initialise our `rx_ring_buf_handle`, which is just a convenience struct
     * where all relevant Receive ring buffers are located. */
    ring_init(
            &serial_driver->rx_ring_buf_handle,
            (ring_buffer_t *) rx_avail_ring_buf,
            (ring_buffer_t *) rx_used_ring_buf,
            NULL, /* Refer to earlier comment about this param. */
            0 /* Refer to earlier comment about this param. */
    );
    /* Initialise the UART device. */
    bool is_success = oc4_uart_init(
            &serial_driver->oc4_uart,
            oc4_uart_base_vaddr,
            auto_insert_carriage_return
    );
    if (is_success) {
        return 0;
    }
    return -1;
}

static void serial_driver_put_char(serial_driver_t *serial_driver, int ch) {
    if (serial_driver == NULL) {
        sel4cp_dbg_puts("Illegal Argument Exception: NULL pointer in serial_driver_put_char().");
        return;
    }
    /* Keep trying to send the character to the UART device until it is successful. */
    while (oc4_uart_put_char(&serial_driver->oc4_uart, ch) < 0);
}

static int serial_driver_get_num_chars_for_client(serial_driver_t *serial_driver) {
    if (serial_driver == NULL) {
        sel4cp_dbg_puts("Illegal Argument Exception: NULL pointer in serial_driver_get_num_chars_for_client().");
        return -1;
    }
    if (serial_driver->num_chars_for_client < 0) {
        sel4cp_dbg_puts("Illegal State Exception: `num_chars_for_client` should not be below 0.");
        return -1;
    }
    return serial_driver->num_chars_for_client;
}

static int serial_driver_inc_num_chars_for_client(serial_driver_t *serial_driver) {
    if (serial_driver == NULL) {
        sel4cp_dbg_puts("Illegal Argument Exception: NULL pointer in serial_driver_inc_num_chars_for_client().");
        return -1;
    }
    /* We return the value of the pre-increment so that we return the value of
     * `num_chars_for_client` after it is updated. */
    serial_driver->num_chars_for_client++;
    return serial_driver_get_num_chars_for_client(serial_driver);
}

static int serial_driver_dec_num_chars_for_client(serial_driver_t *serial_driver) {
    if (serial_driver == NULL) {
        sel4cp_dbg_puts("Illegal Argument Exception: NULL pointer in serial_driver_dec_num_chars_for_client().");
        return -1;
    }
    serial_driver->num_chars_for_client--;
    return serial_driver_get_num_chars_for_client(serial_driver);
}

void init(void) {
    serial_driver_t *serial_driver = &global_serial_driver; /* Local reference to global serial driver for our convenience. */
    /* Initialise our `global_serial_driver` struct. */
    int ret_serial_driver_init = serial_driver_init(
            serial_driver,
            /* As explained in `serial.system`, `uart_base_vaddr` is mapped to a
             * page-aligned virtual address; however, the Mini UART device is
             * actually mapped to a virtual address that is 0x40 offset from the
             * `uart_base_vaddr` virtual address, which is why we add 0x40 to
             * `uart_base_vaddr` */
            uart_base_vaddr + 0x40,
            true
    );
    if (ret_serial_driver_init < 0) {
        sel4cp_dbg_puts("Serial Driver initialisation FAILED.\n");
        return;
    }
}

seL4_MessageInfo_t protected(sel4cp_channel ch, sel4cp_msginfo msginfo) {
    return sel4cp_msginfo_new(0, 0);
}

void notified(sel4cp_channel channel) {
    serial_driver_t *serial_driver = &global_serial_driver; /* Local reference to global serial driver for our convenience. */
    switch(channel) {
        /* This is triggered when there is a UART hardware interrupt (signifying a
         * new character was sent to the UART device). */
        case IRQ_59_CHANNEL: {
            /* We obtain the character for the UART device. */
            int c = oc4_uart_get_char(&global_serial_driver.oc4_uart);
            /* If the character is not erroneous, we send the character to the
             * UART device to output to the console. */
            if (c != -1) {
                /* This will output the character to the console. */
                serial_driver_put_char(serial_driver, c);
                /* Keep looping until we have serviced all `getchar()` requests
                 * from clients. */
                while (serial_driver_get_num_chars_for_client(serial_driver) > 0) {
                    /* Decrement `num_chars_for_client`. */
                    serial_driver_dec_num_chars_for_client(serial_driver);
                    /* The dequeued buffer's address will be stored in `buf_addr`. */
                    uintptr_t buf_addr;
                    /* The dequeued buffer's length will be stored in `buf_len`. */
                    unsigned int buf_len;
                    /* We don't use the `cookie` but the `dequeue_avail` function call requires
                     * a valid pointer for the `cookie` param, so we provide one to it anyway. */
                    void *unused_cookie;
                    /* Dequeue an available buffer from the Receive-available ring. */
                    int ret_dequeue_avail = dequeue_avail(
                            &serial_driver->rx_ring_buf_handle,
                            &buf_addr,
                            &buf_len,
                            &unused_cookie
                    );
                    if (ret_dequeue_avail < 0) {
                        sel4cp_dbg_puts("Failed to dequeue buffer from Receive available queue in notified().\n");
                        return;
                    }
                    /* Set the first byte of the buffer to the character. */
                    ((char *) buf_addr)[0] = (char) c;
                    /* TODO: Think about cache operations for non-serial driver. */
                    /* Enqueue our now-modified buffer onto the Receive-used ring. */
                    int ret_enqueue_used = enqueue_used(
                            &serial_driver->rx_ring_buf_handle,
                            buf_addr, /* This is the buffer that contains our single character. */
                            1, /* We only ever send a single character. */
                            unused_cookie
                    );
                    if (ret_enqueue_used < 0) {
                        sel4cp_dbg_puts("Received-used ring is full in notified().\n");
                        return;
                    }
                }
            }
            /* Acknowledge receipt of the interrupt. */
            sel4cp_irq_ack(channel);
            break;
        }
        /* This is triggered when `serial_client` wants to `printf` something. */
        case SERIAL_DRIVER_TO_SERIAL_CLIENT_PRINTF_CHANNEL: {
            /* The dequeued buffer's address will be stored in `buf_addr`. */
            uintptr_t buf_addr = 0;
            /* The dequeued buffer's length will be stored in `buf_len`. */
            unsigned int buf_len = 0;
            /* We don't use the `cookie` but the `driver_dequeue` function call requires
             * a valid pointer for the `cookie` param, so we provide one to it anyway. */
            void *unused_cookie = NULL;
            /* The while-loop continues to de-queue words until there are no
             * words left in the Transmit-Used queue. In our current setup (no
             * scheduling budget or period set), this while-loop is unnecessary
             * since this PD will never be preempted, which means there will
             * never be more than 1 buffer in the Transmit-Used queue. However,
             * this while-loop becomes necessary if we eventually ascribe a
             * scheduling budget and period to this PD (which we do for the
             * ethernet driver), which would open the possibility for this PD to
             * be preempted before we've finished processing the buffer in the
             * Transmit-Used queue. As such, by the time this PD is rescheduled,
             * the lower-priority `serial_client` PD may have added a new buffer
             * to the Transmit-Used ring, which we may as well just dequeue and
             * process once we are restarted by the kernel. */
            while (driver_dequeue(
                    serial_driver->tx_ring_buf_handle.used_ring,
                    &buf_addr,
                    &buf_len,
                    &unused_cookie
            ) == 0) {
                int curr_idx = 0;
                /* Keep looping and printing out each character of each word
                 * until you hit a NULL terminator. */
                while (((char *) buf_addr)[curr_idx] != '\0') {
                    /* Write the character to the serial device. */
                    serial_driver_put_char(
                            serial_driver,
                            ((char *) buf_addr)[curr_idx] /* Obtain the relevant
                            character for the current word in `buf_addr` */
                    );
                    curr_idx++;
                }
                /* Note, we deliberately do NOT print out the NULL terminator. */
                /* Return buffer back to the transmit-avail queue for the
                 * `serial_client` PD to use. */
                int ret_enqueue_avail = enqueue_avail(
                        &serial_driver->tx_ring_buf_handle,
                        buf_addr,
                        BUF_SIZE, /* TODO: Unit test what happens if you give back a buffer that is smaller than the original. */
                        NULL
                );
                if (ret_enqueue_avail < 0) {
                    sel4cp_dbg_puts("Failed to enqueue buffer onto available queue in notified().\n");
                    return;
                }
            }
            return;
        }
        /* This is triggered when the `serial_client` PD calls `getchar()`. */
        case SERIAL_DRIVER_TO_SERIAL_CLIENT_GETCHAR_CHANNEL: {
            /* Every time this PD receives a `getchar()` request, we increment
             * the `num_chars_for_client` variable by 1. When
             * `num_chars_for_client` is greater than 0, each character the UART
             * device receives is sent back to the `serial_client` PD until
             * there are no more characters to send back. The logic for sending
             * back a character to the `serial_client` PD is located in the IRQ
             * handler branch of this switch-statement. On the client's side,
             * the `serial_client` PD should block until a character is sent
             * back from this PD, which is the expected behaviour of
             * `getchar()`. */
            serial_driver_inc_num_chars_for_client(serial_driver);
            break;
        }
        default:
            sel4cp_dbg_puts("Serial driver: received notification on unexpected channel\n");
            break;
    }
}
