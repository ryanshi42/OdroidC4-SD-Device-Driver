#include "sdcard.h"

result_t sdcard_init(sdcard_t *sdcard) {
    if (sdcard == NULL) {
        return result_err("NULL `sdcard` passed to sdcard_init().");
    }
    /* Initialise everything to 0. */
    memset(sdcard, 0, sizeof(*sdcard));
    return result_ok();
}

result_t sdcard_set_ocr_raw32(sdcard_t *sdcard, uint32_t val) {
    if (sdcard == NULL) {
        return result_err("NULL `sdcard` passed to sdcard_set_ocr_raw32().");
    }
    return ocr_set_raw32(&sdcard->ocr, val);
}

result_t sdcard_get_rca(sdcard_t *sdcard, uint32_t *ret_val) {
    if (sdcard == NULL) {
        return result_err("NULL `sdcard` passed to sdcard_get_rca().");
    }
    if (ret_val == NULL) {
        return result_err("NULL `ret_val` passed to sdcard_get_rca().");
    }
    *ret_val = sdcard->rca;
    return result_ok();
}

result_t sdcard_set_rca(sdcard_t *sdcard, uint32_t val) {
    if (sdcard == NULL) {
        return result_err("NULL `sdcard` passed to sdcard_set_rca().");
    }
    sdcard->rca = val;
    return result_ok();
}

result_t sdcard_set_status(sdcard_t *sdcard, uint32_t val) {
    if (sdcard == NULL) {
        return result_err("NULL `sdcard` passed to sdcard_set_status().");
    }
    sdcard->status = val;
    return result_ok();
}

result_t sdcard_mask_status(sdcard_t *sdcard, uint32_t mask, bool* ret_val) {
    if (sdcard == NULL) {
        return result_err("NULL `sdcard` passed to sdcard_mask_status().");
    }
    if (ret_val == NULL) {
        return result_err("NULL `ret_val` passed to sdcard_mask_status().");
    }
    *ret_val = sdcard->status & mask;
    return result_ok();
}

result_t sdcard_has_rca(sdcard_t *sdcard, bool *ret_val) {
    if (sdcard == NULL) {
        return result_err("NULL `sdcard` passed to sdcard_has_rca().");
    }
    if (ret_val == NULL) {
        return result_err("NULL `ret_val` passed to sdcard_has_rca().");
    }
    *ret_val = sdcard->rca != 0;
    return result_ok();
}

result_t sdcard_is_app_cmd_accepted(sdcard_t *sdcard, bool *ret_val) {
    if (sdcard == NULL) {
        return result_err("NULL `sdcard` passed to sdcard_is_app_cmd_accepted().");
    }
    return sdcard_mask_status(sdcard, ST_APP_CMD, ret_val);
}

result_t sdcard_has_powered_up(sdcard_t *sdcard, bool *ret_val) {
    if (sdcard == NULL) {
        return result_err("NULL `sdcard` passed to sdcard_has_powered_up().");
    }
    return ocr_get_card_power_up_busy(&sdcard->ocr, ret_val);
}

result_t sdcard_is_voltage_3v3(sdcard_t *sdcard, bool *ret_val) {
    if (sdcard == NULL) {
        return result_err("NULL `sdcard` passed to sdcard_is_voltage_3v3().");
    }
    bool voltage3v2to3v3 = false;
    result_t result = ocr_get_voltage_3v2to3v3(&sdcard->ocr, &voltage3v2to3v3);
    if (result_is_err(result)) {
        return result_err_chain(result, "Failed to get voltage 3v2to3v3.");
    }
    bool voltage3v3to3v4 = false;
    result = ocr_get_voltage_3v3to3v4(&sdcard->ocr, &voltage3v3to3v4);
    if (result_is_err(result)) {
        return result_err_chain(result, "Failed to get voltage 3v3to3v4.");
    }
    *ret_val = voltage3v2to3v3 || voltage3v3to3v4;
    return result_ok();
}

result_t sdcard_is_high_capacity(sdcard_t *sdcard, bool *ret_val) {
    if (sdcard == NULL) {
        return result_err("NULL `sdcard` passed to sdcard_is_high_capacity().");
    }
    bool has_powered_up = false;
    result_t result = sdcard_has_powered_up(sdcard, &has_powered_up);
    if (result_is_err(result)) {
        return result_err_chain(result, "Failed to get power up status in sdcard_is_high_capacity().");
    }
    /* Per the SD Physical spec, the capacity bit is only valid when the card is powered up. */
    if (!has_powered_up) {
        return result_err("SD card has not powered up in sdcard_is_high_capacity().");
    }
    return ocr_get_card_capacity(&sdcard->ocr, ret_val);
}

result_t sdcard_set_cid(
        sdcard_t *sdcard,
        uint32_t resp0,
        uint32_t resp1,
        uint32_t resp2,
        uint32_t resp3
) {
    if (sdcard == NULL) {
        return result_err("NULL `sdcard` passed to sdcard_set_cid().");
    }
    result_t res;
    res = cid_set_raw32_3(&sdcard->cid, resp0);
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to set CID raw32_3 in sdcard_set_cid().");
    }
    res = cid_set_raw32_2(&sdcard->cid, resp1);
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to set CID raw32_2 in sdcard_set_cid().");
    }
    res = cid_set_raw32_1(&sdcard->cid, resp2);
    if (result_is_err(res)) {
        return result_err_chain(res, "Failed to set CID raw32_1 in sdcard_set_cid().");
    }
    res = cid_set_raw32_0(&sdcard->cid, resp3);
    return result_ok_or(res, "Failed to set CID raw32_0 in sdcard_set_cid().");
}

result_t sdcard_set_csd(
        sdcard_t *sdcard,
        uint32_t resp0,
        uint32_t resp1,
        uint32_t resp2,
        uint32_t resp3
) {
    uint8_t buf[16] = {0};

    /* Fill buffer CSD comes IN MSB so we sort it for debugging */
    __attribute__((aligned(4))) uint32_t *p;
    p = (uint32_t *) &buf[12];
    *p = resp0;
    p = (uint32_t *) &buf[8];
    *p = resp1;
    p = (uint32_t *) &buf[4];
    *p = resp2;
    p = (uint32_t *) &buf[0];
    *p = resp3;

//    /* Display raw CSD - values of my SANDISK ultra 16GB shown under each */
//    LOG_DEBUG("CSD Contents : %02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\n",
//              buf[2], buf[1], buf[0], buf[7], buf[6], buf[5], buf[4],
//    /*    40       e0      00     32        5b     59     00               */
//              buf[11], buf[10], buf[9], buf[8], buf[15], buf[14], buf[13], buf[12]);
//    /*    00       73       a7     7f       80       0a       40       00 */

    csd_t *csd = &sdcard->csd;

    /* Populate CSD structure */
    csd->csd_structure = (buf[2] & 0xc0) >> 6;                                // @126-127 ** correct
    csd->spec_vers = buf[2] & 0x3F;                                            // @120-125 ** correct
    csd->taac = buf[1];                                                        // @112-119 ** correct
    csd->nsac = buf[0];                                                        // @104-111 ** correct
    csd->tran_speed = buf[7];                                                // @96-103  ** correct
    csd->ccc = (((uint16_t) buf[6]) << 4) | ((buf[5] & 0xf0) >> 4);            // @84-95   ** correct
    csd->read_bl_len = buf[5] & 0x0f;                                        // @80-83   ** correct
    csd->read_bl_partial = (buf[4] & 0x80) ? 1 : 0;                            // @79		** correct
    csd->write_blk_misalign = (buf[4] & 0x40) ? 1 : 0;                        // @78      ** correct
    csd->read_blk_misalign = (buf[4] & 0x20) ? 1 : 0;                        // @77		** correct
    csd->dsr_imp = (buf[4] & 0x10) ? 1 : 0;                                    // @76		** correct

    if (csd->csd_structure == 0x1) {                                        // CSD VERSION 2.0
        /* Basically absorbs bottom of buf[4] to align to next byte */        // @@75-70  ** Correct
        csd->ver2_c_size = (uint32_t) (buf[11] & 0x3F) << 16;                // @69-64
        csd->ver2_c_size += (uint32_t) buf[10] << 8;                            // @63-56
        csd->ver2_c_size += (uint32_t) buf[9];                                // @55-48
//        sdCard.CardCapacity = csd->ver2_c_size;
//        sdCard.CardCapacity *= (512 * 1024);                                // Calculate Card capacity
    } else {                                                                    // CSD VERSION 1.0
        csd->c_size = (uint32_t) (buf[4] & 0x03) << 8;
        csd->c_size += (uint32_t) buf[11];
        csd->c_size <<= 2;
        csd->c_size += (buf[10] & 0xc0) >> 6;                                // @62-73
        csd->vdd_r_curr_min = (buf[10] & 0x38) >> 3;                        // @59-61
        csd->vdd_r_curr_max = buf[10] & 0x07;                                // @56-58
        csd->vdd_w_curr_min = (buf[9] & 0xe0) >> 5;                            // @53-55
        csd->vdd_w_curr_max = (buf[9] & 0x1c) >> 2;                            // @50-52
        csd->c_size_mult = ((buf[9] & 0x03) << 1) | ((buf[8] & 0x80) >> 7);    // @47-49
//        sdCard.CardCapacity = (csd->c_size + 1) * (1 << (csd->c_size_mult + 2)) * (1 << csd->read_bl_len);
    }

    csd->erase_blk_en = (buf[8] & 0x40) >> 6;                                // @46
    csd->sector_size = ((buf[15] & 0x80) >> 1) | (buf[8] & 0x3F);            // @39-45
    csd->wp_grp_size = buf[15] & 0x7f;                                        // @32-38
    csd->wp_grp_enable = (buf[14] & 0x80) ? 1 : 0;                            // @31
    csd->default_ecc = (buf[14] & 0x60) >> 5;                                // @29-30
    csd->r2w_factor = (buf[14] & 0x1c) >> 2;                                // @26-28   ** correct
    csd->write_bl_len = ((buf[14] & 0x03) << 2) | ((buf[13] & 0xc0) >> 6);  // @22-25   **correct
    csd->write_bl_partial = (buf[13] & 0x20) ? 1 : 0;                        // @21
    // @16-20 are reserved
    csd->file_format_grp = (buf[12] & 0x80) ? 1 : 0;                        // @15
    csd->copy = (buf[12] & 0x40) ? 1 : 0;                                    // @14
    csd->perm_write_protect = (buf[12] & 0x20) ? 1 : 0;                        // @13
    csd->tmp_write_protect = (buf[12] & 0x10) ? 1 : 0;                        // @12
    csd->file_format = (buf[12] & 0x0c) >> 2;                                // @10-11    **correct
    csd->ecc = buf[12] & 0x03;                                                // @8-9      **corrrect

//    LOG_DEBUG("  csd_structure=%d\t  spec_vers=%d\t  taac=%02x\t nsac=%02x\t  tran_speed=%02x\t  ccc=%04x\n"
//              "  read_bl_len=%d\t  read_bl_partial=%d\t  write_blk_misalign=%d\t  read_blk_misalign=%d\n"
//              "  dsr_imp=%d\t  sector_size =%d\t  erase_blk_en=%d\n",
//              csd->csd_structure, csd->spec_vers, csd->taac, csd->nsac, csd->tran_speed, csd->ccc,
//              csd->read_bl_len, csd->read_bl_partial, csd->write_blk_misalign, csd->read_blk_misalign,
//              csd->dsr_imp, csd->sector_size, csd->erase_blk_en);
//
//    if (csd->csd_structure == 0x1) {
//        LOG_DEBUG("CSD 2.0: ver2_c_size = %d\t  card capacity: %lu\n",
//                  csd->ver2_c_size, sdCard.CardCapacity);
//    } else {
//        LOG_DEBUG("CSD 1.0: c_size = %d\t  c_size_mult=%d\t card capacity: %lu\n"
//                  "  vdd_r_curr_min = %d\t  vdd_r_curr_max=%d\t  vdd_w_curr_min = %d\t  vdd_w_curr_max=%d\n",
//                  csd->c_size, csd->c_size_mult, sdCard.CardCapacity,
//                  csd->vdd_r_curr_min, csd->vdd_r_curr_max, csd->vdd_w_curr_min, csd->vdd_w_curr_max);
//    }
//
//    LOG_DEBUG("  wp_grp_size=%d\t  wp_grp_enable=%d\t  default_ecc=%d\t  r2w_factor=%d\n"
//              "  write_bl_len=%d\t  write_bl_partial=%d\t  file_format_grp=%d\t  copy=%d\n"
//              "  perm_write_protect=%d\t  tmp_write_protect=%d\t  file_format=%d\t  ecc=%d\n",
//              csd->wp_grp_size, csd->wp_grp_enable, csd->default_ecc, csd->r2w_factor,
//              csd->write_bl_len, csd->write_bl_partial, csd->file_format_grp, csd->copy,
//              csd->perm_write_protect, csd->tmp_write_protect, csd->file_format, csd->ecc);

    return result_ok();
}

result_t sdcard_set_type(sdcard_t *sdcard, sdcard_type_t type) {
    if (sdcard == NULL) {
        return result_err("NULL `sdcard` passed to sdcard_set_type().");
    }
    sdcard->type = type;
    return result_ok();
}

result_t sdcard_set_scr_raw32_lo(sdcard_t *sdcard, uint32_t val) {
    if (sdcard == NULL) {
        return result_err("NULL `sdcard` passed to sdcard_set_scr_raw32_lo().");
    }
    return scr_set_raw32_lo(&sdcard->scr, val);
}

result_t sdcard_set_scr_raw32_hi(sdcard_t *sdcard, uint32_t val) {
    if (sdcard == NULL) {
        return result_err("NULL `sdcard` passed to sdcard_set_scr_raw32_hi().");
    }
    return scr_set_raw32_hi(&sdcard->scr, val);
}

result_t sdcard_is_bus_width_4(sdcard_t *sdcard, bool *ret_val) {
    if (sdcard == NULL) {
        return result_err("NULL `sdcard` passed to sdcard_is_bus_width_4().");
    }
    return scr_is_bus_width_4_supported(&sdcard->scr, ret_val);
}

result_t sdcard_is_bus_width_1(sdcard_t *sdcard, bool *ret_val) {
    if (sdcard == NULL) {
        return result_err("NULL `sdcard` passed to sdcard_is_bus_width_1().");
    }
    return scr_is_bus_width_1_supported(&sdcard->scr, ret_val);
}

result_t sdcard_is_type_unknown(sdcard_t *sdcard, bool *ret_val) {
    if (sdcard == NULL) {
        return result_err("NULL `sdcard` passed to sdcard_is_type_unknown().");
    }
    *ret_val = sdcard->type == SD_TYPE_UNKNOWN;
    return result_ok();
}

result_t sdcard_is_type_standard_capacity(sdcard_t *sdcard, bool *ret_val) {
    if (sdcard == NULL) {
        return result_err("NULL `sdcard` passed to sdcard_is_type_standard_capacity().");
    }
    *ret_val = sdcard->type == SD_TYPE_2_SC;
    return result_ok();
}

result_t sdcard_is_type_high_capacity(sdcard_t *sdcard, bool *ret_val) {
    if (sdcard == NULL) {
        return result_err("NULL `sdcard` passed to sdcard_is_type_high_capacity().");
    }
    *ret_val = sdcard->type == SD_TYPE_2_HC;
    return result_ok();
}

result_t sdcard_is_set_block_count_cmd_supported(sdcard_t *sdcard, bool *ret_val) {
    if (sdcard == NULL) {
        return result_err("NULL `sdcard` passed to sdcard_is_set_block_count_cmd_supported().");
    }
    return scr_is_set_block_count_cmd_supported(&sdcard->scr, ret_val);
}


