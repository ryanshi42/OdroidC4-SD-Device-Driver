
add_executable(test_mmc_rpi3b_lib_arith common/test_arith.cpp)
add_executable(test_mmc_rpi3b_lib_control0 mmc_driver/bcm_emmc_regs/test_control0.cpp)
add_executable(test_mmc_rpi3b_lib_control1 mmc_driver/bcm_emmc_regs/test_control1.cpp)
add_executable(test_mmc_rpi3b_lib_bcm_emmc_regs mmc_driver/test_bcm_emmc_regs.cpp)
add_executable(test_mmc_rpi3b_lib_bcm_emmc mmc_driver/test_bcm_emmc.cpp)
add_executable(test_mmc_rpi3b_lib_result mmc_driver/common/test_result.cpp)
add_executable(test_mmc_rpi3b_lib_bcm_timer_regs timer_driver/test_bcm_timer_regs.cpp)
add_executable(test_mmc_rpi3b_lib_bcm_timer timer_driver/test_bcm_timer.cpp)

add_google_test(test_mmc_rpi3b_lib_arith mmc_rpi3b_lib_arith)
add_google_test(test_mmc_rpi3b_lib_control0 mmc_rpi3b_lib_control0)
add_google_test(test_mmc_rpi3b_lib_control1 mmc_rpi3b_lib_control1)
add_google_test(test_mmc_rpi3b_lib_bcm_emmc_regs mmc_rpi3b_lib_bcm_emmc_regs)
add_google_test(test_mmc_rpi3b_lib_bcm_emmc mmc_rpi3b_lib_bcm_emmc)
add_google_test(test_mmc_rpi3b_lib_result mmc_rpi3b_lib_result)
add_google_test(test_mmc_rpi3b_lib_bcm_timer_regs mmc_rpi3b_lib_bcm_timer_regs)
add_google_test(test_mmc_rpi3b_lib_bcm_timer mmc_rpi3b_lib_bcm_timer)

target_link_libraries(
        test_mmc_rpi3b_lib_bcm_emmc
        lib_fff
)
target_link_libraries(
        test_mmc_rpi3b_lib_bcm_emmc_regs
        lib_fff
)
target_link_libraries(
        test_mmc_rpi3b_lib_bcm_timer
        lib_fff
)
