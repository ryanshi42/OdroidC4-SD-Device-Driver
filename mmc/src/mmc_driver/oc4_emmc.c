#include "oc4_emmc.h"
#include "oc4_emmc_regs.h"

/*

HIGH LEVEL OVERVIEW:

Stage 0: Write to all the registers

Stage 1: Read and write to a register, if not configured properly, then the register might not update

Stage 2: Read and write a disk block on the SD card

*/

static char
hexchar(unsigned int v)
{
    return v < 10 ? '0' + v : ('a' - 10) + v;
}

static void
puthex32(uint32_t val)
{
    char buffer[8 + 3];
    buffer[0] = '0';
    buffer[1] = 'x';
    buffer[8 + 3 - 1] = 0;
    for (unsigned i = 8 + 1; i > 1; i--) {
        buffer[i] = hexchar(val & 0xf);
        val >>= 4;
    }
    sel4cp_dbg_puts(buffer);
}





// Configure the clock
static void oc4_emmc_regs_mmc_config_clock(struct oc4_emmc_regs *mmc)
{
	uint32_t oc4_emmc_regs_mmc_clk = 1;
	unsigned int clk, clk_src, clk_div;

	if (!mmc->sd_emmc_clock)
		return;

    //? Debug statement to copy paste in case clock produces unexpected behaviour.
    // sel4cp_dbg_puts("Putting clock...:\n");
    // puthex32(mmc->sd_emmc_clock);

	/* TOFIX This should use the proper clock taken from DT */

	/* 1GHz / CLK_MAX_DIV = 15,9 MHz */
	if (mmc->sd_emmc_clock > 16000000) {
		clk = SD_EMMC_CLKSRC_DIV2;
		clk_src = CLK_SRC_DIV2;
	} else {
		clk = SD_EMMC_CLKSRC_24M;
		clk_src = CLK_SRC_24M;
	}
	clk_div = div_round_up(clk, mmc->sd_emmc_clock);

	/*
     * From the U-Boot driver:
     * 
	 * SM1 SoCs doesn't work fine over 50MHz with CLK_CO_PHASE_180
	 * If CLK_CO_PHASE_270 is used, it's more stable than other.
	 * Other SoCs use CLK_CO_PHASE_180 by default.
	 * It needs to find what is a proper value about each SoCs.
     * 
     * P.S. Manual says that core phase 180 is recommended, against the advice of the U-Boot driver.
     *
	 */
	// if (true) /* The Odroid C4 we are using uses the SM1 SoC so we should be fine here */
	// 	oc4_emmc_regs_mmc_clk |= CLK_CO_PHASE_270;
	// else
	// 	oc4_emmc_regs_mmc_clk |= CLK_CO_PHASE_180;

	// /* 180 phase tx sd_emmc_clock */
	// oc4_emmc_regs_mmc_clk |= CLK_TX_PHASE_000;

    oc4_emmc_regs_mmc_clk |= CLK_CO_PHASE_270;
	oc4_emmc_regs_mmc_clk |= CLK_TX_PHASE_000;
	oc4_emmc_regs_mmc_clk |= CLK_RX_PHASE_000;

	/* sd_emmc_clock settings */
	oc4_emmc_regs_mmc_clk |= clk_src;
	oc4_emmc_regs_mmc_clk |= clk_div;

	// oc4_emmc_regs_write_unsafe(mmc, oc4_emmc_regs_mmc_clk, SD_EMMC_CLOCK);
	mmc->sd_emmc_clock = oc4_emmc_regs_mmc_clk;
	// mmc->sd_emmc_clock |= 0xffffffff;

    // Setting the gpio part

    // clk81_ptr = (volatile uint32_t *)(clk_vaddr + CLK_OFFSET2);
    // clk81 = *clk81_ptr;

    // // Enable by removing clock gate
    // clk81 |= (SD_CLK81_BIT);
    // *clk81_ptr = clk81;

    // // Check that registers actually changed
    // if (!(*clk81_ptr & SD_CLK81_BIT)) {
    //     sel4cp_dbg_puts("driver: failed to toggle clock gate!\n");
    // } else {
    //     sel4cp_dbg_puts("driver: successfully toggled clock gate!\n");
    // }
    
}

// Set the IO configuration of the registers
static int oc4_emmc_regs_mmc_set_ios(struct oc4_emmc_regs *mmc)
{
	uint32_t oc4_emmc_regs_mmc_cfg = 0;

    // Read the config
	// oc4_emmc_regs_mmc_cfg = oc4_emmc_regs_read_unsafe(mmc, SD_EMMC_CFG);
	oc4_emmc_regs_mmc_cfg = mmc->sd_emmc_cfg;

    // Get the bus width from the registers
    // Does this mean it is automatically instantiated??? // TODO: check this!
	oc4_emmc_regs_mmc_cfg &= ~CFG_BUS_WIDTH_MASK;

    // oc4_emmc_regs_set_bus_width_1(mmc);
	oc4_emmc_regs_mmc_cfg |= CFG_BUS_WIDTH_1;
    // oc4_emmc_regs_set_bus_width_4(mmc);
    // oc4_emmc_regs_mmc_cfg |= CFG_BUS_WIDTH_8;
	// if (mmc->bus_width == 1)
	// 	oc4_emmc_regs_mmc_cfg |= CFG_BUS_WIDTH_1;
	// else if (mmc->bus_width == 4)
	// 	oc4_emmc_regs_mmc_cfg |= CFG_BUS_WIDTH_4;
	// else if (mmc->bus_width == 8)
	// 	oc4_emmc_regs_mmc_cfg |= CFG_BUS_WIDTH_8;
	// else
	// 	return -EINVAL;

    // The following are the default options
    // Some of these constants have been "redefined": // TODO: fix later!!!

	/* 512 bytes block length */
	oc4_emmc_regs_mmc_cfg &= ~CFG_BL_LEN_MASK;
	oc4_emmc_regs_mmc_cfg |= CFG_BL_LEN_512;

	/* Response timeout 256 clk */
	oc4_emmc_regs_mmc_cfg &= ~CFG_RESP_TIMEOUT_MASK;
	oc4_emmc_regs_mmc_cfg |= CFG_RESP_TIMEOUT_256;

	/* Command-command gap 16 clk */
	oc4_emmc_regs_mmc_cfg &= ~CFG_RC_CC_MASK;
	oc4_emmc_regs_mmc_cfg |= CFG_RC_CC_16;

	// oc4_emmc_regs_write_unsafe(mmc, oc4_emmc_regs_mmc_cfg, SD_EMMC_CFG);
	mmc->sd_emmc_cfg = oc4_emmc_regs_mmc_cfg;

	return 0;
}


result_t oc4_emmc_init(
        sdhci_regs_t *sdhci_regs,
        oc4_gpio_regs_t *oc4_gpio_regs
) {

    // sel4cp_dbg_puts("CLK: sd_emmc_cfg:\n\n");
    // puthex32(sdhci_regs->regs->sd_emmc_cfg);	        // 0x44 CFG
	// sel4cp_dbg_puts("\n\n");

    log_trace("\n\nTest log trace statement.\n\n");
    sel4cp_dbg_puts("\n\nTest sel4cp_dbg_puts statement.\n\n");
    printf("\n\nTest printf statement.\n\n");

    uint32_t val;

    oc4_emmc_regs_t *oc4_emmc_regs = sdhci_regs->regs;
    
    if (sdhci_regs == NULL) {
        sel4cp_dbg_puts("\n\nsdhci regs are null.");

        return result_err("NULL `sdhci_regs` passed to oc4_emmc_init().");
    }

    sel4cp_dbg_puts("\n\nCalled OC4 EMMC.3");

    // TODO: Fix this gpio reg stuff
    //? Apparently U-Boot code will do this already
    // if (oc4_gpio_regs == NULL) {
    //     return result_err("NULL `oc4_gpio_regs` passed to oc4_emmc_init().");
    // }

    // TODO: Zero out Registers

    //? To achieve something equivalent to the RPi specific things, we will also zero out all the "default values"
    //? With the hope that not all registers matter if they are set to 0

    //? This is generally not a good idea: see clock

    //! WARNING: this code does not work yet -- we should NOT reset the clock!////

    // log_trace("Resetting Clock.");
    // // oc4_emmc_regs_write_unsafe(oc4_emmc_regs, (uint32_t) 0, SD_EMMC_CLOCK);
    // // sel4cp_dbg_puts("\n\nFinished clock reset1");
    // // oc4_emmc_regs->sd_emmc_clock = (uint32_t) 0;
    // // sel4cp_dbg_puts("\n\nFinished clock reset2");

    // log_trace("Resetting Delay.");
    // // oc4_emmc_regs_write_unsafe(oc4_emmc_regs, (uint32_t) 0, SD_EMMC_DELAY1);
    // sel4cp_dbg_puts("\n\nFinished clock 1e3");
    // oc4_emmc_regs->sd_emmc_delay1 = (uint32_t) 0;
    // sel4cp_dbg_puts("\n\nFinished clock delay");


    // log_trace("Resetting Adjust.");
    // // oc4_emmc_regs_write_unsafe(oc4_emmc_regs, (uint32_t) 0, SD_EMMC_ADJUST);
    // oc4_emmc_regs->sd_emmc_adjust = (uint32_t) 0;
    // sel4cp_dbg_puts("\n\nFinished clock adjust");


    // log_trace("Resetting Start.");
    // // oc4_emmc_regs_write_unsafe(oc4_emmc_regs, (uint32_t) 0, SD_EMMC_START);
    // oc4_emmc_regs->sd_emmc_start = (uint32_t) 0;
    // sel4cp_dbg_puts("\n\nFinished clock start");


    // log_trace("Resetting Config.");
    // // oc4_emmc_regs_write_unsafe(oc4_emmc_regs, (uint32_t) 0, SD_EMMC_CFG);
    // oc4_emmc_regs->sd_emmc_cfg = (uint32_t) 0;
    // sel4cp_dbg_puts("\n\nFinished clock config");

    /* ============================
     * Initialising the SDHCI SD card controller on the Pi.
     * ============================ */

    log_trace("Configuring clock.");
	oc4_emmc_regs_mmc_config_clock(oc4_emmc_regs);

    log_trace("Setting I/Os.");
    oc4_emmc_regs_mmc_set_ios(oc4_emmc_regs);


    /* reset all status bits */
    log_trace("Resetting Status.");
    oc4_emmc_regs->sd_emmc_status = STATUS_MASK;
    sel4cp_dbg_puts("\n\nFinished resetting status");

    /* disable interrupts */
    log_trace("Disabling interrupts.");
    // oc4_emmc_regs_write_unsafe(oc4_emmc_regs, (uint32_t) 0, SD_EMMC_IRQ_EN);
    oc4_emmc_regs->sd_emmc_irq_en = (uint32_t) 0;
    sel4cp_dbg_puts("\n\nFinished resetting interrupts, config, start, adjust, delay and clock");

    /* enable auto clock mode */
    log_trace("Enabling Auto Clock Mode.");
    // val = oc4_emmc_regs_read_unsafe(oc4_emmc_regs, SD_EMMC_CFG);

    val = oc4_emmc_regs->sd_emmc_cfg;
    val &= ~CFG_SDCLK_ALWAYS_ON;
    val |= CFG_AUTO_CLK;
    // oc4_emmc_regs_write_unsafe(oc4_emmc_regs, val, SD_EMMC_CFG);
    oc4_emmc_regs->sd_emmc_cfg = val;

    // sel4cp_dbg_puts("CLK: sd_emmc_cfg:\n\n");
    // puthex32(sdhci_regs->regs->sd_emmc_cfg);	        // 0x44 CFG
	// sel4cp_dbg_puts("\n\n");
    
    sel4cp_dbg_puts("\n\nFinished enabling clock, auto clock and setting I/Os.\n\n");



    /* Set the Data Timeout to the maximum value. */
    //! This may not work!
    //! Even worse, removing this will screw up the device driver
    //! THIS WAS THE MAJOR SOURCE OF ERROR that cost me 3 weeks.
    result_t res = oc4_emmc_regs_set_max_data_timeout(oc4_emmc_regs);
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to set max data timeout in oc4_emmc_init().");
    }
    sel4cp_dbg_puts("\n\nFinished setting max data timeout.\n\n");
    

    // /* Enable the Internal Clock. */
    // res = sdhci_regs_enable_internal_clock(sdhci_regs);
    // if (result_is_err(res)) {
    //     return result_err_chain(res, "Failed to enable internal clock in bcm_emmc_init().");
    // }
    /* Wait 10 microseconds. */
    usleep(10);

    /* Set clock to low-speed setup frequency (400KHz). */
    //? Not a sdhci regs function!
    sel4cp_dbg_puts("\n\nFinished sleeping.\n\n");

    // log_trace("Setting clock to low-speed setup frequency (400KHz).");
    // res = sdhci_set_sd_clock(sdhci_regs, 400000);
    // if (result_is_err(res)) {
    //     return result_err_chain(res, "Failed to set clock to low-speed setup frequency in oc4_emmc_init().");
    // }
    //TODO: fix the sdhci set sd clock stuff
    sel4cp_dbg_puts("\n\nFinished setting sd clock.\n\n");

    // /* enable interrupts */
    // log_trace("Disabling interrupts.");
    // // oc4_emmc_regs_write_unsafe(oc4_emmc_regs, 0, SD_EMMC_IRQ_EN);
    // oc4_emmc_regs->sd_emmc_irq_en = 0;
    // sel4cp_dbg_puts("\n\nFinished Disabling interrupts");

    /* enable interrupts */
    // log_trace("Enabling interrupts.");
    // // oc4_emmc_regs_write_unsafe(oc4_emmc_regs, (uint32_t) 0, SD_EMMC_IRQ_EN);
    // oc4_emmc_regs->sd_emmc_irq_en = (uint32_t) IRQ_EN_MASK;

    sel4cp_dbg_puts("\n\nFinished setting up OC4 emmc. Now printing registers...\n\n");

    sel4cp_dbg_puts("\n\nCLK: sd_emmc_cfg:\n\n");
    puthex32(sdhci_regs->regs->sd_emmc_cfg);	        // 0x44 CFG
	sel4cp_dbg_puts("\n\n");
    

    return result_ok();
}
