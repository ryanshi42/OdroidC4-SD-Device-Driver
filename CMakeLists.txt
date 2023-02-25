
cmake_minimum_required(VERSION 3.7.2)

project(mmc-rpi3b C)

option(MMC_RPI3B_UNIT_TEST "Unit testing mode." OFF)

if (MMC_RPI3B_UNIT_TEST)
    # Add MMC_RPI3B_DEVELOPMENT compile definition.
    add_compile_definitions(MMC_RPI3B_UNIT_TEST)
endif()

add_subdirectory(mmc/)

# ====================================
# Enables unit testing
# ====================================

include (CTest)
# Enabling tests
enable_testing()
# Add our test directory.
add_subdirectory(mmc_test/)
