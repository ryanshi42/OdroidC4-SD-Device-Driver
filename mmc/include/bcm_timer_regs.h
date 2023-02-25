/**
 * Represents the BCM2835 System Timer registers as documented in Section 12 of
 * the BCM2835 ARM Peripherals manual.
 */

#pragma once

#include <stdint.h>

typedef struct bcm_timer_regs bcm_timer_regs_t;
struct __attribute__((__packed__, aligned(4))) bcm_timer_regs {
    uint32_t control_status;  // 0x00
    uint32_t counter_lo;      // 0x04
    uint32_t counter_hi;      // 0x08
    uint32_t compare0;        // 0x0C
    uint32_t compare1;        // 0x10
    uint32_t compare2;        // 0x14
    uint32_t compare3;        // 0x18
};
