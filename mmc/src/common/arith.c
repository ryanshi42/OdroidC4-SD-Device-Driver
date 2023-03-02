#include "arith.h"

uint_fast8_t arith_find_most_sig_bit_set(uint32_t x) {
    uint_fast8_t r = 32;              // Start at 32
    if (!x) return 0;            // If x is zero answer must be zero
    if (!(x & 0xffff0000u)) {    // If none of the upper word bits are set
        x <<= 16;                // We can roll it up 16 bits
        r -= 16;                 // Reduce r by 16
    }
    if (!(x & 0xff000000u)) {    // If none of uppermost byte bits are set
        x <<= 8;                 // We can roll it up by 8 bits
        r -= 8;                  // Reduce r by 8
    }
    if (!(x & 0xf0000000u)) {    // If none of the uppermost 4 bits are set
        x <<= 4;                 // We can roll it up by 4 bits
        r -= 4;                  // Reduce r by 4
    }
    if (!(x & 0xc0000000u)) {    // If none of the uppermost 2 bits are set
        x <<= 2;                 // We can roll it up by 2 bits
        r -= 2;                  // Reduce r by 2
    }
    if (!(x & 0x80000000u)) {    // If the uppermost bit is not set
        x <<= 1;                 // We can roll it up by 1 bit
        r -= 1;                  // Reduce r by 1
    }
    return r;                    // Return the number of the uppermost set bit
}

