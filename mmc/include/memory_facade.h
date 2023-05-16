#pragma once

#if MMC_RPI3B_UNIT_TEST

#include <stdlib.h>

void malloc_addblock(void* addr, size_t size);

void malloc_init(void);

#else

#include "malloc.h"

#endif
