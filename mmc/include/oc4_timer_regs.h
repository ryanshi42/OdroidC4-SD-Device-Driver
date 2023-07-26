/**
 * Represents the oc4 System Timer registers as documented in Section 12 of
 * the oc4 ARM Peripherals manual.
 */

#pragma once

#include <stdint.h>
#include <arith.h>

#include "result.h"

// We will leave the timer register untouched for now, as it doesn't seem to need to work?

typedef struct oc4_timer_regs oc4_timer_regs_t;

//! Odroid C2
// struct __attribute__((__packed__, aligned(4))) oc4_timer_regs {
//     uint32_t control_status;  // 0x00
//     uint32_t counter_lo;      // 0x04
//     uint32_t counter_hi;      // 0x08
//     uint32_t compare0;        // 0x0C
//     uint32_t compare1;        // 0x10
//     uint32_t compare2;        // 0x14
//     uint32_t compare3;        // 0x18
// };

#define IRQ_CH 0
#define GET_TIME 0
#define SET_TIMEOUT 1
#define MAX_TIMEOUTS 6

//! Odroid C2
// #define TIMER_BASE      0xc1100000
// #define TIMER_MAP_BASE  0xc1109000
// #define TIMER_REG_START     0x940    // TIMER_MUX

#define TIMER_BASE          0xffd00000
#define TIMER_REG_START     0x3c50    // TIMER_MUX

#define TIMER_A_INPUT_CLK   0
#define TIMER_E_INPUT_CLK   8
#define TIMER_A_EN          BIT(16)
#define TIMER_A_MODE        BIT(12)

//: Haven't checked these yet, but surely they should be constants
#define TIMESTAMP_TIMEBASE_SYSTEM   0b000
#define TIMESTAMP_TIMEBASE_1_US     0b001
#define TIMESTAMP_TIMEBASE_10_US    0b010
#define TIMESTAMP_TIMEBASE_100_US   0b011
#define TIMESTAMP_TIMEBASE_1_MS     0b100

#define TIMEOUT_TIMEBASE_1_US       0b00
#define TIMEOUT_TIMEBASE_10_US      0b01
#define TIMEOUT_TIMEBASE_100_US     0b10
#define TIMEOUT_TIMEBASE_1_MS       0b11

#define NS_IN_US    1000ULL
#define NS_IN_MS    1000000ULL


//? Start timer E acts as a clock, while timer A can be used for timeouts from clients
struct __attribute__((__packed__, aligned(4))) oc4_timer_regs {
    uint32_t mux;           // 0x3c50
    uint32_t timer_a;       // 0x3c51
    uint32_t timer_b;       // 0x3c52
    uint32_t timer_c;       // 0x3c53
    uint32_t timer_d;       // 0x3c54
    uint32_t unused[7];     // 0x3c55 - 0x3c61 inclusive
    uint32_t timer_e;       // 0x3c62
    uint32_t timer_e_hi;    // 0x3c63
    uint32_t mux1;          // 0x3c64
    uint32_t timer_f;       // 0x3c65
    uint32_t timer_g;       // 0x3c66
    uint32_t timer_h;       // 0x3c67
    uint32_t timer_i;       // 0x3c68
    /* After this point, we get watchdog timer which is not what we care about here */
};



/**
 * Returns a pointer to the oc42835 System Timer registers.
 *
 * @param base_vaddr The base virtual address of the oc42835 System Timer
 * registers.
 * @return A pointer to the oc42835 System Timer registers.
 */
oc4_timer_regs_t *oc4_timer_regs_get(uintptr_t base_vaddr);

/**
 * Returns the value of the `control_status` register.
 *
 * @param oc4_timer_regs A pointer to the oc42835 System Timer registers.
 * @param ret_val Pointer to store return value into.
 * @return A result.
 */
result_t oc4_timer_regs_get_counter_lo(oc4_timer_regs_t *oc4_timer_regs, uint32_t *ret_val);

/**
 * Returns the value of the `control_status` register.
 *
 * @param oc4_timer_regs A pointer to the oc42835 System Timer registers.
 * @param ret_val Pointer to store return value into.
 * @return A result.
 */
result_t oc4_timer_regs_get_counter_hi(oc4_timer_regs_t *oc4_timer_regs, uint32_t *ret_val);


