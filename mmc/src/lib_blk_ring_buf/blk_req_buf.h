#pragma once

#include <stddef.h>
#include <stdint.h>

/* Request Buffer. */

enum blk_req_operation {
    READ,
    WRITE,
};
typedef enum blk_req_operation blk_req_operation_t;

typedef struct blk_req_buf blk_req_buf_t;
struct blk_req_buf {
    /* The MMC driver operation. */
    blk_req_operation_t operation;
    /* Parameters used by Read/Write operations. */
    size_t lba; /* Logical Block Address. */
    size_t num_blocks; /* Number of Blocks. */
    uintptr_t shared_data_buf; /* Virtual address of shared data buffer. */
};



