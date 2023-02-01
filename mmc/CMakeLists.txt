
cmake_minimum_required(VERSION 3.7.2)

project(mmc-rpi3b C)

add_library(mmc_rpi3b_lib_fence INTERFACE)
target_include_directories(mmc_rpi3b_lib_fence INTERFACE include/)

add_library(mmc_rpi3b_lib_shared_dma src/shared_dma.c)
target_include_directories(mmc_rpi3b_lib_shared_dma PUBLIC include/)

add_library(mmc_rpi3b_lib_shared_ringbuffer src/libsharedringbuffer/shared_ringbuffer.c)
target_include_directories(mmc_rpi3b_lib_shared_ringbuffer PUBLIC src/libsharedringbuffer/include/)
target_link_libraries(
        mmc_rpi3b_lib_shared_ringbuffer
        lib_sel4cp_rpi3b
        mmc_rpi3b_lib_fence
)

add_library(mmc_rpi3b_lib_arith src/arith.c)
target_include_directories(mmc_rpi3b_lib_arith PUBLIC include/)

add_library(mmc_rpi3b_lib_serial_parity src/serial_parity.c)
target_include_directories(mmc_rpi3b_lib_serial_parity PUBLIC include/)

add_library(mmc_rpi3b_lib_bcm_uart_regs src/bcm_uart_regs.c)
target_include_directories(mmc_rpi3b_lib_bcm_uart_regs PUBLIC include/)
target_link_libraries(
        mmc_rpi3b_lib_bcm_uart_regs
        lib_sel4cp_rpi3b
        mmc_rpi3b_lib_serial_parity
        mmc_rpi3b_lib_arith
)

add_library(mmc_rpi3b_lib_bcm_uart src/bcm_uart.c)
target_include_directories(mmc_rpi3b_lib_bcm_uart PUBLIC include/)
target_link_libraries(
        mmc_rpi3b_lib_bcm_uart
        lib_sel4cp_rpi3b
        mmc_rpi3b_lib_bcm_uart_regs
)

add_library(mmc_rpi3b_lib_serial_driver src/serial_driver.c)
target_include_directories(mmc_rpi3b_lib_serial_driver PUBLIC include/)
target_link_libraries(
        mmc_rpi3b_lib_serial_driver
        lib_sel4cp_rpi3b
        mmc_rpi3b_lib_bcm_uart
        mmc_rpi3b_lib_shared_ringbuffer
)

add_library(mmc_rpi3b_lib_serial_client src/serial_client.c)
target_include_directories(mmc_rpi3b_lib_serial_client PUBLIC include/)
target_link_libraries(
        mmc_rpi3b_lib_serial_client
        lib_sel4cp_rpi3b
        mmc_rpi3b_lib_shared_ringbuffer
        mmc_rpi3b_lib_shared_dma
)

add_library(mmc_rpi3b_lib_gpio src/gpio.c)
target_include_directories(mmc_rpi3b_lib_gpio PUBLIC include/)
target_link_libraries(
        mmc_rpi3b_lib_gpio
)

add_library(mmc_rpi3b_lib_delays src/delays.c)
target_include_directories(mmc_rpi3b_lib_delays PUBLIC include/)
target_link_libraries(
        mmc_rpi3b_lib_delays
        mmc_rpi3b_lib_gpio
)

add_library(mmc_rpi3b_lib_uart src/uart.c)
target_include_directories(mmc_rpi3b_lib_uart PUBLIC include/)
target_link_libraries(
        mmc_rpi3b_lib_uart
)

add_library(mmc_rpi3b_lib_sd src/sd.c)
target_include_directories(mmc_rpi3b_lib_sd PUBLIC include/)
target_link_libraries(
        mmc_rpi3b_lib_sd
        mmc_pri3b_lib_gpio
)

