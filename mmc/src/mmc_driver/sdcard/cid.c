#include "cid.h"

result_t cid_set_raw32_0(cid_t *cid, uint32_t val) {
    if (cid == NULL) {
        return result_err("NULL `cid` passed to cid_set_raw32_0().");
    }
    cid->raw32_0 = val;
    return result_ok();
}

result_t cid_set_raw32_1(cid_t *cid, uint32_t val) {
    if (cid == NULL) {
        return result_err("NULL `cid` passed to cid_set_raw32_1().");
    }
    cid->raw32_1 = val;
    return result_ok();
}

result_t cid_set_raw32_2(cid_t *cid, uint32_t val) {
    if (cid == NULL) {
        return result_err("NULL `cid` passed to cid_set_raw32_2().");
    }
    cid->raw32_2 = val;
    return result_ok();
}

result_t cid_set_raw32_3(cid_t *cid, uint32_t val) {
    if (cid == NULL) {
        return result_err("NULL `cid` passed to cid_set_raw32_3().");
    }
    cid->raw32_3 = val;
    return result_ok();
}
