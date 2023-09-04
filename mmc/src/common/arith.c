#include "arith.h"

uint_fast8_t arith_find_most_sig_bit_set(uint32_t x) {
    return 32 - __builtin_clz(x);
}

const int tab32[32] = {
     0,  9,  1, 10, 13, 21,  2, 29,
    11, 14, 16, 18, 22, 25,  3, 30,
     8, 12, 20, 28, 15, 17, 24,  7,
    19, 27, 23,  6, 26,  5,  4, 31};

// This algorithm is a simple way to compute the ilog2. The Linux version is complicated and illegible.
// This algorithm uses the De Bruijn magic number! Very cool.
// https://stackoverflow.com/questions/11376288/fast-computing-of-log2-for-64-bit-integers
int ilog2 (uint32_t value)
{
    value |= value >> 1;
    value |= value >> 2;
    value |= value >> 4;
    value |= value >> 8;
    value |= value >> 16;
    return tab32[(uint32_t)(value*0x07C4ACDD) >> 27];
}

