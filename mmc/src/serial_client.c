
#include "serial_client.h"

/* In the SDDF's design-documentation, `shared_dma` is referred to as the "Data
 * Region". `shared_dma` is an array of I/O buffers that will be used to
 * pass/transport data from `serial_client` to `serial_driver` and vice-versa.
 * */
uintptr_t shared_dma;

/* `tx_` stands for "Transmit". The following two Transmit buffers are to assist
 * with transporting data from `serial_client` to `serial_driver`. */

/* In the SDDF's design-documentation, `tx_avail_ring_buf` is referred to as the
 * "Transmit-Free" (TxF) ring buffer. `tx_avail_ring_buf` holds all the buffers
 * in the `shared_dma` data region that are ready to be reused for transporting
 * new data from `serial_client` to `serial_driver`. */
uintptr_t tx_avail_ring_buf;
/* In the SDDF's design-documentation, `tx_used_ring_buf` is referred to as the
 * "Transmit-Available" (TxA) ring buffer. `tx_used_ring_buf` holds all the
 * buffers in the `shared_dma` data region that currently hold data sent by
 * `serial_client` for the `serial_driver` PD to process.  */
uintptr_t tx_used_ring_buf;

/* `rx_` stands for "Receive". The following two Receive buffers are to assist
 * with transporting data from `serial_driver` to `serial_client`. */

/* Identical purpose to `tx_avail_ring_buf` but for `serial_driver` to
 * `serial_client`. */
uintptr_t rx_avail_ring_buf;
/* Identical purpose to `tx_used_ring_buf` but for `serial_driver` to
 * `serial_client`. */
uintptr_t rx_used_ring_buf;

/* Global serial client. */
serial_client_t global_serial_client = {0};

/* Buffer for receiving characters from the `mmc` Protection Domain. */
uintptr_t mmc_to_serial_client_putchar_buf;

/* Used for transporting chars from `timer_driver` to `serial_client`. */
uintptr_t timer_driver_to_serial_client_putchar_buf;

/**
 * Initialises the `serial_client` struct and sets up the serial client.
 * @param serial_client
 * @return 0 for success and -1 for error.
 */
static int serial_client_init(
        serial_client_t *serial_client
);

/**
 * Prints a string.
 * @param str
 * @return -1 for error, otherwise returns the number of characters printed to the terminal.
 */
static int serial_client_printf(char *str);

/**
 * Gets a character from the serial driver.
 * @param serial_client
 * @return -1 for error, otherwise returns the character obtained from the serial driver.
 */
static int serial_client_getchar(serial_client_t *serial_client);

/**
 * Notifies the `serial_driver` PD that there is a `printf()` call.
 */
static void serial_client_notify_serial_driver_printf();

/**
 * Notifies the `serial_driver` PD that there is a `getchar()` call.
 */
static void serial_client_notify_serial_driver_getchar();

static int serial_client_init(
        serial_client_t *serial_client
) {
    /* Initialise our `tx_ring_buf_handle`, which is just a convenience struct
     * where all relevant Transmit ring buffers are located. */
    ring_init(
            &serial_client->tx_ring_buf_handle,
            (ring_buffer_t *) tx_avail_ring_buf,
            (ring_buffer_t *) tx_used_ring_buf,
            NULL,
            1
    );
    /* Populate the available ring buffer with empty buffers from `shared_dma`
     * memory region. */
    for (int i = 0; i < NUM_BUFFERS - 1; i++) {
        int ret_enqueue_avail = enqueue_avail(
                &serial_client->tx_ring_buf_handle,
                shared_dma + (BUF_SIZE * i),
                BUF_SIZE,
                NULL
        );
        if (ret_enqueue_avail < 0) {
            sel4cp_dbg_puts("Failed to enqueue buffer onto Transmit available queue in serial_client_init().\n");
            return -1;
        }
    }
    /* Initialise our `rx_ring_buf_handle`, which is just a convenience struct
     * where all relevant Receive ring buffers are located. */
    ring_init(
            &serial_client->rx_ring_buf_handle,
            (ring_buffer_t *) rx_avail_ring_buf,
            (ring_buffer_t *) rx_used_ring_buf,
            NULL,
            1
    );
    /* Populate the available ring buffer with empty buffers from `shared_dma`
     * memory region. */
    for (int i = 0; i < NUM_BUFFERS - 1; i++) {
        int ret_enqueue_avail = enqueue_avail(
                &serial_client->rx_ring_buf_handle, /* Enqueuing Receive buffers
                onto the Receive buffer handle. */
                shared_dma + (BUF_SIZE * (i + NUM_BUFFERS)), /* Note that we are
                using the buffers in the data region located after the ones we
                used for the Transmit side earlier. */
                BUF_SIZE,
                NULL
        );
        if (ret_enqueue_avail < 0) {
            sel4cp_dbg_puts("Failed to enqueue buffer onto Receive available queue in serial_client_init().\n");
            return -1;
        }
    }
    return 0;
}

static int serial_client_printf(char *str) {
    /* Local reference to global serial_client for convenience. */
    serial_client_t *serial_client = &global_serial_client;
    /* The dequeued buffer's address will be stored in `buf_addr`. */
    uintptr_t buf_addr;
    /* The dequeued buffer's length will be stored in `buf_len`. */
    unsigned int buf_len;
    /* We don't use the `cookie` but the `dequeue_avail` function call requires
     * a valid pointer for the `cookie` param, so we provide one to it anyway. */
    void *unused_cookie;
    /* Dequeue an available buffer. */
    int ret_dequeue_avail = dequeue_avail(
            &serial_client->tx_ring_buf_handle,
            &buf_addr,
            &buf_len,
            &unused_cookie
    );
    if (ret_dequeue_avail < 0) {
        sel4cp_dbg_puts("Failed to dequeue buffer from available queue in serial_client_printf().\n");
        return -1;
    }
    /* Length of string including the NULL terminator. */
    size_t str_len = strlen(str) + 1;
    if (str_len > BUF_SIZE) {
        /* TODO: Allocate multiple buffers for strings larger than BUF_SIZE. */
        return -1;
    }
    /* Copy the string (including the NULL terminator) into
     * `buf_addr` to update our dequeued available buffer. */
    memcpy(
            (char *) buf_addr,
            str,
            str_len /* We define the length of a string as inclusive of its NULL
            terminator. */
    );
    /* Since we have just written fresh data to the `shared_dma` memory region,
     * we need to clean the cache, which will force the contents of the cache to
     * be written back to physical memory. This ensures subsequent reads of the
     * `shared_dma` memory region will contain the latest data we have just
     * written. However, for our serial driver, this cache operation is
     * technically unnecessary because we are simply setting the device register
     * to the value of the character to be printed out. This cache operation
     * would be necessary for other device drivers (e.g. block) that set device
     * registers to an address that points to physical memory. */
    int ret_vspace_clean = seL4_ARM_VSpace_Clean_Data(
            3, /* The capability to every PD's VSpace. */
            buf_addr,
            buf_addr + str_len
    );
    if (ret_vspace_clean) {
        sel4cp_dbg_puts("Failed to clean cache in serial_client_printf().\n");
        return -1;
    }
    /* If the Transmit-Used ring was empty prior to us enqueuing our new used
     * buffer to the Transmit-Used ring (done in the following step), then we
     * know the `serial_driver` PD was NOT preempted while working on the
     * Transmit-Used ring. Therefore, after we enqueue our buffer to the
     * Transmit-Used buffer, we need to notify the `serial_driver` PD.
     * Conversely, if the Transmit-Used ring was NOT empty, then we know the
     * `serial_driver` PD was preempted while doing work on the Transmit-Used
     * ring. Therefore, we can save the kernel an unnecessary system call by NOT
     * notifying the `serial_driver` PD since the kernel will resume the
     * `serial_driver` PD (e.g. once its budget is refilled). This optimisation
     * is handled by the if(tx_used_was_empty) check later in this function. */
    bool tx_used_was_empty = ring_empty(serial_client->tx_ring_buf_handle.used_ring);
    /* Enqueue our dequeued (and now updated) available buffer into the used
     * transmit queue. Now the `serial_driver` PD will be able to access our
     * buffer by de-queuing from this used transmit queue. */
    int ret_enqueue_used = enqueue_used(
            &serial_client->tx_ring_buf_handle,
            buf_addr,
            str_len,
            unused_cookie
    );
    if (ret_enqueue_used < 0) {
        sel4cp_dbg_puts("Transmit used ring is full in serial_client_printf().\n");
        return -1;
    }
    /* As explained above, we only notify the `serial_driver` PD if the
     * Transmit-Used buffer was empty, which we know is when the PD was NOT
     * doing `printf` work for us. In our current setup (with no scheduling
     * budget or period set), this optimisation is unnecessary because our PD
     * has a lower priority than the `serial_driver` PD, meaning the
     * `serial_driver` PD can never be preempted i.e. the `serial_driver` PD
     * will always clear all the buffers in the Transmit-Used ring before we are
     * allowed to run. However, this optimisation becomes necessary if we
     * eventually decide to ascribe a scheduling "budget" and "period" to the
     * higher-priority `serial_driver` PD (which is the case for the ethernet
     * driver), which would allow the `serial_driver` PD to be preempted by us
     * (leading to several buffers left in the Transmit-Used buffer for the
     * `serial_driver` PD to resume processing once its scheduling budget is
     * refilled at some later time). */
    if (tx_used_was_empty) {
        /* Notify the `serial_driver`. */
        serial_client_notify_serial_driver_printf();
    }
    return (int) strlen(str); /* Return the length of the string excluding the NULL terminator. */
}

static int serial_client_getchar(serial_client_t *serial_client) {
    /* Notify the `serial_driver` PD that we wish to obtain a character. */
    serial_client_notify_serial_driver_getchar();
    /* The dequeued buffer's address will be stored in `buf_addr`. */
    uintptr_t buf_addr = 0;
    /* The dequeued buffer's length will be stored in `buf_len`. */
    unsigned int buf_len = 0;
    /* We don't use the `cookie` but the `dequeue_used` function call requires
     * a valid pointer for the `cookie` param, so we provide one to it anyway. */
    void *unused_cookie = NULL;
    /* This while-loop blocks until we're able to obtain a character from the
     * Receive-Used ring. */
    while (dequeue_used(
            &serial_client->rx_ring_buf_handle,
            &buf_addr,
            &buf_len,
            &unused_cookie
    ) != 0) {
        /* Keep spinning while there is nothing in the Receive-Used ring. There
         * will be nothing in the Receive-Used ring if the user has yet to enter
         * a character into the console. Blocking here while there is no user
         * input is the expected behaviour of `getchar`. */
        sel4cp_dbg_puts(""); /* This seemingly useless print statement is
        required in order to prevent the C compiler from optimising this entire
        while-loop out (which it will do if there was nothing in the body of
        this while-loop). When we aren't in debug mode, this will be compiled
        into a noop, which we're fine with. */
    }
    /* We only reach this point when there is a successful dequeue from the
     * Receive-Used ring, which means `buf_addr` and `buf_len` are populated
     * with valid values. */
    if (buf_len != 1) {
        sel4cp_dbg_puts("Illegal State Exception: The buffer should only contain a single character and therefore should have length 1.");
        return -1;
    }
    /* We save the character into a temporary variable. */
    char ch = *((char *) buf_addr);
    /* Now we can enqueue our free buffer onto the Receive-Available queue to be
     * used by another `getchar()` call. */
    int ret_enqueue_avail = enqueue_avail(
            &serial_client->rx_ring_buf_handle,
            buf_addr,
            BUF_SIZE,
            NULL
    );
    if (ret_enqueue_avail < 0) {
        sel4cp_dbg_puts("Failed to enqueue buffer onto Receive available queue in serial_client_getchar().\n");
        return -1;
    }
    /* Return the obtained character to the caller */
    return (int) ch;
}

static void serial_client_notify_serial_driver_printf() {
    sel4cp_notify(SERIAL_CLIENT_TO_SERIAL_DRIVER_PRINTF_CHANNEL);
}

static void serial_client_notify_serial_driver_getchar() {
    sel4cp_notify(SERIAL_CLIENT_TO_SERIAL_DRIVER_GETCHAR_CHANNEL);
}

void init(void) {
    /* Local reference to global serial_client for convenience. */
    serial_client_t *serial_client = &global_serial_client;
    /* Initialise serial_client. */
    int ret_serial_client_init = serial_client_init(serial_client);
    if (ret_serial_client_init < 0) {
        sel4cp_dbg_puts("Serial Client initialisation FAILED.\n");
        return;
    }
    /* Basic E2E tests for printf. */
    serial_client_printf("\n=== START ===\n");
    serial_client_printf("Initialising UART device...\n");
    serial_client_printf("UART device initialisation SUCCESS.\n");
    serial_client_printf("\n");
    for (int i = 0; i < 5; i++) {
        serial_client_printf("a");
        serial_client_printf("b");
        serial_client_printf("c");
    }
    serial_client_printf("\n");
    serial_client_printf("Ending UART test...\n");
    serial_client_printf("=== END ===\n");

//    for (int i = 0; i < 2; i++) {
//        char ch_as_str[2] = {0};
//        ch_as_str[0] = (char) serial_client_getchar(serial_client);
//        serial_client_printf("Serial Client received the character: ");
//        serial_client_printf(ch_as_str);
//        serial_client_printf("\n");
//    }
}

seL4_MessageInfo_t protected(sel4cp_channel ch, sel4cp_msginfo msginfo) {
    return sel4cp_msginfo_new(0, 0);
}

void notified(sel4cp_channel channel) {
    serial_client_t *serial_client = &global_serial_client; /* Local reference to global serial driver for our convenience. */
    (void) serial_client; /* Suppress unused variable warning. */
    switch(channel) {
        /* If MMC has asked the serial client to `putchar`, then print out a character. */
        case SERIAL_CLIENT_TO_TIMER_DRIVER_PUTCHAR_CHANNEL: {
            /* Create a temporary zero-ed string to be passed into `serial_client_printf()`. */
            char str_tmp[2] = {0};
            /* Obtain the character to print from the `mmc_to_serial_client_putchar_buf`. */
            str_tmp[0] = ((char *) timer_driver_to_serial_client_putchar_buf)[0];
            /* Print out the character. */
            serial_client_printf(str_tmp);
            break;
        }
        case SERIAL_CLIENT_TO_MMC_PUTCHAR_CHANNEL: {
            /* Create a temporary zero-ed string to be passed into `serial_client_printf()`. */
            char str_tmp[2] = {0};
            /* Obtain the character to print from the `mmc_to_serial_client_putchar_buf`. */
            str_tmp[0] = ((char *) mmc_to_serial_client_putchar_buf)[0];
            /* Print out the character. */
            serial_client_printf(str_tmp);
            break;
        }
        default:
            sel4cp_dbg_puts("Serial client: received notification on unexpected channel\n");
            break;
    }
}
