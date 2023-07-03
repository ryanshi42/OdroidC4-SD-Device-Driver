#include "irpt_en.h"

result_t irpt_en_set_raw32(irpt_en_t *irpt_en, uint32_t val) {
    if (irpt_en == NULL) {
        return result_err("NULL `irpt_en` passed to irpt_en_set_raw32().");
    }
    irpt_en->raw32 = val;
    return result_ok();
}
