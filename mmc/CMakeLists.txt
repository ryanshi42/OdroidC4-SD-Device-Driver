

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

add_library(mmc_rpi3b_lib_clock src/lib_clock/clock.c)
target_include_directories(mmc_rpi3b_lib_clock PUBLIC src/lib_clock/)
target_link_libraries(
        mmc_rpi3b_lib_clock
        mmc_rpi3b_lib_timer_client
        mmc_rpi3b_lib_result
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

add_library(mmc_rpi3b_lib_gpio_fix_resistor src/gpio_driver/gpio_fix_resistor.c)
target_include_directories(mmc_rpi3b_lib_gpio_fix_resistor PUBLIC include/)
target_link_libraries(
        mmc_rpi3b_lib_gpio_fix_resistor
)

add_library(mmc_rpi3b_lib_gpio_mode src/gpio_driver/gpio_mode.c)
target_include_directories(mmc_rpi3b_lib_gpio_mode PUBLIC include/)
target_link_libraries(
        mmc_rpi3b_lib_gpio_mode
)

add_library(mmc_rpi3b_lib_bcm_gpio_regs src/gpio_driver/bcm_gpio_regs.c)
target_include_directories(mmc_rpi3b_lib_bcm_gpio_regs PUBLIC include/)
target_link_libraries(
        mmc_rpi3b_lib_bcm_gpio_regs
        mmc_rpi3b_lib_result
)

add_library(mmc_rpi3b_lib_gpio_driver src/gpio_driver.c)
target_include_directories(mmc_rpi3b_lib_gpio_driver PUBLIC include/)
target_link_libraries(
        mmc_rpi3b_lib_gpio_driver
        mmc_rpi3b_lib_result
        mmc_rpi3b_lib_gpio_mode
        mmc_rpi3b_lib_bcm_gpio_regs
        mmc_rpi3b_lib_gpio_fix_resistor
        mmc_rpi3b_lib_sleep
)

add_library(mmc_rpi3b_lib_result_test src/lib_result/result_test.c)
target_include_directories(mmc_rpi3b_lib_result_test PUBLIC src/lib_result/)

add_library(mmc_rpi3b_lib_result_prod src/lib_result/result_prod.c)
target_include_directories(mmc_rpi3b_lib_result_prod PUBLIC src/lib_result/)

add_library(mmc_rpi3b_lib_result src/lib_result/result.c)
target_include_directories(mmc_rpi3b_lib_result PUBLIC src/lib_result/)
target_link_libraries(
        mmc_rpi3b_lib_result
        mmc_rpi3b_lib_result_test
        mmc_rpi3b_lib_result_prod
)

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

add_library(mmc_rpi3b_lib_blksizecnt src/mmc_driver/bcm_emmc_regs/blksizecnt.c)
target_include_directories(mmc_rpi3b_lib_blksizecnt PUBLIC include/)
target_link_libraries(
        mmc_rpi3b_lib_blksizecnt
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
        mmc_rpi3b_lib_blksizecnt
)

add_library(mmc_rpi3b_lib_csd src/mmc_driver/sdcard/sdcard_data/csd.c)
target_include_directories(mmc_rpi3b_lib_csd PUBLIC include/)
target_link_libraries(
        mmc_rpi3b_lib_csd
        mmc_rpi3b_lib_result
)

add_library(mmc_rpi3b_lib_ocr src/mmc_driver/sdcard/ocr.c)
target_include_directories(mmc_rpi3b_lib_ocr PUBLIC include/)
target_link_libraries(
        mmc_rpi3b_lib_ocr
        mmc_rpi3b_lib_result
)

add_library(mmc_rpi3b_lib_cid src/mmc_driver/sdcard/cid.c)
target_include_directories(mmc_rpi3b_lib_cid PUBLIC include/)
target_link_libraries(
        mmc_rpi3b_lib_cid
        mmc_rpi3b_lib_result
)

add_library(mmc_rpi3b_lib_sdcard_type src/mmc_driver/sdcard/sdcard_type.c)
target_include_directories(mmc_rpi3b_lib_sdcard_type PUBLIC include/)
target_link_libraries(
        mmc_rpi3b_lib_sdcard_type
        mmc_rpi3b_lib_result
)

add_library(mmc_rpi3b_lib_scr src/mmc_driver/sdcard/scr.c)
target_include_directories(mmc_rpi3b_lib_scr PUBLIC include/)
target_link_libraries(
        mmc_rpi3b_lib_scr
        mmc_rpi3b_lib_result
)

add_library(mmc_rpi3b_lib_sdcard_data_v1 src/mmc_driver/sdcard/sdcard_data/sdcard_data_v1.c)
target_include_directories(mmc_rpi3b_lib_sdcard_data_v1 PUBLIC include/)
target_link_libraries(
        mmc_rpi3b_lib_sdcard_data_v1
        mmc_rpi3b_lib_result
        mmc_rpi3b_lib_scr
)

add_library(mmc_rpi3b_lib_sdcard_data_v2 src/mmc_driver/sdcard/sdcard_data/sdcard_data_v2.c)
target_include_directories(mmc_rpi3b_lib_sdcard_data_v2 PUBLIC include/)
target_link_libraries(
        mmc_rpi3b_lib_sdcard_data_v2
        mmc_rpi3b_lib_result
        mmc_rpi3b_lib_csd
)

add_library(mmc_rpi3b_lib_sdcard_data src/mmc_driver/sdcard/sdcard_data.c)
target_include_directories(mmc_rpi3b_lib_sdcard_data PUBLIC include/)
target_link_libraries(
        mmc_rpi3b_lib_sdcard_data
        mmc_rpi3b_lib_result
        mmc_rpi3b_lib_csd
        mmc_rpi3b_lib_sdcard_data_v1
        mmc_rpi3b_lib_sdcard_data_v2
)

add_library(mmc_rpi3b_lib_sdcard src/mmc_driver/sdcard.c)
target_include_directories(mmc_rpi3b_lib_sdcard PUBLIC include/)
target_link_libraries(
        mmc_rpi3b_lib_sdcard
        mmc_rpi3b_lib_result
        mmc_rpi3b_lib_ocr
        mmc_rpi3b_lib_cid
        mmc_rpi3b_lib_sdcard_type
        mmc_rpi3b_lib_scr
        mmc_rpi3b_lib_log
        mmc_rpi3b_lib_sdcard_data
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

add_library(mmc_rpi3b_lib_sdhci_resp_r1 src/mmc_driver/sdhci/sdhci_resp_r1.c)
target_include_directories(mmc_rpi3b_lib_sdhci_resp_r1 PUBLIC include/)
target_link_libraries(
        mmc_rpi3b_lib_sdhci_resp_r1
        mmc_rpi3b_lib_result
)

add_library(mmc_rpi3b_lib_sdhci_resp_r6 src/mmc_driver/sdhci/sdhci_resp_r6.c)
target_include_directories(mmc_rpi3b_lib_sdhci_resp_r6 PUBLIC include/)
target_link_libraries(
        mmc_rpi3b_lib_sdhci_resp_r6
        mmc_rpi3b_lib_result
)

add_library(mmc_rpi3b_lib_sdhci_cmds src/mmc_driver/sdhci/sdhci_cmds.c)
target_include_directories(mmc_rpi3b_lib_sdhci_cmds PUBLIC include/)
target_link_libraries(
        mmc_rpi3b_lib_sdhci_cmds
        mmc_rpi3b_lib_sdhci_cmd
        mmc_rpi3b_lib_sdhci_result
)

add_library(mmc_rpi3b_lib_sdhci_result src/mmc_driver/sdhci/sdhci_result.c)
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
        mmc_rpi3b_lib_sdcard
        mmc_rpi3b_lib_sdhci_resp_r1
        mmc_rpi3b_lib_sdhci_resp_r6
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
        mmc_rpi3b_lib_bcm_gpio_regs
        mmc_rpi3b_lib_gpio_driver
)

add_library(mmc_rpi3b_lib_e2e_test src/e2e_test.c)
target_include_directories(mmc_rpi3b_lib_e2e_test PUBLIC include/)
target_link_libraries(
        mmc_rpi3b_lib_e2e_test
        mmc_rpi3b_lib_result
        mmc_rpi3b_lib_sleep
        mmc_rpi3b_lib_assert
        mmc_rpi3b_lib_clock
        mmc_rpi3b_lib_log
        mmc_rpi3b_lib_bcm_emmc
        mmc_rpi3b_lib_sdcard
)

add_library(mmc_rpi3b_lib_mmc_driver src/mmc_driver.c)
target_include_directories(mmc_rpi3b_lib_mmc_driver PUBLIC include/)
target_link_libraries(
        mmc_rpi3b_lib_mmc_driver
        mmc_rpi3b_sel4cp_facade
        mmc_rpi3b_lib_bcm_emmc
        mmc_rpi3b_lib_result
        mmc_rpi3b_lib_printf
        mmc_rpi3b_lib_log
        mmc_rpi3b_lib_timer_client
        mmc_rpi3b_lib_sdcard
        mmc_rpi3b_lib_e2e_test
        mmc_rpi3b_lib_sdhci
        mmc_rpi3b_lib_fatfs_e2e
)

add_library(mmc_rpi3b_lib_blk_data_ring_buf src/lib_blk_ring_buf/shared_data/blk_data_ring_buf.c)
target_include_directories(mmc_rpi3b_lib_blk_data_ring_buf PUBLIC src/lib_blk_ring_buf/shared_data/)
target_link_libraries(
        mmc_rpi3b_lib_blk_data_ring_buf
)

add_library(mmc_rpi3b_lib_blk_req_buf src/lib_blk_ring_buf/blk_req_buf.c)
target_include_directories(mmc_rpi3b_lib_blk_req_buf PUBLIC src/lib_blk_ring_buf/)
target_link_libraries(
        mmc_rpi3b_lib_blk_req_buf
)

add_library(mmc_rpi3b_lib_blk_res_buf src/lib_blk_ring_buf/blk_res_buf.c)
target_include_directories(mmc_rpi3b_lib_blk_res_buf PUBLIC src/lib_blk_ring_buf/)
target_link_libraries(
        mmc_rpi3b_lib_blk_res_buf
)

add_library(mmc_rpi3b_lib_blk_req_ring_buf src/lib_blk_ring_buf/blk_req_ring_buf.c)
target_include_directories(mmc_rpi3b_lib_blk_req_ring_buf PUBLIC src/lib_blk_ring_buf/)
target_link_libraries(
        mmc_rpi3b_lib_blk_req_ring_buf
        mmc_rpi3b_lib_blk_req_buf
)

add_library(mmc_rpi3b_lib_diskio src/lib_fatfs/diskio.c)
target_include_directories(mmc_rpi3b_lib_diskio PUBLIC src/lib_fatfs/)
target_link_libraries(
        mmc_rpi3b_lib_diskio
        mmc_rpi3b_lib_ff
        mmc_rpi3b_lib_mmc_driver
)

add_library(mmc_rpi3b_lib_diskio_test src/lib_fatfs/diskio_test.c)
target_include_directories(mmc_rpi3b_lib_diskio_test PUBLIC src/lib_fatfs/)
target_link_libraries(
        mmc_rpi3b_lib_diskio_test
        mmc_rpi3b_lib_ff
        mmc_rpi3b_lib_diskio
        mmc_rpi3b_lib_printf
)

add_library(mmc_rpi3b_lib_ff src/lib_fatfs/ff.c)
target_include_directories(mmc_rpi3b_lib_ff PUBLIC src/lib_fatfs/)
target_link_libraries(
        mmc_rpi3b_lib_ff
        mmc_rpi3b_lib_diskio
)

add_library(mmc_rpi3b_lib_ffconf INTERFACE)
target_include_directories(mmc_rpi3b_lib_ffconf INTERFACE src/lib_fatfs/)

add_library(mmc_rpi3b_lib_ffsystem src/lib_fatfs/ffsystem.c)
target_link_libraries(
        mmc_rpi3b_lib_ffsystem
        mmc_rpi3b_lib_ff
)

add_library(mmc_rpi3b_lib_ffunicode src/lib_fatfs/ffunicode.c)
target_link_libraries(
        mmc_rpi3b_lib_ffunicode
        mmc_rpi3b_lib_ff
)

add_library(mmc_rpi3b_lib_fatfs_e2e src/fatfs_e2e.c)
target_include_directories(mmc_rpi3b_lib_fatfs_e2e PUBLIC include/)
target_link_libraries(
        mmc_rpi3b_lib_fatfs_e2e
        mmc_rpi3b_lib_result
        mmc_rpi3b_lib_diskio_test
        mmc_rpi3b_lib_ff
        mmc_rpi3b_lib_assert
        mmc_rpi3b_lib_log
)

add_library(mmc_rpi3b_lib_fatfs src/fatfs.c)
target_include_directories(mmc_rpi3b_lib_fatfs PUBLIC include/)
target_link_libraries(
        mmc_rpi3b_lib_fatfs
        mmc_rpi3b_lib_sel4cp_facade
        mmc_rpi3b_lib_printf
        mmc_rpi3b_lib_log
        mmc_rpi3b_lib_result
)
