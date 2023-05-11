#pragma once

#include <stddef.h>
#include <stdint.h>

typedef struct blk_shared_data_buf blk_shared_data_buf_t;
struct blk_shared_data_buf {
    uintptr_t buf_vaddr;
    size_t buf_size;
};
