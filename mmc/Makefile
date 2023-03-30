#
# Copyright 2022, UNSW
#
# SPDX-License-Identifier: BSD-2-Clause
#

ifeq ($(strip $(BUILD_DIR)),)
$(error BUILD_DIR must be specified)
endif

ifeq ($(strip $(SEL4CP_SDK)),)
$(error SEL4CP_SDK must be specified)
endif

ifeq ($(strip $(SEL4CP_BOARD)),)
$(error SEL4CP_BOARD must be specified)
endif

ifeq ($(strip $(SEL4CP_CONFIG)),)
$(error SEL4CP_CONFIG must be specified)
endif

TOOLCHAIN := aarch64-none-elf

CPU := cortex-a53

CC := $(TOOLCHAIN)-gcc
LD := $(TOOLCHAIN)-ld
AS := $(TOOLCHAIN)-as
SEL4CP_TOOL ?= $(SEL4CP_SDK)/bin/sel4cp

# Specifies where the source files are.
SRC_DIR := src
# Specifies where the ring buffer files are.
LIB_SHARED_RINGBUFFER_DIR := $(SRC_DIR)/libsharedringbuffer
LIB_PRINTF_DIR := $(SRC_DIR)/lib_printf
LIB_SLEEP_DIR := $(SRC_DIR)/lib_sleep
LIB_ASSERT_DIR := $(SRC_DIR)/lib_assert
LIB_CLOCK_DIR := $(SRC_DIR)/lib_clock
LIB_RESULT_DIR := $(SRC_DIR)/lib_result
LIB_FATFS_DIR := $(SRC_DIR)/lib_fatfs

BOARD_DIR := $(SEL4CP_SDK)/board/$(SEL4CP_BOARD)/$(SEL4CP_CONFIG)

CFLAGS := \
	-mcpu=$(CPU) \
	-mstrict-align \
	-ffreestanding \
	-g3 \
	-O3 \
	-Wall \
	-Werror \
	-Wno-unused-function
LDFLAGS := -L$(BOARD_DIR)/lib
LIBS := -lsel4cp -Tsel4cp.ld -lc

IMAGE_FILE = $(BUILD_DIR)/loader.img
REPORT_FILE = $(BUILD_DIR)/report.txt

CFLAGS += \
	-I$(BOARD_DIR)/include \
	-Iinclude \
	-I$(LIB_SHARED_RINGBUFFER_DIR)/include \
	-I$(LIB_PRINTF_DIR) \
	-I$(LIB_SLEEP_DIR) \
	-I$(LIB_ASSERT_DIR) \
	-I$(LIB_CLOCK_DIR) \
	-I$(LIB_RESULT_DIR) \
	-I$(LIB_FATFS_DIR) \

# Specifies which Object files in $(BUILD_DIR) should be linked.
SERIAL_DRIVER_OBJS := \
	serial_driver.o \
	serial_driver/bcm_uart_regs.o \
	serial_driver/bcm_uart.o \
	serial_driver/serial_parity.o \
	common/arith.o \
	libsharedringbuffer/shared_ringbuffer.o \
	common/shared_dma.o \

SERIAL_CLIENT_OBJS := \
	serial_client.o \
	libsharedringbuffer/shared_ringbuffer.o \
	common/shared_dma.o \

MMC_DRIVER_OBJS := \
	mmc_driver.o \
	e2e_test.o \
	mmc_driver/bcm_emmc.o \
	mmc_driver/bcm_emmc_regs.o \
	mmc_driver/sdcard.o \
	mmc_driver/sdcard/ocr.o \
	mmc_driver/sdcard/cid.o \
	mmc_driver/sdcard/scr.o \
	mmc_driver/sdcard/csd.o \
	mmc_driver/sdhci.o \
	mmc_driver/sdhci/sdhci_result.o \
	mmc_driver/sdhci/sdhci_cmd.o \
	mmc_driver/sdhci/sdhci_cmds.o \
	mmc_driver/sdhci/sdhci_resp_r1.o \
	mmc_driver/sdhci/sdhci_resp_r6.o \
	mmc_driver/bcm_emmc_regs/control0.o \
	mmc_driver/bcm_emmc_regs/control1.o \
	mmc_driver/bcm_emmc_regs/irpt_en.o \
	mmc_driver/bcm_emmc_regs/irpt_mask.o \
	mmc_driver/bcm_emmc_regs/status.o \
	mmc_driver/bcm_emmc_regs/slotisr_ver.o \
	mmc_driver/bcm_emmc_regs/cmdtm.o \
	mmc_driver/bcm_emmc_regs/cmdtm/cmd_index.o \
	mmc_driver/bcm_emmc_regs/cmdtm/cmd_rspns_type.o \
	mmc_driver/bcm_emmc_regs/interrupt.o \
	mmc_driver/bcm_emmc_regs/blksizecnt.o \
	lib_printf/printf.o \
	lib_sleep/sleep.o \
	lib_assert/assert.o \
	lib_clock/clock.o \
	timer_driver/bcm_timer.o \
	timer_driver/bcm_timer_regs.o \
	common/arith.o \
	lib_result/result.o \
	lib_result/result_prod.o \
	common/timer_client.o \
	common/shared_dma.o \
	common/log.o \
	libsharedringbuffer/shared_ringbuffer.o \
	gpio_driver.o \
	gpio_driver/bcm_gpio_regs.o \
	gpio_driver/gpio_mode.o \
	gpio_driver/gpio_fix_resistor.o \
	lib_fatfs/diskio.o \
	lib_fatfs/ff.o \
	lib_fatfs/diskio_test.o \

TIMER_DRIVER_OBJS := \
	timer_driver.o \
	libsharedringbuffer/shared_ringbuffer.o \
	lib_result/result.o \
	lib_result/result_prod.o \
	timer_driver/bcm_timer.o \
	timer_driver/bcm_timer_regs.o \
	lib_printf/printf.o \

IMAGES := \
	serial_driver.elf \
	serial_client.elf \
	mmc_driver.elf \
	timer_driver.elf \

.PHONY: all
all: directories $(IMAGE_FILE)

# Create build directory
directories:
	mkdir -p $(BUILD_DIR)
	mkdir -p $(BUILD_DIR)/libsharedringbuffer
	mkdir -p $(BUILD_DIR)/lib_printf
	mkdir -p $(BUILD_DIR)/lib_sleep
	mkdir -p $(BUILD_DIR)/lib_assert
	mkdir -p $(BUILD_DIR)/lib_clock
	mkdir -p $(BUILD_DIR)/lib_result
	mkdir -p $(BUILD_DIR)/lib_fatfs
	mkdir -p $(BUILD_DIR)/common
	mkdir -p $(BUILD_DIR)/mmc_driver
	mkdir -p $(BUILD_DIR)/mmc_driver/sdhci
	mkdir -p $(BUILD_DIR)/mmc_driver/bcm_emmc_regs
	mkdir -p $(BUILD_DIR)/mmc_driver/bcm_emmc_regs/cmdtm
	mkdir -p $(BUILD_DIR)/mmc_driver/sdcard
	mkdir -p $(BUILD_DIR)/serial_driver
	mkdir -p $(BUILD_DIR)/timer_driver
	mkdir -p $(BUILD_DIR)/gpio_driver

# ======================================
# Step 1: Generate $(BUILD_DIR)/%.o object files from $(SRC_DIR)/%.c source
# files and $(SRC_DIR)/%.c assembler files.
# ======================================

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c Makefile
	$(CC) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.s Makefile
	$(AS) -g3 -mcpu=$(CPU) $< -o $@

# ======================================
# Step 2: Generate our $(BUILD_DIR)/serial_driver.elf ELF file using each
# $(SERIAL_DRIVER_OBJS) object file in the $(BUILD_DIR).
# ======================================

$(BUILD_DIR)/serial_driver.elf: $(addprefix $(BUILD_DIR)/, $(SERIAL_DRIVER_OBJS))
	$(LD) $(LDFLAGS) $^ $(LIBS) -o $@

$(BUILD_DIR)/serial_client.elf: $(addprefix $(BUILD_DIR)/, $(SERIAL_CLIENT_OBJS))
	$(LD) $(LDFLAGS) $^ $(LIBS) -o $@

$(BUILD_DIR)/mmc_driver.elf: $(addprefix $(BUILD_DIR)/, $(MMC_DRIVER_OBJS))
	$(LD) $(LDFLAGS) $^ $(LIBS) -o $@

$(BUILD_DIR)/timer_driver.elf: $(addprefix $(BUILD_DIR)/, $(TIMER_DRIVER_OBJS))
	$(LD) $(LDFLAGS) $^ $(LIBS) -o $@

# ======================================
# Step 3: Generate our $(BUILD_DIR)/loader.img using the $(SEL4CP_TOOL).
# ======================================

$(IMAGE_FILE) $(REPORT_FILE): $(addprefix $(BUILD_DIR)/, $(IMAGES)) mmc.system
	$(SEL4CP_TOOL) \
		  mmc.system \
		  --search-path $(BUILD_DIR) \
		  --board $(SEL4CP_BOARD) \
		  --config $(SEL4CP_CONFIG) \
		  -o $(IMAGE_FILE) \
		  -r $(REPORT_FILE)

.PHONY: clean
clean:
	rm -f *.o *.elf .depend*
	find . -name \*.o |xargs --no-run-if-empty rm

