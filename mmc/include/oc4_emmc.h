
#pragma once

#include "result.h"
#include "sdhci_regs.h"
#include "sleep.h"
#include "sdhci.h"
#include "log.h"
#include "sdhci_result.h"
#include "oc4_gpio_regs.h"
#include "gpio_driver.h"

#define CLK_OFFSET1     0x140
#define CLK_OFFSET2     0x0c0
#define SD_CLK81_BIT    BIT(26)

// uintptr_t clk_vaddr;

/**
 * Initialises the oc4 EMMC device.
 * @param sdhci_regs
 * @param oc4_gpio_regs
 * @return
 */
result_t oc4_emmc_init(
        sdhci_regs_t *sdhci_regs,
        oc4_gpio_regs_t *oc4_gpio_regs
);


// Get the base of the register structs
static inline void *get_regbase(const struct oc4_emmc_regs *mmc)
{
    // Or can be 0xFFE05000?
	return (void *) 0xFFE05000;
}

// Read a value in a certain register
static inline uint32_t oc4_emmc_regs_read_unsafe(struct oc4_emmc_regs *mmc, int offset)
{
    // return readl(get_regbase(mmc) + offset);
	uint32_t *addr = (uint32_t *)(mmc + offset);
    return *addr;
}

// Write a value to a certain register
// Note is uint32_t
static inline void oc4_emmc_regs_write_unsafe(struct oc4_emmc_regs *mmc, uint32_t val, int offset)
{
	// writel(val, get_regbase(mmc) + offset);
    // sel4cp_dbg_puts("\n\n inside write unsafe\n\n");
    uint32_t *addr = (uint32_t *)(mmc + offset * 8);

    if (addr == NULL) {
        sel4cp_dbg_puts("\n\n null address passed to write unsafe\n\n");
    }

    // sel4cp_dbg_puts("\n\n inside write unsafe2\n\n");
    printf("\n\n trying to write val %d to offset %d, address %p\n\n", val, offset, addr);

    *addr = val;
    // sel4cp_dbg_puts("\n\n inside write unsafe3\n\n");
    return;
}