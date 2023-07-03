#include "sdhci_regs.h"
#include "oc4_emmc_regs.h"

result_t sdhci_regs_init(sdhci_regs_t *sdhci_regs, void *regs) {
    if (sdhci_regs == NULL) {
        return result_err("NULL `sdhci_regs` passed to sdhci_regs_init().");
    }
    if (regs == NULL) {
        return result_err("NULL `regs` passed to sdhci_regs_init().");
    }
    sdhci_regs->regs = (oc4_emmc_regs_t *) regs;
    return result_ok();
}

result_t sdhci_regs_zero_control0(sdhci_regs_t *sdhci_regs) {
    return oc4_emmc_regs_zero_control0(sdhci_regs->regs);
}

result_t sdhci_regs_zero_control1(sdhci_regs_t *sdhci_regs) {
    return oc4_emmc_regs_zero_control1(sdhci_regs->regs);
}

result_t sdhci_regs_reset_host_circuit(sdhci_regs_t *sdhci_regs) {
    return oc4_emmc_regs_reset_host_circuit(sdhci_regs->regs);
}

result_t sdhci_regs_is_host_circuit_reset(sdhci_regs_t *sdhci_regs, bool *ret_val) {
    return oc4_emmc_regs_is_host_circuit_reset(sdhci_regs->regs, ret_val);
}

result_t sdhci_regs_set_max_data_timeout(sdhci_regs_t *sdhci_regs) {
    return oc4_emmc_regs_set_max_data_timeout(sdhci_regs->regs);
}

result_t sdhci_regs_enable_internal_clock(sdhci_regs_t *sdhci_regs) {
    return oc4_emmc_regs_enable_internal_clock(sdhci_regs->regs);
}

result_t sdhci_regs_is_data_lines_busy(sdhci_regs_t *sdhci_regs, bool *ret_val) {
    return oc4_emmc_regs_is_data_lines_busy(sdhci_regs->regs, ret_val);
}

result_t sdhci_regs_is_cmd_line_busy(sdhci_regs_t *sdhci_regs, bool *ret_val) {
    return oc4_emmc_regs_is_cmd_line_busy(sdhci_regs->regs, ret_val);
}

result_t sdhci_regs_disable_sd_clock(sdhci_regs_t *sdhci_regs) {
    return oc4_emmc_regs_disable_sd_clock(sdhci_regs->regs);
}

result_t sdhci_regs_enable_sd_clock(sdhci_regs_t *sdhci_regs) {
    return oc4_emmc_regs_enable_sd_clock(sdhci_regs->regs);
}

result_t sdhci_regs_get_host_controller_spec_version(
        sdhci_regs_t *sdhci_regs,
        uint8_t *ret_val
) {
    return oc4_emmc_regs_get_host_controller_spec_version(sdhci_regs->regs, ret_val);
}

result_t sdhci_regs_set_sd_clock_mode_as_divided(
        sdhci_regs_t *sdhci_regs
) {
    return oc4_emmc_regs_set_sd_clock_mode_as_divided(sdhci_regs->regs);
}

result_t sdhci_regs_set_sd_clock_divisor(
        sdhci_regs_t *sdhci_regs,
        uint16_t divisor
) {
    return oc4_emmc_regs_set_sd_clock_divisor(sdhci_regs->regs, divisor);
}

result_t sdhci_regs_is_sd_clock_stable(
        sdhci_regs_t *sdhci_regs,
        bool *ret_val
) {
    return oc4_emmc_regs_is_sd_clock_stable(sdhci_regs->regs, ret_val);
}

result_t sdhci_regs_enable_interrupts(
        sdhci_regs_t *sdhci_regs
) {
    return oc4_emmc_regs_enable_interrupts(sdhci_regs->regs);
}

result_t sdhci_regs_mask_interrupt(
        sdhci_regs_t *sdhci_regs,
        uint32_t mask,
        bool *ret_val
) {
    return oc4_emmc_regs_mask_interrupt(sdhci_regs->regs, mask, ret_val);
}

result_t sdhci_regs_get_interrupt_raw32(
        sdhci_regs_t *sdhci_regs,
        uint32_t *ret_val
) {
    return oc4_emmc_regs_get_interrupt_raw32(sdhci_regs->regs, ret_val);
}

result_t sdhci_regs_set_interrupt_raw32(
        sdhci_regs_t *sdhci_regs,
        uint32_t val
) {
    return oc4_emmc_regs_set_interrupt_raw32(sdhci_regs->regs, val);
}

result_t sdhci_regs_is_cmd_timeout_err(
        sdhci_regs_t *sdhci_regs,
        bool *ret_val
) {
    return oc4_emmc_regs_is_cmd_timeout_err(sdhci_regs->regs, ret_val);
}

result_t sdhci_regs_is_data_timeout_err(
        sdhci_regs_t *sdhci_regs,
        bool *ret_val
) {
    return oc4_emmc_regs_is_data_timeout_err(sdhci_regs->regs, ret_val);
}

result_t sdhci_regs_is_any_err(
        sdhci_regs_t *sdhci_regs,
        bool *ret_val
) {
    return oc4_emmc_regs_is_any_err(sdhci_regs->regs, ret_val);
}

result_t sdhci_regs_is_cmd_in_progress(
        sdhci_regs_t *sdhci_regs,
        bool *ret_val
) {
    return oc4_emmc_regs_is_cmd_in_progress(sdhci_regs->regs, ret_val);
}

result_t sdhci_regs_is_data_in_progress(
        sdhci_regs_t *sdhci_regs,
        bool *ret_val
) {
    return oc4_emmc_regs_is_data_in_progress(sdhci_regs->regs, ret_val);
}

result_t sdhci_regs_clear_interrupt(
        sdhci_regs_t *sdhci_regs
) {
    return oc4_emmc_regs_clear_interrupt(sdhci_regs->regs);
}

result_t sdhci_regs_set_arg1(
        sdhci_regs_t *sdhci_regs,
        uint32_t val
) {
    return oc4_emmc_regs_set_arg1(sdhci_regs->regs, val);
}

result_t sdhci_regs_set_cmdtm(
        sdhci_regs_t *sdhci_regs,
        cmdtm_t val
) {
    return oc4_emmc_regs_set_cmdtm(sdhci_regs->regs, val);
}

result_t sdhci_regs_get_resp0(
        sdhci_regs_t *sdhci_regs,
        uint32_t *ret_val
) {
    return oc4_emmc_regs_get_resp0(sdhci_regs->regs, ret_val);
}

result_t sdhci_regs_get_resp1(
        sdhci_regs_t *sdhci_regs,
        uint32_t *ret_val
) {
    return oc4_emmc_regs_get_resp1(sdhci_regs->regs, ret_val);
}

result_t sdhci_regs_get_resp2(
        sdhci_regs_t *sdhci_regs,
        uint32_t *ret_val
) {
    return oc4_emmc_regs_get_resp2(sdhci_regs->regs, ret_val);
}

result_t sdhci_regs_get_resp3(
        sdhci_regs_t *sdhci_regs,
        uint32_t *ret_val
) {
    return oc4_emmc_regs_get_resp3(sdhci_regs->regs, ret_val);
}

result_t sdhci_regs_set_block_size(
        sdhci_regs_t *sdhci_regs,
        uint32_t val
) {
    return oc4_emmc_regs_set_block_size(sdhci_regs->regs, val);
}

result_t sdhci_regs_set_block_count(
        sdhci_regs_t *sdhci_regs,
        uint32_t val
) {
    return oc4_emmc_regs_set_block_count(sdhci_regs->regs, val);
}

result_t sdhci_regs_is_read_ready(
        sdhci_regs_t *sdhci_regs,
        bool *ret_val
) {
    return oc4_emmc_regs_is_read_ready(sdhci_regs->regs, ret_val);
}

result_t sdhci_regs_is_write_ready(
        sdhci_regs_t *sdhci_regs,
        bool *ret_val
) {
    return oc4_emmc_regs_is_write_ready(sdhci_regs->regs, ret_val);
}

result_t sdhci_regs_get_data(
        sdhci_regs_t *sdhci_regs,
        uint32_t *ret_val
) {
    return oc4_emmc_regs_get_data(sdhci_regs->regs, ret_val);
}

result_t sdhci_regs_set_data(
        sdhci_regs_t *sdhci_regs,
        uint32_t val
) {
    return oc4_emmc_regs_set_data(sdhci_regs->regs, val);
}

result_t sdhci_regs_set_bus_width_4(
        sdhci_regs_t *sdhci_regs,
        bool val
) {
    return oc4_emmc_regs_set_bus_width_4(sdhci_regs->regs, val);
}
