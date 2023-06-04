/**
 * Generic interface for SDHCI-compliant MultiMediaCard Controller Registers.
 * - This is consumed by `sdhci.c`.
 * - If you are porting this driver to different hardware, this is the file you
 * should modify.
 */

#pragma once

#include "result.h"
#include "bcm_emmc_regs.h"

typedef struct sdhci_regs sdhci_regs_t;
struct sdhci_regs {
    bcm_emmc_regs_t *regs;
};

/**
 * Initialises the SDHCI registers.
 * @param sdhci_regs
 * @param regs
 * @return
 */
result_t sdhci_regs_init(
        sdhci_regs_t *sdhci_regs,
        void *regs
);

/**
 * Zeros out the `control0` register.
 * @param esdhci_regs
 */
result_t sdhci_regs_zero_control0(sdhci_regs_t *sdhci_regs);

/**
 * Zeros out the `control1` register.
 * @param sdhci_regs
 */
result_t sdhci_regs_zero_control1(sdhci_regs_t *sdhci_regs);

/**
 * Resets the complete host circuit.
 * @param sdhci_regs
 * @return
 */
result_t sdhci_regs_reset_host_circuit(sdhci_regs_t *sdhci_regs);

/**
 * Checks if the host circuit is in reset.
 * @param sdhci_regs
 * @param ret_val
 * @return
 */
result_t sdhci_regs_is_host_circuit_reset(sdhci_regs_t *sdhci_regs, bool *ret_val);

/**
 * Sets the data timeout to the maximum value.
 * @param sdhci_regs
 * @return
 */
result_t sdhci_regs_set_max_data_timeout(sdhci_regs_t *sdhci_regs);

/**
 * Enables the internal clock.
 * @param sdhci_regs
 * @return
 */
result_t sdhci_regs_enable_internal_clock(sdhci_regs_t *sdhci_regs);

/**
 * Returns True if Data Lines are in use and False otherwise.
 * @param sdhci_regs
 * @param ret_val
 * @return
 */
result_t sdhci_regs_is_data_lines_busy(sdhci_regs_t *sdhci_regs, bool *ret_val);

/**
 * Returns True if Command Line is in use and False otherwise.
 * @param sdhci_regs
 * @param ret_val
 * @return
 */
result_t sdhci_regs_is_cmd_line_busy(sdhci_regs_t *sdhci_regs, bool *ret_val);

/**
 * Disables the SD clock.
 * @param sdhci_regs
 * @return
 */
result_t sdhci_regs_disable_sd_clock(sdhci_regs_t *sdhci_regs);

/**
 * Enables the SD clock.
 * @param sdhci_regs
 * @return
 */
result_t sdhci_regs_enable_sd_clock(sdhci_regs_t *sdhci_regs);


/**
 * Returns the Host Controller specification version.
 * @param sdhci_regs
 * @param ret_val
 * @return
 */
result_t sdhci_regs_get_host_controller_spec_version(
        sdhci_regs_t *sdhci_regs,
        uint8_t *ret_val
);

/**
 * Sets the SD clock mode to "Divided".
 * @param sdhci_regs
 * @return
 */
result_t sdhci_regs_set_sd_clock_mode_as_divided(
        sdhci_regs_t *sdhci_regs
);

/**
 * Sets the SD clock's divisor.
 * @param sdhci_regs
 * @param divisor
 * @return
 */
result_t sdhci_regs_set_sd_clock_divisor(
        sdhci_regs_t *sdhci_regs,
        uint16_t divisor
);

/**
 * Returns True if the SD clock is stable and False otherwise.
 * @param sdhci_regs
 * @param ret_val
 * @return
 */
result_t sdhci_regs_is_sd_clock_stable(
        sdhci_regs_t *sdhci_regs,
        bool *ret_val
);

/**
 * Enables interrupts.
 * @param sdhci_regs
 * @return
 */
result_t sdhci_regs_enable_interrupts(
        sdhci_regs_t *sdhci_regs
);

/**
 * Masks interrupts.
 * @param sdhci_regs
 * @param mask The interrupt mask.
 * @param ret_val
 * @return
 */
result_t sdhci_regs_mask_interrupt(
        sdhci_regs_t *sdhci_regs,
        uint32_t mask,
        bool *ret_val
);

/**
 * Retrieves the interrupt as a raw 32-bit value.
 * @param sdhci_regs
 * @param ret_val
 * @return
 */
result_t sdhci_regs_get_interrupt_raw32(
        sdhci_regs_t *sdhci_regs,
        uint32_t *ret_val
);

/**
 * Sets the interrupt as a raw 32-bit value.
 * @param sdhci_regs
 * @param val
 * @return
 */
result_t sdhci_regs_set_interrupt_raw32(
        sdhci_regs_t *sdhci_regs,
        uint32_t val
);

/**
 * Returns true if there is a CMD timeout error and false otherwise.
 * @param sdhci_regs
 * @param ret_val
 * @return
 */
result_t sdhci_regs_is_cmd_timeout_err(
        sdhci_regs_t *sdhci_regs,
        bool *ret_val
);

/**
 * Returns true if there is a DATA timeout error and false otherwise.
 * @param sdhci_regs
 * @param ret_val
 * @return
 */
result_t sdhci_regs_is_data_timeout_err(
        sdhci_regs_t *sdhci_regs,
        bool *ret_val
);

/**
 * Returns true if there is any error and false otherwise.
 * @param sdhci_regs
 * @param ret_val
 * @return
 */
result_t sdhci_regs_is_any_err(
        sdhci_regs_t *sdhci_regs,
        bool *ret_val
);

/**
 * Returns true if there is a CMD in progress and false otherwise.
 * @param sdhci_regs
 * @param ret_val
 * @return
 */
result_t sdhci_regs_is_cmd_in_progress(
        sdhci_regs_t *sdhci_regs,
        bool *ret_val
);

/**
 * Returns true if there is a DATA in progress and false otherwise.
 * @param sdhci_regs
 * @param ret_val
 * @return
 */
result_t sdhci_regs_is_data_in_progress(
        sdhci_regs_t *sdhci_regs,
        bool *ret_val
);

/**
 * Clears interrupt flags.
 * @param sdhci_regs
 * @return
 */
result_t sdhci_regs_clear_interrupt(
        sdhci_regs_t *sdhci_regs
);

/**
 * Sets the `arg1` register.
 * @param sdhci_regs
 * @param val
 * @return
 */
result_t sdhci_regs_set_arg1(
        sdhci_regs_t *sdhci_regs,
        uint32_t val
);

/**
 * Sets the `cmdtm` register.
 * @param sdhci_regs
 * @param val
 * @return
 */
result_t sdhci_regs_set_cmdtm(
        sdhci_regs_t *sdhci_regs,
        cmdtm_t val
);

/**
 * Gets the `resp0` register.
 * @param sdhci_regs
 * @param ret_val
 * @return
 */
result_t sdhci_regs_get_resp0(
        sdhci_regs_t *sdhci_regs,
        uint32_t *ret_val
);

/**
 * Gets the `resp1` register.
 * @param sdhci_regs
 * @param ret_val
 * @return
 */
result_t sdhci_regs_get_resp1(
        sdhci_regs_t *sdhci_regs,
        uint32_t *ret_val
);

/**
 * Gets the `resp2` register.
 * @param sdhci_regs
 * @param ret_val
 * @return
 */
result_t sdhci_regs_get_resp2(
        sdhci_regs_t *sdhci_regs,
        uint32_t *ret_val
);

/**
 * Gets the `resp3` register.
 * @param sdhci_regs
 * @param ret_val
 * @return
 */
result_t sdhci_regs_get_resp3(
        sdhci_regs_t *sdhci_regs,
        uint32_t *ret_val
);

/**
 * Sets the block size.
 * @param sdhci_regs
 * @param val
 * @return
 */
result_t sdhci_regs_set_block_size(
        sdhci_regs_t *sdhci_regs,
        uint32_t val
);

/**
 * Sets the block count.
 * @param sdhci_regs
 * @param val
 * @return
 */
result_t sdhci_regs_set_block_count(
        sdhci_regs_t *sdhci_regs,
        uint32_t val
);

/**
 * Returns True if new data is available and ready to be read.
 * @param sdhci_regs
 * @param ret_val
 * @return
 */
result_t sdhci_regs_is_read_ready(
        sdhci_regs_t *sdhci_regs,
        bool *ret_val
);

/**
 * Returns True if new data is available and ready to be written.
 * @param sdhci_regs
 * @param ret_val
 * @return
 */
result_t sdhci_regs_is_write_ready(
        sdhci_regs_t *sdhci_regs,
        bool *ret_val
);

/**
 * Returns the data as a raw 32-bit value.
 * @param sdhci_regs
 * @param ret_val
 * @return
 */
result_t sdhci_regs_get_data(
        sdhci_regs_t *sdhci_regs,
        uint32_t *ret_val
);

/**
 * Sets the data as a raw 32-bit value.
 * @param sdhci_regs
 * @param val
 * @return
 */
result_t sdhci_regs_set_data(
        sdhci_regs_t *sdhci_regs,
        uint32_t val
);

/**
 * Enables or disables the use of 4 data lines.
 * @param sdhci_regs
 * @param val
 * @return
 */
result_t sdhci_regs_set_bus_width_4(
        sdhci_regs_t *sdhci_regs,
        bool val
);


