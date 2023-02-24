#pragma once

#include <sel4cp.h>

#define assert(expression) \
    seL4_Assert(expression); \
    if (!(expression)) {   \
        seL4_DebugHalt(); \
    }