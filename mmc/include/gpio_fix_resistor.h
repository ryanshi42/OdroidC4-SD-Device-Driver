#pragma once

#include <stdint.h>

/*
 * ENUMERATED GPIO FIX RESISTOR ... BCM2835.PDF MANUAL see page 101
 */
/* In binary so any error is obvious */
#define NO_RESISTOR		0b00							// 0
#define PULLUP			0b01							// 1
#define PULLDOWN		0b10							// 2
typedef uint8_t gpio_fix_resistor_t;

