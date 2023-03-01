#include "bcm_emmc.h"

result_t bcm_emmc_init(
        bcm_emmc_t *bcm_emmc,
        bcm_emmc_regs_t *bcm_emmc_regs
) {
    if (bcm_emmc == NULL) {
        return result_err("NULL `bcm_emmc` passed to bcm_emmc_init().");
    }
    if (bcm_emmc_regs == 0) {
        return result_err("NULL `bcm_emmc_regs` passed to bcm_emmc_init().");
    }
    bcm_emmc->regs = bcm_emmc_regs;
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
    /* Wait until host circuit has finished resetting. */
    size_t retries = 10000;
    bool is_host_circuit_reset = false;
    do {
        usleep(10); /* Wait for 10 microseconds. */
        res = bcm_emmc_regs_is_host_circuit_reset(
                bcm_emmc->regs,
                &is_host_circuit_reset
        );
        if (result_is_err(res)) {
            return result_err_chain(res, "Failed to check if host circuit was reset in bcm_emmc_init().");
        }
    } while (!is_host_circuit_reset && (retries-- > 0));
    if (!is_host_circuit_reset) {
        return result_err("Host circuit did not reset in bcm_emmc_init().");
    }

    return result_ok();
}
