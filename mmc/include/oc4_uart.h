#pragma once

#include "shared_ringbuffer.h"
#include <stdbool.h>

#define BIT(nr) (1UL << (nr))

/* AML_UART_CONTROL bits */
#define AML_UART_TX_EN			BIT(12)
#define AML_UART_RX_EN			BIT(13)
#define AML_UART_TWO_WIRE_EN		BIT(15)
#define AML_UART_STOP_BIT_LEN_MASK	(0x03 << 16)
#define AML_UART_STOP_BIT_1SB		(0x00 << 16)
#define AML_UART_STOP_BIT_2SB		(0x01 << 16)
#define AML_UART_PARITY_TYPE		BIT(18)
#define AML_UART_PARITY_EN		BIT(19)
#define AML_UART_TX_RST			BIT(22)
#define AML_UART_RX_RST			BIT(23)
#define AML_UART_CLEAR_ERR		BIT(24)
#define AML_UART_RX_INT_EN		BIT(27)
#define AML_UART_TX_INT_EN		BIT(28)
#define AML_UART_DATA_LEN_MASK		(0x03 << 20)
#define AML_UART_DATA_LEN_8BIT		(0x00 << 20)
#define AML_UART_DATA_LEN_7BIT		(0x01 << 20)
#define AML_UART_DATA_LEN_6BIT		(0x02 << 20)
#define AML_UART_DATA_LEN_5BIT		(0x03 << 20)
/* AML_UART_STATUS bits */
#define AML_UART_PARITY_ERR		BIT(16)
#define AML_UART_FRAME_ERR		BIT(17)
#define AML_UART_TX_FIFO_WERR		BIT(18)
#define AML_UART_RX_EMPTY		BIT(20)
#define AML_UART_TX_FULL		BIT(21)
#define AML_UART_TX_EMPTY		BIT(22)
#define AML_UART_XMIT_BUSY		BIT(25)
#define AML_UART_ERR			(AML_UART_PARITY_ERR | AML_UART_FRAME_ERR  | AML_UART_TX_FIFO_WERR)
/* AML_UART_IRQ bits */
#define AML_UART_XMIT_IRQ(c)		(((c) & 0xff) << 8)
#define AML_UART_RECV_IRQ(c)		((c) & 0xff)
/* AML_UART_REG5 bits */
#define AML_UART_BAUD_MASK		0x7fffff
#define AML_UART_BAUD_USE		BIT(23)
#define AML_UART_BAUD_XTAL		BIT(24)
#define AML_UART_BAUD_XTAL_DIV2		BIT(27)

// Move this into a seperate file in the future
#define NUM_BUFFERS 512
#define BUFFER_SIZE 2048

enum serial_parity {
    PARITY_NONE,
    PARITY_EVEN,
    PARITY_ODD
};

struct oc4_uart_regs {
    uint32_t wfifo;      /* 0x000 Write Data */
    uint32_t rfifo;      /* 0x040 Read Data */
    uint32_t cr;         /* 0x080 Control Register */
    uint32_t sr;         /* 0x0c0 Status Register */
    uint32_t irqc;       /* 0x100 IRQ Control Register*/
    uint32_t reg5;       /* 0x140 Baud Rate Control */
};
typedef volatile struct oc4_uart_regs oc4_uart_regs_t;

// /*
// serial driver struct akin to patrick's implementation*/
// struct serial_driver {
//     oc4_uart_regs_t *regs;

//     ring_handle_t rx_ring;
//     ring_handle_t tx_ring;

//     int num_to_get_chars;
// };
//? This has already been defined elsewhere


typedef struct oc4_uart oc4_uart_t;
struct oc4_uart {
    volatile oc4_uart_regs_t *regs;
    /* Will be `true` if user wants `\n` chars to be automatically converted to
     * `\r\n` and will be `false` otherwise. */
    bool auto_insert_carriage_return;
};

/**
 * Initialises the IMX Uart device.
 * @param oc4_uart
 * @param base_vaddr
 * @param auto_insert_carriage_return Set to `true` if you want `\n` chars to be
 * automatically converted to `\r\n` and set to `false` otherwise.
 * @return
 */
bool oc4_uart_init(
        oc4_uart_t *oc4_uart,
        uintptr_t base_vaddr,
        bool auto_insert_carriage_return
);

/**
 * Prints out a character using the imx uart device.
 * @param oc4_uart
 * @param c
 * @return
 */
int oc4_uart_put_char(
        oc4_uart_t *oc4_uart,
        int c
);

/**
 * Gets character from UART device.
 * @param oc4_uart
 * @return
 */
int oc4_uart_get_char(
        oc4_uart_t *oc4_uart
);