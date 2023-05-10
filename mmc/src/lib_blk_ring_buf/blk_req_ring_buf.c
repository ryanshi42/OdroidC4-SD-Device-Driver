#include "blk_req_ring_buf.h"


blk_req_ring_buf_result_t blk_req_ring_buf_init(
        blk_req_ring_buf_t *ring_buf,
        size_t req_ring_buf_region_size
) {
    if (ring_buf == NULL) {
        return ERR_NULL_BLK_REQ_RING_BUF;
    }
    if (req_ring_buf_region_size < sizeof(*ring_buf)) {
        return ERR_BLK_REQ_RING_BUF_REGION_TOO_SMALL;
    }
    size_t const num_unused_bytes = (req_ring_buf_region_size - sizeof(*ring_buf));
    if ((num_unused_bytes / sizeof(blk_req_buf_t)) > 0) {
        return ERR_BLK_REQ_RING_BUF_REGION_TOO_LARGE;
    }
    /* Clear the `ring_buf` data structure, which initialises `head_idx` and
     * `tail_idx` to zero. */
    memset(ring_buf, 0, sizeof(*ring_buf));
    return OK_BLK_REQ_RING_BUF;
}


