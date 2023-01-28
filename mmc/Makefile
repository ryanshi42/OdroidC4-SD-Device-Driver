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
SHARED_RING_BUFFER_DIR := $(SRC_DIR)/libsharedringbuffer

BOARD_DIR := $(SEL4CP_SDK)/board/$(SEL4CP_BOARD)/$(SEL4CP_CONFIG)

IMAGES := serial_driver.elf \
			serial_client.elf
CFLAGS := -mcpu=$(CPU) -mstrict-align -ffreestanding -g3 -O3 -Wall  -Wno-unused-function
LDFLAGS := -L$(BOARD_DIR)/lib
LIBS := -lsel4cp -Tsel4cp.ld -lc

IMAGE_FILE = $(BUILD_DIR)/loader.img
REPORT_FILE = $(BUILD_DIR)/report.txt

CFLAGS += \
	-I$(BOARD_DIR)/include \
	-Iinclude \
	-I$(SHARED_RING_BUFFER_DIR)/include \

# Specifies which Object files in $(BUILD_DIR) should be linked.
SERIAL_DRIVER_OBJS := \
	serial_driver.o \
	bcm_uart_regs.o \
	bcm_uart.o \
	serial_parity.o \
	arith.o \
	libsharedringbuffer/shared_ringbuffer.o \
	shared_dma.o \

SERIAL_CLIENT_OBJS := \
	serial_client.o \
	libsharedringbuffer/shared_ringbuffer.o \
	shared_dma.o \

.PHONY: all
all: directories $(IMAGE_FILE)

# Create build directory
directories:
	mkdir -p $(BUILD_DIR)
	mkdir -p $(BUILD_DIR)/libsharedringbuffer

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

# ======================================
# Step 3: Generate our $(BUILD_DIR)/loader.img using the $(SEL4CP_TOOL).
# ======================================

$(IMAGE_FILE) $(REPORT_FILE): $(addprefix $(BUILD_DIR)/, $(IMAGES)) serial.system
	$(SEL4CP_TOOL) \
		  serial.system \
		  --search-path $(BUILD_DIR) \
		  --board $(SEL4CP_BOARD) \
		  --config $(SEL4CP_CONFIG) \
		  -o $(IMAGE_FILE) \
		  -r $(REPORT_FILE)

.PHONY: clean
clean:
	rm -f *.o *.elf .depend*
	find . -name \*.o |xargs --no-run-if-empty rm

