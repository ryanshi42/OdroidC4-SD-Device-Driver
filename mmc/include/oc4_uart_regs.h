#pragma once

//! None of these functions are necessary for the OC4

// #include <stdlib.h>
// #include <stdint.h>
// #include <stdbool.h>

// #include "serial_parity.h"
// #include "arith.h"

// typedef volatile struct oc4_uart_regs oc4_uart_regs_t;
// struct oc4_uart_regs {
// //     uint32_t mu_io;         // 0x40: mini UART I/O Data
// //     uint32_t mu_ier;        // 0x44: mini UART interrupt enable
// //     uint32_t mu_iir;        // 0x48: mini UART interrupt identify
// //     uint32_t mu_lcr;        // 0x4c: mini UART line control
// //     uint32_t mu_mcr;        // 0x50: mini UART modem control
// //     uint32_t mu_lsr;        // 0x54: mini UART line status
// //     uint32_t mu_msr;        // 0x58: mini UART modem status
// //     uint32_t mu_scratch;    // 0x5c: mini UART scratch
// //     uint32_t mu_cntl;       // 0x60: mini UART extra control
// //     uint32_t mu_stat;       // 0x64: mini UART extra status
// //     uint32_t mu_baud;       // 0x68: mini UART baudrate
// };

// /* Modem Status Interrupt (MSI) bit 3,
//  * Line Status Interrupt (LSI) bit 2,
//  * and receive data (RXD) bit 0 need
//  * to be enabled in order to receive
//  * interrupts.
//  */
// #define MU_IER_ENA_RX_IRQ  (BIT(3) | BIT(2) | BIT(0))

// /* This bit is set if the transmit FIFO can accept at least one byte.*/
// #define MU_LSR_TXEMPTY   BIT(5)

// /* This bit is set if the transmit FIFO is empty and the
//  * transmitter is idle. (Finished shifting out the last bit). */
// #define MU_LSR_TXIDLE    BIT(6)
// #define MU_LSR_RXOVERRUN BIT(1)
// #define MU_LSR_DATAREADY BIT(0)

// #define MU_CNTL_RXE      BIT(0)
// #define MU_CNTL_TXE      BIT(1)

// #define MU_LCR_DLAB      BIT(7)
// #define MU_LCR_BREAK     BIT(6)
// #define MU_LCR_DATASIZE  BIT(0)

// /**
//  * Returns a pointer to the oc4_uart_regs struct.
//  * @param base_vaddr
//  * @return
//  */
// oc4_uart_regs_t *oc4_uart_regs_get(uintptr_t base_vaddr);

// /**
//  * Returns `True` if Transmit FIFO buffer is not empty and `False` otherwise.
//  * @param regs
//  * @return
//  */
// bool oc4_uart_regs_is_tx_fifo_busy(
//         oc4_uart_regs_t *regs
// );

// /**
//  * Disables Receive Interrupt Requests.
//  * @param regs
//  */
// void oc4_uart_regs_disable_rx_irq(
//         oc4_uart_regs_t *regs
// );

// /**
//  * Disables UART.
//  * @param regs
//  */
// void oc4_uart_regs_disable(
//         oc4_uart_regs_t *regs
// );

// /**
//  * Enables UART.
//  * @param regs
//  */
// void oc4_uart_regs_enable(
//         oc4_uart_regs_t *regs
// );

// /**
//  * Enables Receive Interrupt Requests.
//  * @param regs
//  */
// void oc4_uart_regs_enable_rx_irq(
//         oc4_uart_regs_t *regs
// );

// /**
//  * Transmits character to UART.
//  * @param regs
//  * @param ch
//  */
// void oc4_uart_regs_tx_char(
//         oc4_uart_regs_t *regs,
//         int ch
// );

// /**
//  * Returns True if the Receive buffer is ready to be read and `false` otherwise.
//  * @param regs
//  * @return
//  */
// bool oc4_uart_regs_is_rx_ready(
//         oc4_uart_regs_t *regs
// );

// /**
//  * Returns the character in the Receive buffer..
//  * @param regs
//  * @return
//  */
// int oc4_uart_regs_rx_char(
//         oc4_uart_regs_t *regs
// );

