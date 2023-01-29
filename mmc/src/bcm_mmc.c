#include "bcm_mmc.h"


bool bcm_uart_init(
        bcm_uart_t *bcm_uart,
        uintptr_t base_vaddr,
        bool auto_insert_carriage_return
) {
    if (bcm_uart == NULL) {
        return false;
    }
    /* Save pointer to the registers. */
    bcm_uart->regs = bcm_uart_regs_get(base_vaddr);
    /* Save user's auto carriage return preference. */
    bcm_uart->auto_insert_carriage_return = auto_insert_carriage_return;
    /* Disable RX interrupts */
    bcm_uart_regs_disable_rx_irq(bcm_uart->regs);
    /* Disable RX and TX */
    bcm_uart_regs_disable(bcm_uart->regs);
    /* TODO: line configuration? */
    /* Enable RX and TX */
    bcm_uart_regs_enable(bcm_uart->regs);
    /* Enable receive interrupts */
    bcm_uart_regs_enable_rx_irq(bcm_uart->regs);
    /* Return true for successful initialisation. */
    return true;
}


int bcm_uart_put_char(
        bcm_uart_t *bcm_uart,
        int c
) {
    bcm_uart_regs_t *regs = bcm_uart->regs;

    if (bcm_uart_regs_is_tx_fifo_busy(regs)) {
        return -1;
    }
    /* If `auto_insert_carriage_return` is enabled, we first set the `\r`
     * character and then set the `\n` character. */
    if (c == '\n' && bcm_uart->auto_insert_carriage_return) {
        /* Write carriage return first */
        bcm_uart_regs_tx_char(regs, '\r');
        /* if we transform a '\n' (LF) into '\r\n' (CR+LF) this shall become an
         * atom, ie we don't want CR to be sent and then fail at sending LF
         * because the TX FIFO is full. Basically there are two options:
         *   - check if the FIFO can hold CR+LF and either send both or none
         *   - send CR, then block until the FIFO has space and send LF.
         * Assuming that if SERIAL_AUTO_CR is set, it's likely this is a serial
         * console for logging, so blocking seems acceptable in this special
         * case.
         */
        while (bcm_uart_regs_is_tx_fifo_busy(regs)) {
            /* busy loop */
        }
    }

    bcm_uart_regs_tx_char(regs, c);
    return c;
}

int bcm_uart_get_char(
        bcm_uart_t *bcm_uart
) {
    bcm_uart_regs_t *regs = bcm_uart->regs;
    int c = -1;

    if (bcm_uart_regs_is_rx_ready(regs)) {
        c = bcm_uart_regs_rx_char(regs);
    }

    return c;
}



