
RESOURCES_DIR := resources

# Command used to decompile the Raspberry Pi 3B+'s device tree blob.
.PHONY: resources-decompile-dtb-rpi3bp
resources-decompile-dtb-rpi3bp:
	dtc \
		-I dtb \
		-O dts \
		$(RESOURCES_DIR)/bcm2710-rpi-3-b-plus.dtb \
		-o $(RESOURCES_DIR)/bcm2710-rpi-3-b-plus.dts
