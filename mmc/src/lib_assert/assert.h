#pragma once

#if MMC_RPI3B_UNIT_TEST

#include <assert.h>

#else

#include <sel4cp.h>

#define assert(expression) \
    seL4_Assert(expression); \
    if (!(expression)) {   \
        seL4_DebugHalt(); \
    }

#endif