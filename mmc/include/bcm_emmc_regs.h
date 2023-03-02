/**
 * Represents the BCM2835 EMMC Registers as documented in Section 5 of the
 * BCM2835 ARM Peripherals manual.
 */

#pragma once

#include <stdint.h>

#include "result.h"
#include "control0.h"
#include "control1.h"
#include "status.h"
#include "slotisr_ver.h"
#include "irpt_mask.h"
#include "irpt_en.h"

typedef struct bcm_emmc_regs bcm_emmc_regs_t;
struct __attribute__((__packed__, aligned(4))) bcm_emmc_regs {
    uint32_t arg2; /* ACMD23 Argument. */
    uint32_t blksizecnt; /* Block Size and Count. */
    uint32_t arg1; /* Argument. */
    uint32_t cmdtm; /* Command and Transfer Mode. */
    uint32_t resp0; /* Response 0. */
    uint32_t resp1; /* Response 1. */
    uint32_t resp2; /* Response 2. */
    uint32_t resp3; /* Response 3. */
    uint32_t data; /* Data. */
    status_t status; /* Status. */
    control0_t control0; /* Control 0. */
    control1_t control1; /* Control 1. */
    uint32_t interrupt; /* Interrupt. */
    irpt_mask_t irpt_mask; /* Interrupt Mask. */
    irpt_en_t irpt_en; /* Interrupt Enable. */
    uint32_t control2; /* Control 2. */
    uint8_t undocumented0[0x50 - 0x40]; /* Undocumented portion of the Register Map. */
    uint32_t force_irpt; /* Force Interrupt. */
    uint8_t undocumented1[0x70 - 0x54]; /* Undocumented portion of the Register Map. */
    uint32_t boot_timeout; /* Boot Timeout. */
    uint32_t dbg_sel; /* Debug Select. */
    uint8_t undocumented2[0x80 - 0x78]; /* Undocumented portion of the Register Map. */
    uint32_t exrdfifo_cfg; /* Extended Read FIFO Configuration. */
    uint32_t exrdfifo_en; /* Extended Read FIFO Enable. */
    uint32_t tune_step; /* Tuning Step. */
    uint32_t tune_steps_std; /* Tuning Steps Standard. */
    uint32_t tune_steps_ddr; /* Tuning Steps DDR. */
    uint8_t undocumented3[0xf0 - 0x94]; /* Undocumented portion of the Register Map. */
    uint32_t spi_int_spt; /* SPI Interrupt Support. */
    uint8_t undocumented4[0xfc - 0xf4]; /* Undocumented portion of the Register Map. */
    slotisr_ver_t slotisr_ver; /* Slot Interrupt Status and Version. */
};

/**
 * Zeros out the `control0` register.
 * @param bcm_emmc_regs
 */
result_t bcm_emmc_regs_zero_control0(bcm_emmc_regs_t *bcm_emmc_regs);

/**
 * Zeros out the `control1` register.
 * @param bcm_emmc_regs
 */
result_t bcm_emmc_regs_zero_control1(bcm_emmc_regs_t *bcm_emmc_regs);

/**
 * Resets the complete host circuit.
 * @param bcm_emmc_regs
 * @return
 */
result_t bcm_emmc_regs_reset_host_circuit(bcm_emmc_regs_t *bcm_emmc_regs);

/**
 * Checks if the host circuit is in reset.
 * @param bcm_emmc_regs
 * @param ret_val
 * @return
 */
result_t bcm_emmc_regs_is_host_circuit_reset(bcm_emmc_regs_t *bcm_emmc_regs, bool *ret_val);

/**
 * Sets the data timeout to the maximum value.
 * @param bcm_emmc_regs
 * @return
 */
result_t bcm_emmc_regs_set_max_data_timeout(bcm_emmc_regs_t *bcm_emmc_regs);

/**
 * Enables the internal clock.
 * @param bcm_emmc_regs
 * @return
 */
result_t bcm_emmc_regs_enable_internal_clock(bcm_emmc_regs_t *bcm_emmc_regs);

/**
 * Returns True if Data Lines are in use and False otherwise.
 * @param bcm_emmc_regs
 * @param ret_val
 * @return
 */
result_t bcm_emmc_regs_is_data_lines_busy(bcm_emmc_regs_t *bcm_emmc_regs, bool *ret_val);

/**
 * Returns True if Command Line is in use and False otherwise.
 * @param bcm_emmc_regs
 * @param ret_val
 * @return
 */
result_t bcm_emmc_regs_is_cmd_line_busy(bcm_emmc_regs_t *bcm_emmc_regs, bool *ret_val);

/**
 * Disables the SD clock.
 * @param bcm_emmc_regs
 * @return
 */
result_t bcm_emmc_regs_disable_sd_clock(bcm_emmc_regs_t *bcm_emmc_regs);

/**
 * Enables the SD clock.
 * @param bcm_emmc_regs
 * @return
 */
result_t bcm_emmc_regs_enable_sd_clock(bcm_emmc_regs_t *bcm_emmc_regs);


/**
 * Returns the Host Controller specification version.
 * @param bcm_emmc_regs
 * @param ret_val
 * @return
 */
result_t bcm_emmc_regs_get_host_controller_spec_version(
        bcm_emmc_regs_t *bcm_emmc_regs,
        uint8_t *ret_val
);

/**
 * Sets the SD clock mode to "Divided".
 * @param bcm_emmc_regs
 * @return
 */
result_t bcm_emmc_regs_set_sd_clock_mode_as_divided(
        bcm_emmc_regs_t *bcm_emmc_regs
);

/**
 * Sets the SD clock's divisor.
 * @param bcm_emmc_regs
 * @param divisor
 * @return
 */
result_t bcm_emmc_regs_set_sd_clock_divisor(
        bcm_emmc_regs_t *bcm_emmc_regs,
        uint16_t divisor
);

/**
 * Returns True if the SD clock is stable and False otherwise.
 * @param bcm_emmc_regs
 * @param ret_val
 * @return
 */
result_t bcm_emmc_regs_is_sd_clock_stable(
        bcm_emmc_regs_t *bcm_emmc_regs,
        bool *ret_val
);

/**
 * Enables interrupts.
 * @param bcm_emmc_regs
 * @return
 */
result_t bcm_emmc_regs_enable_interrupts(
        bcm_emmc_regs_t *bcm_emmc_regs
);
