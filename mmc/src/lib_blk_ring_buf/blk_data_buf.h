#pragma once

#include <stddef.h>
#include <stdint.h>

typedef struct blk_data_buf blk_data_buf_t;
struct blk_data_buf {
    uintptr_t buf;
    size_t buf_size;
};
