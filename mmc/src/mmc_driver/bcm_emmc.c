#include "bcm_emmc.h"

result_t bcm_emmc_init(
        bcm_emmc_t *bcm_emmc,
        uintptr_t base_vaddr
) {
    if (bcm_emmc == NULL) {
        return result_err("NULL `bcm_emmc` passed to bcm_emmc_init().");
    }
    if (base_vaddr == 0) {
        return result_err("NULL `base_vaddr` passed to bcm_emmc_init().");
    }
    bcm_emmc->regs = bcm_emmc_regs_get(base_vaddr);
    /* Set control0 to zero. */
    result_t res = bcm_emmc_regs_zero_control0(bcm_emmc->regs);
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to zero `control0` in bcm_emmc_init().");
    }
    /* Set control1 to zero. */
    res = bcm_emmc_regs_zero_control1(bcm_emmc->regs);
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to zero `control1` in bcm_emmc_init().");
    }
    /* Reset the complete host circuit */
    res = bcm_emmc_regs_reset_host_circuit(bcm_emmc->regs);
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to reset host circuit in bcm_emmc_init().");
    }


    return result_ok();
}
