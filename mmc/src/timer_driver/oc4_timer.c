#include "oc4_timer.h"
#include "sel4cp.h"

result_t oc4_timer_init(
        oc4_timer_t *oc4_timer,
        uintptr_t base_vaddr
) {
    if (oc4_timer == NULL) {
        return result_err("NULL `oc4_timer` passed to oc4_timer_init().");
    }
    if (base_vaddr == 0) {
        return result_err("NULL `base_vaddr` passed to oc4_timer_init().");
    }
    // Set the register to be equal to the base virtual address
    //: Must add the 0xc50 as the timer register is a little offset
    // return result_err("should fail");
    oc4_timer->regs = oc4_timer_regs_get(base_vaddr + 0x140);
    // oc4_timer->regs = (oc4_timer_regs_t *) 0xffd0f140;
    /* Start timer E acts as a clock, while timer A can be used for timeouts from clients */
    oc4_timer->regs->mux = TIMER_A_EN | (0b001 << TIMER_E_INPUT_CLK) | (TIMEOUT_TIMEBASE_1_MS << TIMER_A_INPUT_CLK);
    // oc4_timer->regs->mux = (0b001 << TIMER_E_INPUT_CLK);
    // oc4_timer->regs->timer_e = 0b111111;
    // if (oc4_timer->regs->mux == 0) {
    //     return result_err("failed");
    // }
    // sel4cp_dbg_puts("I HAVE BEEN CALLED\n\n\n\n\n\n\n");
    
    return result_ok();
}

// char* itoa(int i, char b[]){
//     char const digit[] = "0123456789";
//     char* p = b;
//     if(i<0){
//         *p++ = '-';
//         i *= -1;
//     }
//     int shifter = i;
//     do{ //Move to where representation ends
//         ++p;
//         shifter = shifter/10;
//     }while(shifter);
//     *p = '\0';
//     do{ //Move back, inserting digits as u go
//         *--p = digit[i%10];
//         i = i/10;
//     }while(i);
//     return b;
// }

result_t oc4_timer_get_num_ticks(
        oc4_timer_t *oc4_timer,
        uint64_t *ret_val
) {
    if (oc4_timer == NULL) {
        return result_err("NULL `oc4_timer` passed to oc4_timer_get_ticks().");
    }
    if (ret_val == NULL) {
        return result_err("NULL `ret_val` passed to oc4_timer_get_ticks().");
    }
    // if (oc4_timer->regs->mux == 0) {
    //     return result_err("failed");
    // }
    // char snum[10];
    // sel4cp_dbg_puts(itoa(oc4_timer->regs->mux, snum));
    // sel4cp_dbg_puts("\n");

    uint32_t counter_lo;
    uint32_t counter_hi;
    uint32_t counter_hi_tmp;
    //? A bit confused at this code... 
    //? On a little-endian system, the higher 32 bits are referred to as "lo." So counter_hi would be the smaller ones so are more likely to tick over.
    do {
        result_t res = oc4_timer_regs_get_counter_lo(oc4_timer->regs, &counter_lo);
        if (result_is_err(res)) {
            return result_err("Failed to get `counter_lo` in oc4_timer_get_ticks().");
        }
        res = oc4_timer_regs_get_counter_hi(oc4_timer->regs, &counter_hi);
        if (result_is_err(res)) {
            return result_err("Failed to get `counter_hi` in oc4_timer_get_ticks().");
        }
        res = oc4_timer_regs_get_counter_hi(oc4_timer->regs, &counter_hi_tmp);
        if (result_is_err(res)) {
            return result_err("Failed to get `counter_hi_tmp` in oc4_timer_get_ticks().");
        }
    } while (counter_hi_tmp != counter_hi); /* Check system timer hasn't ticked over in that time. */
    /* Join the two 32-bit halves into a single 64-bit integer. */
    *ret_val = ((uint64_t) counter_hi << 32) | counter_lo;
    return result_ok();
}

