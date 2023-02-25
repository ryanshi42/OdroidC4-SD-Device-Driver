#include "sel4cp_facade.h"

#if MMC_RPI3B_UNIT_TEST

void sel4cp_notify(sel4cp_channel ch) {
    (void) ch;
}

void sel4cp_dbg_puts(const char *s) {
    (void) s;
}


sel4cp_msginfo sel4cp_msginfo_new(uint64_t label, uint16_t count) {
    (void) label;
    (void) count;
    return 0;
}


void sel4cp_irq_ack(sel4cp_channel ch) {
    (void) ch;
}

#endif
