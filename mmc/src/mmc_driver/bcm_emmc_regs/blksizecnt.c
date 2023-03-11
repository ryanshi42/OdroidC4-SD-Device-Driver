#include "blksizecnt.h"

result_t blksizecnt_set_blksize(blksizecnt_t *blksizecnt, uint32_t val) {
    if (blksizecnt == NULL) {
        return result_err("NULL `blksizecnt` passed to blksizecnt_set_blksize().");
    }
    if (val > 0b1111111111) {
        return result_err("Invalid `val` passed to blksizecnt_set_blksize().");
    }
    blksizecnt->BLKSIZE = val;
    return result_ok();
}

result_t blksizecnt_set_blkcnt(blksizecnt_t *blksizecnt, uint32_t val) {
    if (blksizecnt == NULL) {
        return result_err("NULL `blksizecnt` passed to blksizecnt_set_blkcnt().");
    }
    if (val > 0b1111111111111111) {
        return result_err("Invalid `val` passed to blksizecnt_set_blkcnt().");
    }
    blksizecnt->BLKCNT = val;
    return result_ok();
}
