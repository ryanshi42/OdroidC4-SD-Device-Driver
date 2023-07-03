#include "oc4_uart_regs.h"

oc4_uart_regs_t *oc4_uart_regs_get(uintptr_t base_vaddr) {
    return (oc4_uart_regs_t *) base_vaddr;
}

bool oc4_uart_regs_is_tx_fifo_busy(
        oc4_uart_regs_t *regs
) {
    return !(regs->mu_lsr & MU_LSR_TXEMPTY);
}

void oc4_uart_regs_disable_rx_irq(
        oc4_uart_regs_t *regs
) {
    regs->mu_ier &= ~MU_IER_ENA_RX_IRQ;
}

void oc4_uart_regs_disable(
        oc4_uart_regs_t *regs
) {
    regs->mu_cntl &= ~(MU_CNTL_RXE | MU_CNTL_TXE);
}

void oc4_uart_regs_enable(
        oc4_uart_regs_t *regs
) {
    regs->mu_cntl |= (MU_CNTL_RXE | MU_CNTL_TXE);
}

void oc4_uart_regs_enable_rx_irq(
        oc4_uart_regs_t *regs
) {
    regs->mu_ier |= MU_IER_ENA_RX_IRQ;
}

void oc4_uart_regs_tx_char(
        oc4_uart_regs_t *regs,
        int ch
) {
    regs->mu_io = (ch & MASK_UNSAFE(8));
}

bool oc4_uart_regs_is_rx_ready(
        oc4_uart_regs_t *regs
) {
    return regs->mu_lsr & MU_LSR_DATAREADY;
}

int oc4_uart_regs_rx_char(
        oc4_uart_regs_t *regs
) {
    return (int) (regs->mu_io & MASK_UNSAFE(8));
}
