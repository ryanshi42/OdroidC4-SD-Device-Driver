#pragma once

#include <stdint.h>

/*
 * ENUMERATED FSEL REGISTERS ... BCM2835.PDF MANUAL see page 92		}
 */
enum gpio_mode {
    GPIO_INPUT = 0b000,     // 0
    GPIO_OUTPUT = 0b001,    // 1
    GPIO_ALTFUNC5 = 0b010,  // 2
    GPIO_ALTFUNC4 = 0b011,  // 3
    GPIO_ALTFUNC0 = 0b100,  // 4
    GPIO_ALTFUNC1 = 0b101,  // 5
    GPIO_ALTFUNC2 = 0b110,  // 6
    GPIO_ALTFUNC3 = 0b111   // 7
};
typedef enum gpio_mode gpio_mode_t;
