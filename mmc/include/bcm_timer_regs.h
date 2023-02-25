/**
 * Represents the BCM2835 System Timer registers as documented in Section 12 of
 * the BCM2835 ARM Peripherals manual.
 */

#pragma once

#include <stdint.h>

#include "result.h"

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

/**
 * Returns a pointer to the BCM2835 System Timer registers.
 *
 * @param base_vaddr The base virtual address of the BCM2835 System Timer
 * registers.
 * @return A pointer to the BCM2835 System Timer registers.
 */
bcm_timer_regs_t *bcm_timer_regs_get(uintptr_t base_vaddr);

/**
 * Returns the value of the `control_status` register.
 *
 * @param bcm_timer_regs A pointer to the BCM2835 System Timer registers.
 * @param ret_val Pointer to store return value into.
 * @return A result.
 */
result_t bcm_timer_regs_get_counter_lo(bcm_timer_regs_t *bcm_timer_regs, uint32_t *ret_val);

/**
 * Returns the value of the `control_status` register.
 *
 * @param bcm_timer_regs A pointer to the BCM2835 System Timer registers.
 * @param ret_val Pointer to store return value into.
 * @return A result.
 */
result_t bcm_timer_regs_get_counter_hi(bcm_timer_regs_t *bcm_timer_regs, uint32_t *ret_val);


