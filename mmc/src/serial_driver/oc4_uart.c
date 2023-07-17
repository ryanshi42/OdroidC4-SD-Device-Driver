#include "oc4_uart.h"
#include <stdint.h>
#include <stdbool.h>

bool oc4_uart_init(
        oc4_uart_t *oc4_uart,
        uintptr_t base_vaddr,
        bool auto_insert_carriage_return
) {
    if (oc4_uart == NULL) {
        return false;
    }
    /* Save pointer to the registers. */
    oc4_uart->regs = (oc4_uart_regs_t *) base_vaddr;
    // /* Save user's auto carriage return preference. */
    // oc4_uart->auto_insert_carriage_return = auto_insert_carriage_return;
    // /* Disable RX interrupts */
    // oc4_uart_regs_disable_rx_irq(oc4_uart->regs);
    // /* Disable RX and TX */
    // oc4_uart_regs_disable(oc4_uart->regs);
    // /* TODO: line configuration? */
    // /* Enable RX and TX */
    // oc4_uart_regs_enable(oc4_uart->regs);
    // /* Enable receive interrupts */
    // oc4_uart_regs_enable_rx_irq(oc4_uart->regs);
    /* Return true for successful initialisation. */
    return true;
}


int oc4_uart_put_char(
        oc4_uart_t *oc4_uart,
        int c
) {
    oc4_uart_regs_t *regs = oc4_uart->regs;

    /* Add character to the buffer. */
    regs->wfifo = c & 0x7f;
    if (c == '\n') {
        oc4_uart_put_char(oc4_uart, '\r');
    }

    return 0;

    // if (oc4_uart_regs_is_tx_fifo_busy(regs)) {
    //     return -1;
    // }
    // /* If `auto_insert_carriage_return` is enabled, we first set the `\r`
    //  * character and then set the `\n` character. */
    // if (c == '\n' && oc4_uart->auto_insert_carriage_return) {
    //     /* Write carriage return first */
    //     oc4_uart_regs_tx_char(regs, '\r');
    //     /* if we transform a '\n' (LF) into '\r\n' (CR+LF) this shall become an
    //      * atom, ie we don't want CR to be sent and then fail at sending LF
    //      * because the TX FIFO is full. Basically there are two options:
    //      *   - check if the FIFO can hold CR+LF and either send both or none
    //      *   - send CR, then block until the FIFO has space and send LF.
    //      * Assuming that if SERIAL_AUTO_CR is set, it's likely this is a serial
    //      * console for logging, so blocking seems acceptable in this special
    //      * case.
    //      */
    //     while (oc4_uart_regs_is_tx_fifo_busy(regs)) {
    //         /* busy loop */
    //     }
    // }

    // oc4_uart_regs_tx_char(regs, c);
    // return c;
}

int oc4_uart_get_char(
        oc4_uart_t *oc4_uart
) {
    oc4_uart_regs_t *regs = oc4_uart->regs;
    // int c = -1;

    // if (oc4_uart_regs_is_rx_ready(regs)) {
    //     c = oc4_uart_regs_rx_char(regs);
    // }
    // oc4_uart_regs_t *regs = (oc4_uart_regs_t *) uart_base;

    while (regs->sr & AML_UART_RX_EMPTY);
    return regs->rfifo;

    // return c;
}

static int oc4_uart_regs_is_tx_fifo_busy(
    oc4_uart_regs_t *regs)
{
    /* check the TXFE (transmit buffer FIFO empty) flag, which is cleared
     * automatically when data is written to the TxFIFO. Even though the flag
     * is set, the actual data transmission via the UART's 32 byte FIFO buffer
     * might still be in progress.
     */
    return (0 == (regs->sr & AML_UART_TX_EMPTY));
}

// int serial_configure(
//     long bps,
//     int char_size,
//     enum serial_parity parity,
//     int stop_bits,
//     oc4_uart_regs_t *uart_base
// )
// {
//     oc4_uart_regs_t *regs = (oc4_uart_regs_t *) uart_base;
    
//     uint32_t cr;
//     /* Character size */
//     cr = regs->cr;
//     if (char_size == 8) {
//         cr |= AML_UART_DATA_LEN_8BIT;
//     } else if (char_size == 7) {
//         cr |= AML_UART_DATA_LEN_7BIT;
//     } else {
//         return -1;
//     }
//     /* Stop bits */
//     if (stop_bits == 2) {
//         cr |= AML_UART_STOP_BIT_2SB;
//     } else if (stop_bits == 1) {
//         cr |= AML_UART_STOP_BIT_1SB;
//     } else {
//         return -1;
//     }

//     /* Parity */
//     if (parity == PARITY_NONE) {
//         cr &= ~AML_UART_PARITY_EN;
//     } else if (parity == PARITY_ODD) {
//         /* ODD */
//         cr |= AML_UART_PARITY_EN;
//         cr |= AML_UART_PARITY_TYPE;
//     } else if (parity == PARITY_EVEN) {
//         /* Even */
//         cr |= AML_UART_PARITY_EN;
//         cr &= ~AML_UART_PARITY_TYPE;
//     } else {
//         return -1;
//     }
//     /* Apply the changes */
//     regs->cr = cr;
//     sel4cp_dbg_puts("finished configuring the line, setting the baud rate\n");
//     /* Now set the baud rate */
//     // imx_uart_set_baud(bps);
//     return 0;
// }

// // Called from handle tx, write each character stored in the buffer to the serial port
// static void
// raw_tx(char *phys, unsigned int len, void *cookie)
// {
//     sel4cp_dbg_puts("entering raw tx function\n");
//     // This is byte by byte for now, switch to DMA use later
//     for (int i = 0; i < len || phys[i] != '\0'; i++) {
//         // Loop until the fifo queue is ready to transmit
//         while (putchar(phys[i]) != 0);
//     }
// }

// void handle_tx() {
//     sel4cp_dbg_puts("In the handle tx func\n");
//     uintptr_t buffer = 0;
//     unsigned int len = 0;
//     void *cookie = 0;
//     // Dequeue something from the Tx ring -> the server will have placed something in here, if its empty then nothing to do
//     sel4cp_dbg_puts("Dequeuing and printing everything currently in the ring buffer\n");
//     while (!driver_dequeue(tx_ring.used_ring, &buffer, &len, &cookie)) {
//         sel4cp_dbg_puts("in the driver dequeue loop\n");
//         // Buffer cointaining the bytes to write to serial
//         char *phys = (char * )buffer;
//         // Handle the tx
//         raw_tx(phys, len, cookie);
//         // Then enqueue this buffer back into the available queue, so that it can be collected and reused by the server
//         enqueue_avail(&tx_ring, buffer, len, &cookie);
//     }
//     sel4cp_dbg_puts("Finished handle_tx\n");
// }
