#include "oc4_uart.h"


bool oc4_uart_init(
        oc4_uart_t *oc4_uart,
        uintptr_t base_vaddr,
        bool auto_insert_carriage_return
) {
    if (oc4_uart == NULL) {
        return false;
    }
    /* Save pointer to the registers. */
    oc4_uart->regs = oc4_uart_regs_get(base_vaddr);
    /* Save user's auto carriage return preference. */
    oc4_uart->auto_insert_carriage_return = auto_insert_carriage_return;
    /* Disable RX interrupts */
    oc4_uart_regs_disable_rx_irq(oc4_uart->regs);
    /* Disable RX and TX */
    oc4_uart_regs_disable(oc4_uart->regs);
    /* TODO: line configuration? */
    /* Enable RX and TX */
    oc4_uart_regs_enable(oc4_uart->regs);
    /* Enable receive interrupts */
    oc4_uart_regs_enable_rx_irq(oc4_uart->regs);
    /* Return true for successful initialisation. */
    return true;
}


int oc4_uart_put_char(
        oc4_uart_t *oc4_uart,
        int c
) {
    oc4_uart_regs_t *regs = oc4_uart->regs;

    if (oc4_uart_regs_is_tx_fifo_busy(regs)) {
        return -1;
    }
    /* If `auto_insert_carriage_return` is enabled, we first set the `\r`
     * character and then set the `\n` character. */
    if (c == '\n' && oc4_uart->auto_insert_carriage_return) {
        /* Write carriage return first */
        oc4_uart_regs_tx_char(regs, '\r');
        /* if we transform a '\n' (LF) into '\r\n' (CR+LF) this shall become an
         * atom, ie we don't want CR to be sent and then fail at sending LF
         * because the TX FIFO is full. Basically there are two options:
         *   - check if the FIFO can hold CR+LF and either send both or none
         *   - send CR, then block until the FIFO has space and send LF.
         * Assuming that if SERIAL_AUTO_CR is set, it's likely this is a serial
         * console for logging, so blocking seems acceptable in this special
         * case.
         */
        while (oc4_uart_regs_is_tx_fifo_busy(regs)) {
            /* busy loop */
        }
    }

    oc4_uart_regs_tx_char(regs, c);
    return c;
}

int oc4_uart_get_char(
        oc4_uart_t *oc4_uart
) {
    oc4_uart_regs_t *regs = oc4_uart->regs;
    int c = -1;

    if (oc4_uart_regs_is_rx_ready(regs)) {
        c = oc4_uart_regs_rx_char(regs);
    }

    return c;
}



