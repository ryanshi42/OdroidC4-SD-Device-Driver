#pragma once

#include <stddef.h>
#include <stdint.h>

/* Request Buffer. */

enum blk_req_operation {
    READ,
    WRITE,
};
typedef enum blk_req_operation blk_req_operation_t;

#define NUM_SHARED_DATA_BUFS (512)

typedef struct blk_req_buf blk_req_buf_t;
struct blk_req_buf {
    /* The MMC driver operation. */
    blk_req_operation_t operation;
    /* Parameters used by Read/Write operations. */
    size_t lba; /* Logical Block Address. */
    size_t num_blocks; /* Number of Blocks. */
    size_t block_size; /* Size of the Blocks. */
    /* An array of virtual addresses to shared data buffers used by a request.
     * */
    uintptr_t shared_data_bufs[NUM_SHARED_DATA_BUFS];
    /* The number of shared data buffers used. */
    size_t num_shared_data_bufs;
};



