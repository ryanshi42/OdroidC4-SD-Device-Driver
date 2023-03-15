#pragma once

#include <stdint.h>

/*
 * ENUMERATED FSEL REGISTERS ... BCM2835.PDF MANUAL see page 92		}
 */
#define GPIO_INPUT		0b000							// 0
#define GPIO_OUTPUT		0b001							// 1
#define GPIO_ALTFUNC5	0b010							// 2
#define GPIO_ALTFUNC4	0b011							// 3
#define GPIO_ALTFUNC0	0b100							// 4
#define	GPIO_ALTFUNC1	0b101							// 5
#define	GPIO_ALTFUNC2	0b110							// 6
#define GPIO_ALTFUNC3	0b111							// 7
typedef uint8_t gpio_mode_t;