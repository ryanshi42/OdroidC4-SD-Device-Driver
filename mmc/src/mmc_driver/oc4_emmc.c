#include "oc4_emmc.h"
#include "oc4_emmc_regs.h"

/*

HIGH LEVEL OVERVIEW:

Stage 0: Write to all the registers

Stage 1: Read and write to a register, if not configured properly, then the register might not update

Stage 2: Read and write a disk block on the SD card

*/

// Get the base of the register structs
static inline void *get_regbase(const struct oc4_emmc_regs *mmc)
{
    // Or can be 0xFFE05000?
	return 0xFFE07000;
}

// Read a value in a certain register
static inline uint32_t oc4_mmc_regs_read_unsafe(struct oc4_emmc_regs *mmc, int offset)
{
	return readl(get_regbase(mmc) + offset);
}

// Write a value to a certain register
static inline void oc4_mmc_regs_write_unsafe(struct oc4_emmc_regs *mmc, uint32_t val, int offset)
{
	writel(val, get_regbase(mmc) + offset);
}

// Configure the clock
static void oc4_mmc_regs_mmc_config_clock(struct oc4_emmc_regs *mmc)
{
	uint32_t oc4_mmc_regs_mmc_clk = 0;
	unsigned int clk, clk_src, clk_div;

	if (!mmc->sd_emmc_clock)
		return;

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
	if (true) /* The Odroid C4 we are using uses the SM1 SoC so we should be fine here */
		oc4_mmc_regs_mmc_clk |= CLK_CO_PHASE_270;
	else
		oc4_mmc_regs_mmc_clk |= CLK_CO_PHASE_180;

	/* 180 phase tx sd_emmc_clock */
	oc4_mmc_regs_mmc_clk |= CLK_TX_PHASE_000;

	/* sd_emmc_clock settings */
	oc4_mmc_regs_mmc_clk |= clk_src;
	oc4_mmc_regs_mmc_clk |= clk_div;

	oc4_mmc_regs_write_unsafe(mmc, oc4_mmc_regs_mmc_clk, SD_EMMC_CLOCK);
}

// Set the IO configuration of the registers
static int oc4_mmc_regs_mmc_set_ios(struct oc4_emmc_regs *mmc)
{
	uint32_t oc4_mmc_regs_mmc_cfg = 0;

    // Read the config
	oc4_mmc_regs_mmc_cfg = oc4_mmc_regs_read_unsafe(mmc, SD_EMMC_CFG);

    // Get the bus width from the registers
    // Does this mean it is automatically instantiated??? // TODO: check this!
	oc4_mmc_regs_mmc_cfg &= ~CFG_BUS_WIDTH_MASK;
    oc4_mmc_regs_mmc_cfg |= CFG_BUS_WIDTH_4;
	// if (mmc->bus_width == 1)
	// 	oc4_mmc_regs_mmc_cfg |= CFG_BUS_WIDTH_1;
	// else if (mmc->bus_width == 4)
	// 	oc4_mmc_regs_mmc_cfg |= CFG_BUS_WIDTH_4;
	// else if (mmc->bus_width == 8)
	// 	oc4_mmc_regs_mmc_cfg |= CFG_BUS_WIDTH_8;
	// else
	// 	return -EINVAL;

    // The following are the default options
    // Some of these constants have been "redefined": // TODO: fix later!!!

	/* 512 bytes block length */
	oc4_mmc_regs_mmc_cfg &= ~CFG_BL_LEN_MASK;
	oc4_mmc_regs_mmc_cfg |= CFG_BL_LEN_512;

	/* Response timeout 256 clk */
	oc4_mmc_regs_mmc_cfg &= ~CFG_RESP_TIMEOUT_MASK;
	oc4_mmc_regs_mmc_cfg |= CFG_RESP_TIMEOUT_256;

	/* Command-command gap 16 clk */
	oc4_mmc_regs_mmc_cfg &= ~CFG_RC_CC_MASK;
	oc4_mmc_regs_mmc_cfg |= CFG_RC_CC_16;

	oc4_mmc_regs_write_unsafe(mmc, oc4_mmc_regs_mmc_cfg, SD_EMMC_CFG);

	return 0;
}

// TODO: Set up commands

// static void oc4_mmc_regs_mmc_setup_cmd(struct oc4_emmc_regs *mmc)
// {
// 	uint32_t oc4_mmc_regs_mmc_cmd = 0, cfg;

//     // This line seems to be useless, as it sets the cmd to be equal to the value that is already there
//     // Seems like a U-Boot specific implementation
// 	oc4_mmc_regs_mmc_cmd |= oc4_mmc_regs_read_unsafe(mmc, SD_EMMC_CMD_CFG | CMD_CFG_CMD_INDEX_SHIFT) << CMD_CFG_CMD_INDEX_SHIFT;


//     // NOTE: How to obtain information from MMC controller?
// 	if (cmd->resp_type & MMC_RSP_PRESENT) {
// 		if (cmd->resp_type & MMC_RSP_136)
// 			oc4_mmc_regs_mmc_cmd |= CMD_CFG_RESP_128;

// 		if (cmd->resp_type & MMC_RSP_BUSY)
// 			oc4_mmc_regs_mmc_cmd |= CMD_CFG_R1B;

// 		if (!(cmd->resp_type & MMC_RSP_CRC))
// 			oc4_mmc_regs_mmc_cmd |= CMD_CFG_RESP_NOCRC;
// 	} else {
// 		oc4_mmc_regs_mmc_cmd |= CMD_CFG_NO_RESP;
// 	}

// 	if (data) {
// 		cfg = oc4_mmc_regs_read_unsafe(mmc, SD_EMMC_CFG);
// 		cfg &= ~CFG_BL_LEN_MASK;
// 		cfg |= ilog2(data->blocksize) << CFG_BL_LEN_SHIFT;
// 		oc4_mmc_regs_write_unsafe(mmc, cfg, SD_EMMC_CFG);

// 		if (data->flags == MMC_DATA_WRITE)
// 			oc4_mmc_regs_mmc_cmd |= CMD_CFG_DATA_WR;

// 		oc4_mmc_regs_mmc_cmd |= CMD_CFG_DATA_IO | CMD_CFG_BLOCK_MODE | data->blocks;
// 	}

// 	oc4_mmc_regs_mmc_cmd |= CMD_CFG_TIMEOUT_4S | CMD_CFG_OWNER | CMD_CFG_END_OF_CHAIN;

// 	oc4_mmc_regs_write_unsafe(mmc, oc4_mmc_regs_mmc_cmd, SD_EMMC_CMD_CFG);
// }

// TODO: Setup Addresses
// static void oc4_mmc_regs_mmc_setup_addr(struct oc4_emmc_regs *mmc, struct oc4_emmc_regs_data *data)
// {
// 	struct oc4_mmc_regs_mmc_plat *pdata = mmc->priv;
// 	unsigned int data_size;
// 	uint32_t data_addr = 0;

// 	if (data) {
// 		data_size = data->blocks * data->blocksize;

// 		if (data->flags == MMC_DATA_READ) {
// 			data_addr = (ulong) data->dest;
// 			invalidate_dcache_range(data_addr,
// 						data_addr + data_size);
// 		} else {
// 			pdata->w_buf = calloc(data_size, sizeof(char));
// 			data_addr = (ulong) pdata->w_buf;
// 			memcpy(pdata->w_buf, data->src, data_size);
// 			flush_dcache_range(data_addr, data_addr + data_size);
// 		}
// 	}

// 	oc4_mmc_regs_write_unsafe(mmc, data_addr, SD_EMMC_CMD_DAT);
// }

// TODO: Read responses
// static void oc4_mmc_regs_mmc_read_response(struct oc4_emmc_regs *mmc, struct oc4_emmc_regs_cmd *cmd)
// {
// 	if (cmd->resp_type & MMC_RSP_136) {
// 		cmd->response[0] = oc4_mmc_regs_read_unsafe(mmc, SD_EMMC_CMD_RSP3);
// 		cmd->response[1] = oc4_mmc_regs_read_unsafe(mmc, SD_EMMC_CMD_RSP2);
// 		cmd->response[2] = oc4_mmc_regs_read_unsafe(mmc, SD_EMMC_CMD_RSP1);
// 		cmd->response[3] = oc4_mmc_regs_read_unsafe(mmc, SD_EMMC_CMD_RSP);
// 	} else {
// 		cmd->response[0] = oc4_mmc_regs_read_unsafe(mmc, SD_EMMC_CMD_RSP);
// 	}
// }

// TODO: Send commands
// static int oc4_mmc_regs_dm_mmc_send_cmd(struct udevice *dev, struct oc4_emmc_regs_cmd *cmd,
// 				 struct oc4_emmc_regs_data *data)
// {
// 	struct oc4_emmc_regs *mmc = mmc_get_mmc_dev(dev);
// 	struct oc4_mmc_regs_mmc_plat *pdata = mmc->priv;
// 	uint32_t status;
// 	ulong start;
// 	int ret = 0;

// 	/* max block size supported by chip is 512 byte */
// 	if (data && data->blocksize > 512)
// 		return -EINVAL;

// 	oc4_mmc_regs_mmc_setup_cmd(mmc, data, cmd);
// 	oc4_mmc_regs_mmc_setup_addr(mmc, data);

// 	oc4_mmc_regs_write_unsafe(mmc, cmd->cmdarg, SD_EMMC_CMD_ARG);

// 	/* use 10s timeout */
// 	start = get_timer(0);
// 	do {
// 		status = oc4_mmc_regs_read_unsafe(mmc, SD_EMMC_STATUS);
// 	} while(!(status & STATUS_END_OF_CHAIN) && get_timer(start) < 10000);

// 	if (!(status & STATUS_END_OF_CHAIN))
// 		ret = -ETIMEDOUT;
// 	else if (status & STATUS_RESP_TIMEOUT)
// 		ret = -ETIMEDOUT;
// 	else if (status & STATUS_ERR_MASK)
// 		ret = -EIO;

// 	oc4_mmc_regs_mmc_read_response(mmc, cmd);

// 	if (data && data->flags == MMC_DATA_WRITE)
// 		free(pdata->w_buf);

// 	/* reset status bits */
// 	oc4_mmc_regs_write_unsafe(mmc, STATUS_MASK, SD_EMMC_STATUS);

// 	return ret;
// }

// TODO: Set up Operations
// static const struct dm_mmc_ops oc4_mmc_regs_dm_mmc_ops = {
// 	.send_cmd = oc4_mmc_regs_dm_mmc_send_cmd,
// 	.set_ios = oc4_mmc_regs_dm_mmc_set_ios,
// };

// TODO: ???
// static int oc4_mmc_regs_mmc_of_to_plat(struct udevice *dev)
// {
// 	struct oc4_mmc_regs_mmc_plat *pdata = dev_get_plat(dev);
// 	fdt_addr_t addr;

// 	addr = dev_read_addr(dev);
// 	if (addr == FDT_ADDR_T_NONE)
// 		return -EINVAL;

// 	pdata->regbase = (void *)addr;

// 	return 0;
// }

// TODO: Set some probe stuff
// static int oc4_mmc_regs_mmc_probe()
// {
// 	struct oc4_emmc_regs *mmc = &pdata->mmc;
// 	struct oc4_emmc_regs_config *cfg = &pdata->cfg;
// 	struct clk_bulk clocks;
// 	uint32_t val;
// 	int ret;

// 	/* Enable the clocks feeding the MMC controller */
// 	ret = clk_get_bulk(dev, &clocks);
// 	if (ret)
// 		return ret;

// 	ret = clk_enable_bulk(&clocks);
// 	if (ret)
// 		return ret;

// 	cfg->voltages = MMC_VDD_33_34 | MMC_VDD_32_33 |
// 			MMC_VDD_31_32 | MMC_VDD_165_195;
// 	cfg->host_caps = MMC_MODE_8BIT | MMC_MODE_4BIT |
// 			MMC_MODE_HS_52MHz | MMC_MODE_HS;
// 	cfg->f_min = div_round_up(SD_EMMC_CLKSRC_24M, CLK_MAX_DIV);
// 	cfg->f_max = 100000000; /* 100 MHz */
// 	cfg->b_max = 511; /* max 512 - 1 blocks */
// 	cfg->name = dev->name;

// 	mmc->priv = pdata;
// 	upriv->mmc = mmc;

// 	mmc_set_clock(mmc, cfg->f_min, MMC_CLK_ENABLE);

// #ifdef CONFIG_MMC_PWRSEQ
// 	/* Enable power if needed */
// 	ret = mmc_pwrseq_get_power(dev, cfg);
// 	if (!ret) {
// 		ret = pwrseq_set_power(cfg->pwr_dev, true);
// 		if (ret)
// 			return ret;
// 	}
// #endif

// 	/* reset all status bits */
// 	oc4_mmc_regs_write_unsafe(mmc, STATUS_MASK, SD_EMMC_STATUS);

// 	/* disable interrupts */
// 	oc4_mmc_regs_write_unsafe(mmc, 0, SD_EMMC_IRQ_EN);

// 	/* enable auto clock mode */
// 	val = oc4_mmc_regs_read_unsafe(mmc, SD_EMMC_CFG);
// 	val &= ~CFG_SDCLK_ALWAYS_ON;
// 	val |= CFG_AUTO_CLK;
// 	oc4_mmc_regs_write_unsafe(mmc, val, SD_EMMC_CFG);

// 	return 0;
// }

// int oc4_mmc_regs_mmc_bind(struct udevice *dev)
// {
// 	struct oc4_mmc_regs_mmc_plat *pdata = dev_get_plat(dev);

// 	return mmc_bind(dev, &pdata->mmc, &pdata->cfg);
// }

// static const struct udevice_id oc4_mmc_regs_mmc_match[] = {
// 	{ .compatible = "amlogic,oc4_mmc_regs-gx-mmc", .data = MMC_COMPATIBLE_GX },
// 	{ .compatible = "amlogic,oc4_mmc_regs-axg-mmc", .data = MMC_COMPATIBLE_GX },
// 	{ .compatible = "amlogic,oc4_mmc_regs-sm1-mmc", .data = MMC_COMPATIBLE_SM1 },
// 	{ /* sentinel */ }
// };


result_t oc4_emmc_init(
        sdhci_regs_t *sdhci_regs,
        oc4_gpio_regs_t *oc4_gpio_regs
) {
    uint32_t val;
    
    if (sdhci_regs == NULL) {
        return result_err("NULL `sdhci_regs` passed to oc4_emmc_init().");
    }
    // TODO: Fix this gpio reg stuff
    // if (oc4_gpio_regs == NULL) {
    //     return result_err("NULL `oc4_gpio_regs` passed to oc4_emmc_init().");
    // }

    /* ============================
     * Initialising the SDHCI SD card controller on the OC4.
     * ============================ */

    /* reset all status bits */
    log_trace("Resetting all status bits.");
    oc4_mmc_regs_write_unsafe(mmc, STATUS_MASK, SD_EMMC_STATUS);

    /* disable interrupts */
    log_trace("Disabling interrupts.");
    oc4_mmc_regs_write_unsafe(mmc, 0, SD_EMMC_IRQ_EN);

    /* enable auto clock mode */
    log_trace("Enabling Auto Clock Mode.");
    val = oc4_mmc_regs_read_unsafe(mmc, SD_EMMC_CFG);
    val &= ~CFG_SDCLK_ALWAYS_ON;
    val |= CFG_AUTO_CLK;
    oc4_mmc_regs_write_unsafe(mmc, val, SD_EMMC_CFG);

    log_trace("Configuring clock.");
	oc4_mmc_regs_mmc_config_clock(mmc);

    log_trace("Setting I/Os.");
    oc4_mmc_regs_dm_mmc_set_ios(mmc);

    // TODO: Some resistor stuff

    /* Set the Data Timeout to the maximum value. */
    res = sdhci_regs_set_max_data_timeout(sdhci_regs);
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to set max data timeout in oc4_emmc_init().");
    }
    /* Enable the Internal Clock. */
    res = sdhci_regs_enable_internal_clock(sdhci_regs);
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to enable internal clock in oc4_emmc_init().");
    }
    /* Wait 10 microseconds. */
    usleep(10);

    /* Set clock to low-speed setup frequency (400KHz). */
    log_trace("Setting clock to low-speed setup frequency (400KHz).");
    res = sdhci_set_sd_clock(sdhci_regs, 400000);
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to set clock to low-speed setup frequency in oc4_emmc_init().");
    }

    /* Enable interrupts. */
    log_trace("Enabling interrupts.");
    res = sdhci_regs_enable_interrupts(sdhci_regs);
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to enable interrupts in oc4_emmc_init().");
    }

    /* enable interrupts */
    log_trace("Enabling interrupts.");
    oc4_mmc_regs_write_unsafe(mmc, 0, SD_EMMC_IRQ_EN);

    return result_ok();
}
