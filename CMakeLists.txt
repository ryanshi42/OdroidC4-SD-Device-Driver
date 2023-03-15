
cmake_minimum_required(VERSION 3.7.2)

project(mmc-rpi3b C)

# Fix behavior of CMAKE_C_STANDARD when targeting macOS.
if (POLICY CMP0025)
    cmake_policy(SET CMP0025 NEW)
endif ()
set(CMAKE_C_STANDARD 11)

# Using most flags in src/Makefile.
set(GCC_COMPILE_FLAGS -mstrict-align -Wall -Werror -Wvla -g3 -O3)

SET(CMAKE_BUILD_TYPE Debug)

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
