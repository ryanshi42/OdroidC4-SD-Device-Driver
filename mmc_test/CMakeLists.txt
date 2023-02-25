
cmake_minimum_required(VERSION 3.14)
project(mmc_rpi3b_test_unit CXX C)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_C_STANDARD 99)

# These are compiler flags specific for running tests.
set(GCC_COMPILE_FLAGS -Wall -Werror -Wvla -O0 -Wno-unused-function -Wno-unused-variable -Wno-unused-but-set-variable)

# Download Google Test
include(FetchContent)
FetchContent_Declare(
        googletest
        # Specify the commit you depend on and update it regularly.
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

function(add_google_test test_target target)
    target_link_libraries(${test_target} ${target} gtest_main)
    # Using the compile flags for the main executable from GCC_COMPILE_FLAGS
    # and appending test-specific flags from GCC_COMPILE_FLAGS_TEST
    target_compile_options(${test_target} PRIVATE ${GCC_COMPILE_FLAGS} ${GCC_COMPILE_FLAGS_TEST})
    add_test(${test_target} ${test_target})
endfunction(add_google_test)

add_subdirectory(libfff)

add_subdirectory(unit)

