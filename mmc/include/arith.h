#pragma once

#define BIT(n) (1ul<<(n))

/*
 * MASK_UNSAFE(3) -> 0b00111
 * MASK_UNSAFE(4) -> 0b01111
 * MASK_UNSAFE(5) -> 0b11111
 * etc...
 */
#define MASK_UNSAFE(x) ((BIT(x) - 1ul))

