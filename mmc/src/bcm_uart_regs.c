#include "bcm_uart_regs.h"

bcm_uart_regs_t *bcm_uart_regs_get(uintptr_t base_vaddr) {
    return (bcm_uart_regs_t *) base_vaddr;
}

bool bcm_uart_regs_is_tx_fifo_busy(
        bcm_uart_regs_t *regs
) {
    return !(regs->mu_lsr & MU_LSR_TXEMPTY);
}

void bcm_uart_regs_disable_rx_irq(
        bcm_uart_regs_t *regs
) {
    regs->mu_ier &= ~MU_IER_ENA_RX_IRQ;
}

void bcm_uart_regs_disable(
        bcm_uart_regs_t *regs
) {
    regs->mu_cntl &= ~(MU_CNTL_RXE | MU_CNTL_TXE);
}

void bcm_uart_regs_enable(
        bcm_uart_regs_t *regs
) {
    regs->mu_cntl |= (MU_CNTL_RXE | MU_CNTL_TXE);
}

void bcm_uart_regs_enable_rx_irq(
        bcm_uart_regs_t *regs
) {
    regs->mu_ier |= MU_IER_ENA_RX_IRQ;
}

void bcm_uart_regs_tx_char(
        bcm_uart_regs_t *regs,
        int ch
) {
    regs->mu_io = (ch & MASK_UNSAFE(8));
}

bool bcm_uart_regs_is_rx_ready(
        bcm_uart_regs_t *regs
) {
    return regs->mu_lsr & MU_LSR_DATAREADY;
}

int bcm_uart_regs_rx_char(
        bcm_uart_regs_t *regs
) {
    return (int) (regs->mu_io & MASK_UNSAFE(8));
}
