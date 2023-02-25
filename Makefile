
BUILD_DIR := build
BUILD_UNIT_TEST_DIR := build-unit-test
RESOURCES_DIR := resources

.PHONY: directories
directories:
	mkdir -p $(BUILD_UNIT_TEST_DIR)

# =================================
# Resources
# =================================

# Command used to decompile the Raspberry Pi 3B+'s device tree blob.
.PHONY: resources-decompile-dtb-rpi3bp
resources-decompile-dtb-rpi3bp:
	dtc \
		-I dtb \
		-O dts \
		$(RESOURCES_DIR)/bcm2710-rpi-3-b-plus.dtb \
		-o $(RESOURCES_DIR)/bcm2710-rpi-3-b-plus.dts

# =================================
# Testing
# =================================

.PHONY: test-build
test-build: directories
	cmake -S . -B ./$(BUILD_UNIT_TEST_DIR) -DMMC_RPI3B_UNIT_TEST=1
	cmake --build ./$(BUILD_UNIT_TEST_DIR) --parallel "$(nproc)"

.PHONY: test-unit
test-unit: test-build
	cmake --build ./$(BUILD_UNIT_TEST_DIR) --parallel "$(nproc)" --target test

.PHONY: test-unit-valgrind
test-unit-valgrind: test-build
	cd ./$(BUILD_UNIT_TEST_DIR) && ctest -T memcheck

