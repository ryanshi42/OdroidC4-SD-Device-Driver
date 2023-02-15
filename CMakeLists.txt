
add_subdirectory(mmc/)

# ====================================
# Enables unit testing
# ====================================

include (CTest)
# Enabling tests
enable_testing()
# Add our test directory.
add_subdirectory(mmc_test/unit)
