#pragma once

/*
 * - `result_test.h` is used during development/unit testing. This
 * implementation allows unit tests to run in parallel; however, it is much less
 * performant than `result_prod.h` since `result_t` is a struct (containing
 * bookkeeping data) that is returned on the stack every time `result_t` is
 * returned by a function.
 * - `result_prod.h` is used in production on hardware; however, it cannot be
 * used for unit testing because it stores all its bookkeeping data in static
 * global space, which is not thread-safe for unit tests that are run in
 * parallel. However, its implementation is much more performant and lightweight
 * than `result_test.h` since `result_t` is simply bool. We can store
 * bookkeeping data in static global space since each seL4CP Protection Domain
 * (PD) is single-threaded, and we always use a single `result` object per PD.
 */

#if MMC_RPI3B_UNIT_TEST
#include "result_test.h"
#else
#include "result_prod.h"
#endif
