#pragma once

#include <stdint.h>

/*
 * ENUMERATED GPIO FIX RESISTOR ... BCM2835.PDF MANUAL see page 101
 */
enum gpio_fix_resister {
    NO_RESISTOR = 0b00, // 0
    PULLUP = 0b01,      // 1
    PULLDOWN = 0b10     // 2
};
typedef enum gpio_fix_resister gpio_fix_resistor_t;

