#include "irpt_mask.h"

result_t irpt_mask_set_raw32(irpt_mask_t *irpt_mask, uint32_t val) {
    if (irpt_mask == NULL) {
        return result_err("NULL `irpt_mask` passed to irpt_mask_set_raw32().");
    }
    irpt_mask->raw32 = val;
    return result_ok();
}
