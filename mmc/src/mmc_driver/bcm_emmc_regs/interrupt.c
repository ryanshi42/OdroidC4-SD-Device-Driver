#include "interrupt.h"

result_t interrupt_get_raw32(interrupt_t *interrupt, uint32_t *ret_val) {
    if (interrupt == NULL) {
        return result_err("NULL `interrupt` passed to interrupt_get_raw32().");
    }
    *ret_val = interrupt->raw32;
    return result_ok();
}

result_t interrupt_set_raw32(interrupt_t *interrupt, uint32_t val) {
    if (interrupt == NULL) {
        return result_err("NULL `interrupt` passed to interrupt_set_raw32().");
    }
    interrupt->raw32 = val;
    return result_ok();
}

result_t interrupt_mask_raw32(interrupt_t *interrupt, uint32_t mask, bool *ret_val) {
    if (interrupt == NULL) {
        return result_err("NULL `interrupt` passed to interrupt_mask_raw32().");
    }
    *ret_val = interrupt->raw32 & mask;
    return result_ok();
}

result_t interrupt_get_cmd_done(interrupt_t *interrupt, bool *ret_val) {
    if (interrupt == NULL) {
        return result_err("NULL `interrupt` passed to interrupt_get_cmd_done().");
    }
    *ret_val = interrupt->CMD_DONE;
    return result_ok();
}

result_t interrupt_get_data_done(interrupt_t *interrupt, bool *ret_val) {
    if (interrupt == NULL) {
        return result_err("NULL `interrupt` passed to interrupt_get_data_done().");
    }
    *ret_val = interrupt->DATA_DONE;
    return result_ok();
}

result_t interrupt_get_write_rdy(interrupt_t *interrupt, bool *ret_val) {
    if (interrupt == NULL) {
        return result_err("NULL `interrupt` passed to interrupt_get_write_rdy().");
    }
    *ret_val = interrupt->WRITE_RDY;
    return result_ok();
}

result_t interrupt_get_read_rdy(interrupt_t *interrupt, bool *ret_val) {
    if (interrupt == NULL) {
        return result_err("NULL `interrupt` passed to interrupt_get_read_rdy().");
    }
    *ret_val = interrupt->READ_RDY;
    return result_ok();
}

result_t interrupt_get_cto_err(interrupt_t *interrupt, bool *ret_val) {
    if (interrupt == NULL) {
        return result_err("NULL `interrupt` passed to interrupt_get_cto_err().");
    }
    *ret_val = interrupt->CTO_ERR;
    return result_ok();
}

result_t interrupt_get_dto_err(interrupt_t *interrupt, bool *ret_val) {
    if (interrupt == NULL) {
        return result_err("NULL `interrupt` passed to interrupt_get_dto_err().");
    }
    *ret_val = interrupt->DTO_ERR;
    return result_ok();
}

result_t interrupt_clear(interrupt_t *interrupt) {
    if (interrupt == NULL) {
        return result_err("NULL `interrupt` passed to interrupt_clear().");
    }
    interrupt->raw32 = interrupt->raw32;
    return result_ok();
}
