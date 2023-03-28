#include "arith.h"

uint_fast8_t arith_find_most_sig_bit_set(uint32_t x) {
    return 32 - __builtin_clz(x);
}

