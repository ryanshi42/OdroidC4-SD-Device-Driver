

add_library(mmc_rpi3b_lib_sel4cp_facade src/common/sel4cp_facade.c)
target_include_directories(mmc_rpi3b_lib_sel4cp_facade PUBLIC include/)
if (NOT MMC_RPI3B_UNIT_TEST)
    target_link_libraries(
            mmc_rpi3b_lib_sel4cp_facade
            lib_sel4cp_rpi3b
    )
endif ()

add_library(mmc_rpi3b_lib_fence INTERFACE)
target_include_directories(mmc_rpi3b_lib_fence INTERFACE include/)

add_library(mmc_rpi3b_lib_shared_dma src/common/shared_dma.c)
target_include_directories(mmc_rpi3b_lib_shared_dma PUBLIC include/)

add_library(mmc_rpi3b_lib_printf src/lib_printf/printf.c)
target_include_directories(mmc_rpi3b_lib_printf PUBLIC src/lib_printf/)
target_link_libraries(
        mmc_rpi3b_lib_printf
        mmc_rpi3b_lib_sel4cp_facade
)

add_library(mmc_rpi3b_lib_log src/common/log.c)
target_include_directories(mmc_rpi3b_lib_log PUBLIC src/lib_log/)
target_link_libraries(
        mmc_rpi3b_lib_log
        mmc_rpi3b_lib_printf
)

add_library(mmc_rpi3b_lib_assert src/lib_assert/assert.c)
target_include_directories(mmc_rpi3b_lib_assert PUBLIC include/)
target_link_libraries(
        mmc_rpi3b_lib_assert
        lib_sel4cp_rpi3b
)


add_library(mmc_rpi3b_lib_delays src/delays.c)
target_include_directories(mmc_rpi3b_lib_delays PUBLIC include/)
target_link_libraries(
        mmc_rpi3b_lib_delays
        mmc_rpi3b_lib_gpio
)

add_library(mmc_rpi3b_lib_time src/lib_time/time.c)
target_include_directories(mmc_rpi3b_lib_time PUBLIC src/lib_time/)
target_link_libraries(
        mmc_rpi3b_lib_time
        mmc_rpi3b_lib_delays
)

add_library(mmc_rpi3b_lib_sleep src/lib_sleep/sleep.c)
target_include_directories(mmc_rpi3b_lib_sleep PUBLIC src/lib_sleep/)
target_link_libraries(
        mmc_rpi3b_lib_sleep
        mmc_rpi3b_lib_timer_client
        mmc_rpi3b_lib_result
)

add_library(mmc_rpi3b_lib_shared_ringbuffer src/libsharedringbuffer/shared_ringbuffer.c)
target_include_directories(mmc_rpi3b_lib_shared_ringbuffer PUBLIC src/libsharedringbuffer/include/)
target_link_libraries(
        mmc_rpi3b_lib_shared_ringbuffer
        mmc_rpi3b_lib_fence
)

add_library(mmc_rpi3b_lib_arith src/common/arith.c)
target_include_directories(mmc_rpi3b_lib_arith PUBLIC include/)

add_library(mmc_rpi3b_lib_timer_client src/common/timer_client.c)
target_include_directories(mmc_rpi3b_lib_timer_client PUBLIC include/)
target_link_libraries(
        mmc_rpi3b_lib_timer_client
        mmc_rpi3b_lib_shared_ringbuffer
        mmc_rpi3b_lib_shared_dma
        mmc_rpi3b_lib_result
        mmc_rpi3b_lib_sel4cp_facade
)

add_library(mmc_rpi3b_lib_bcm_timer_regs src/timer_driver/bcm_timer_regs.c)
target_include_directories(mmc_rpi3b_lib_bcm_timer_regs PUBLIC include/)
target_link_libraries(
        mmc_rpi3b_lib_bcm_timer_regs
        mmc_rpi3b_lib_result
)

add_library(mmc_rpi3b_lib_bcm_timer src/timer_driver/bcm_timer.c)
target_include_directories(mmc_rpi3b_lib_bcm_timer PUBLIC include/)
target_link_libraries(
        mmc_rpi3b_lib_bcm_timer
        mmc_rpi3b_lib_bcm_timer_regs
        mmc_rpi3b_lib_result
)

add_library(mmc_rpi3b_lib_timer_driver src/timer_driver.c)
target_include_directories(mmc_rpi3b_lib_timer_driver PUBLIC include/)
target_link_libraries(
        mmc_rpi3b_lib_timer_driver
        mmc_rpi3b_lib_sel4cp_facade
        mmc_rpi3b_lib_result
        mmc_rpi3b_lib_bcm_timer
        mmc_rpi3b_lib_shared_ringbuffer
        mmc_rpi3b_lib_printf
)

add_library(mmc_rpi3b_lib_serial_parity src/serial_driver/serial_parity.c)
target_include_directories(mmc_rpi3b_lib_serial_parity PUBLIC include/)

add_library(mmc_rpi3b_lib_bcm_uart_regs src/serial_driver/bcm_uart_regs.c)
target_include_directories(mmc_rpi3b_lib_bcm_uart_regs PUBLIC include/)
target_link_libraries(
        mmc_rpi3b_lib_bcm_uart_regs
        lib_sel4cp_rpi3b
        mmc_rpi3b_lib_serial_parity
        mmc_rpi3b_lib_arith
)

add_library(mmc_rpi3b_lib_bcm_uart src/serial_driver/bcm_uart.c)
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
        mmc_rpi3b_sel4cp_facade
        mmc_rpi3b_lib_bcm_uart
        mmc_rpi3b_lib_shared_ringbuffer
)

add_library(mmc_rpi3b_lib_serial_client src/serial_client.c)
target_include_directories(mmc_rpi3b_lib_serial_client PUBLIC include/)
target_link_libraries(
        mmc_rpi3b_lib_serial_client
        mmc_rpi3b_lib_shared_ringbuffer
        mmc_rpi3b_lib_shared_dma
        mmc_rpi3b_lib_sel4cp_facade
)

add_library(mmc_rpi3b_lib_gpio src/gpio.c)
target_include_directories(mmc_rpi3b_lib_gpio PUBLIC include/)
target_link_libraries(
        mmc_rpi3b_lib_gpio
)

add_library(mmc_rpi3b_lib_uart src/uart.c)
target_include_directories(mmc_rpi3b_lib_uart PUBLIC include/)
target_link_libraries(
        mmc_rpi3b_lib_uart
        mmc_rpi3b_lib_printf
)

add_library(mmc_rpi3b_lib_result src/common/result.c)
target_include_directories(mmc_rpi3b_lib_result PUBLIC include/)

add_library(mmc_rpi3b_lib_status src/mmc_driver/bcm_emmc_regs/status.c)
target_include_directories(mmc_rpi3b_lib_status PUBLIC include/)
target_link_libraries(
        mmc_rpi3b_lib_status
        mmc_rpi3b_lib_result
)

add_library(mmc_rpi3b_lib_control0 src/mmc_driver/bcm_emmc_regs/control0.c)
target_include_directories(mmc_rpi3b_lib_control0 PUBLIC include/)
target_link_libraries(
        mmc_rpi3b_lib_control0
        mmc_rpi3b_lib_result
)

add_library(mmc_rpi3b_lib_control1 src/mmc_driver/bcm_emmc_regs/control1.c)
target_include_directories(mmc_rpi3b_lib_control1 PUBLIC include/)
target_link_libraries(
        mmc_rpi3b_lib_control1
        mmc_rpi3b_lib_result
)

add_library(mmc_rpi3b_lib_irpt_en src/mmc_driver/bcm_emmc_regs/irpt_en.c)
target_include_directories(mmc_rpi3b_lib_irpt_en PUBLIC include/)
target_link_libraries(
        mmc_rpi3b_lib_irpt_en
        mmc_rpi3b_lib_result
)

add_library(mmc_rpi3b_lib_irpt_mask src/mmc_driver/bcm_emmc_regs/irpt_mask.c)
target_include_directories(mmc_rpi3b_lib_irpt_mask PUBLIC include/)
target_link_libraries(
        mmc_rpi3b_lib_irpt_mask
        mmc_rpi3b_lib_result
)

add_library(mmc_rpi3b_lib_slotisr_ver src/mmc_driver/bcm_emmc_regs/slotisr_ver.c)
target_include_directories(mmc_rpi3b_lib_slotisr_ver PUBLIC include/)
target_link_libraries(
        mmc_rpi3b_lib_slotisr_ver
        mmc_rpi3b_lib_result
)

add_library(mmc_rpi3b_lib_cmd_index src/mmc_driver/bcm_emmc_regs/cmdtm/cmd_index.c)
target_include_directories(mmc_rpi3b_lib_cmd_index PUBLIC include/)

add_library(mmc_rpi3b_lib_cmd_rspns_type src/mmc_driver/bcm_emmc_regs/cmdtm/cmd_rspns_type.c)
target_include_directories(mmc_rpi3b_lib_cmd_rspns_type PUBLIC include/)

add_library(mmc_rpi3b_lib_cmdtm src/mmc_driver/bcm_emmc_regs/cmdtm.c)
target_include_directories(mmc_rpi3b_lib_cmdtm PUBLIC include/)
target_link_libraries(
        mmc_rpi3b_lib_cmdtm
        mmc_rpi3b_lib_result
        mmc_rpi3b_lib_cmd_rspns_type
        mmc_rpi3b_lib_cmd_index
)

add_library(mmc_rpi3b_lib_interrupt src/mmc_driver/bcm_emmc_regs/interrupt.c)
target_include_directories(mmc_rpi3b_lib_interrupt PUBLIC include/)
target_link_libraries(
        mmc_rpi3b_lib_interrupt
        mmc_rpi3b_lib_result
)

add_library(mmc_rpi3b_lib_bcm_emmc_regs src/mmc_driver/bcm_emmc_regs.c)
target_include_directories(mmc_rpi3b_lib_bcm_emmc_regs PUBLIC include/)
target_link_libraries(
        mmc_rpi3b_lib_bcm_emmc_regs
        mmc_rpi3b_lib_result
        mmc_rpi3b_lib_control0
        mmc_rpi3b_lib_control1
        mmc_rpi3b_lib_status
        mmc_rpi3b_lib_slotisr_ver
        mmc_rpi3b_lib_irpt_en
        mmc_rpi3b_lib_irpt_mask
        mmc_rpi3b_lib_interrupt
        mmc_rpi3b_lib_cmdtm
)

add_library(mmc_rpi3b_lib_sdhci_cmd src/mmc_driver/sdhci/sdhci_cmd.c)
target_include_directories(mmc_rpi3b_lib_sdhci_cmd PUBLIC include/)
target_link_libraries(
        mmc_rpi3b_lib_sdhci_cmd
        mmc_rpi3b_lib_cmdtm
        mmc_rpi3b_lib_result
        mmc_rpi3b_lib_cmd_rspns_type
        mmc_rpi3b_lib_cmd_index
)

add_library(mmc_rpi3b_lib_sdhci_cmds src/mmc_driver/sdhci/sdhci_cmds.c)
target_include_directories(mmc_rpi3b_lib_sdhci_cmds PUBLIC include/)
target_link_libraries(
        mmc_rpi3b_lib_sdhci_cmds
        mmc_rpi3b_lib_sdhci_cmd
        mmc_rpi3b_lib_sdhci_result
)

add_library(mmc_rpi3b_lib_sdhci_result src/mmc_driver/sdhci_result.c)
target_include_directories(mmc_rpi3b_lib_sdhci_result PUBLIC include/)
target_link_libraries(
        mmc_rpi3b_lib_sdhci_result
)

add_library(mmc_rpi3b_lib_sdhci src/mmc_driver/sdhci.c)
target_include_directories(mmc_rpi3b_lib_sdhci PUBLIC include/)
target_link_libraries(
        mmc_rpi3b_lib_sdhci
        mmc_rpi3b_lib_bcm_emmc_regs
        mmc_rpi3b_lib_result
        mmc_rpi3b_lib_sleep
        mmc_rpi3b_lib_log
        mmc_rpi3b_lib_arith
        mmc_rpi3b_lib_sdhci_result
        mmc_rpi3b_lib_sdhci_cmds
        mmc_rpi3b_lib_sdhci_cmd
        mmc_rpi3b_lib_cmd_rspns_type
        mmc_rpi3b_lib_interrupt
        mmc_rpi3b_lib_cmd_index
)

add_library(mmc_rpi3b_lib_bcm_emmc src/mmc_driver/bcm_emmc.c)
target_include_directories(mmc_rpi3b_lib_bcm_emmc PUBLIC include/)
target_link_libraries(
        mmc_rpi3b_lib_bcm_emmc
        mmc_rpi3b_lib_result
        mmc_rpi3b_lib_bcm_emmc_regs
        mmc_rpi3b_lib_sleep
        mmc_rpi3b_lib_sdhci
        mmc_rpi3b_lib_log
        mmc_rpi3b_lib_sdhci_result
)

add_library(mmc_rpi3b_lib_sd src/sd.c)
target_include_directories(mmc_rpi3b_lib_sd PUBLIC include/)
target_link_libraries(
        mmc_rpi3b_lib_sd
        mmc_rpi3b_lib_gpio
        mmc_rpi3b_lib_bcm_emmc_regs
        mmc_rpi3b_lib_sdhci
)

add_library(mmc_rpi3b_lib_mmc_driver src/mmc_driver.c)
target_include_directories(mmc_rpi3b_lib_mmc_driver PUBLIC include/)
target_link_libraries(
        mmc_rpi3b_lib_mmc_driver
        mmc_rpi3b_sel4cp_facade
        mmc_rpi3b_lib_uart
        mmc_rpi3b_lib_sd
        mmc_rpi3b_lib_bcm_emmc
        mmc_rpi3b_lib_result
        mmc_rpi3b_lib_printf
        mmc_rpi3b_lib_sleep
        mmc_rpi3b_lib_assert
        mmc_rpi3b_lib_time
        mmc_rpi3b_lib_timer_client
)



