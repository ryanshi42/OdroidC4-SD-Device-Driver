#pragma once

#include <stdint.h>

#include "result.h"

/*
 * EMMC INTERRUPT register - BCM2835.PDF Manual Section 5 pages 75-77
 */
typedef struct interrupt interrupt_t;
struct __attribute__((__packed__, aligned(4))) interrupt {
    union {
        struct __attribute__((__packed__, aligned(1))) {
            volatile unsigned CMD_DONE: 1;   // @0		Command has finished
            volatile unsigned DATA_DONE: 1;  // @1		Data transfer has finished
            volatile unsigned BLOCK_GAP: 1;  // @2		Data transfer has stopped at block gap
            unsigned reserved: 1;            // @3		Write as zero read as don't care
            volatile unsigned WRITE_RDY: 1;  // @4		Data can be written to DATA register
            volatile unsigned READ_RDY: 1;   // @5		DATA register contains data to be read
            unsigned reserved1: 2;           // @6-7		Write as zero read as don't care
            volatile unsigned CARD: 1;       // @8		Card made interrupt request
            unsigned reserved2: 3;           // @9-11	Write as zero read as don't care
            volatile unsigned RETUNE: 1;     // @12		Clock retune request was made
            volatile unsigned BOOTACK: 1;    // @13		Boot acknowledge has been received
            volatile unsigned ENDBOOT: 1;    // @14		Boot operation has terminated
            volatile unsigned ERR: 1;        // @15		An error has occurred
            volatile unsigned CTO_ERR: 1;    // @16		Timeout on command line
            volatile unsigned CCRC_ERR: 1;   // @17		Command CRC error
            volatile unsigned CEND_ERR: 1;   // @18		End bit on command line not 1
            volatile unsigned CBAD_ERR: 1;   // @19		Incorrect command index in response
            volatile unsigned DTO_ERR: 1;    // @20		Timeout on data line
            volatile unsigned DCRC_ERR: 1;   // @21		Data CRC error
            volatile unsigned DEND_ERR: 1;   // @22		End bit on data line not 1
            unsigned reserved3: 1;           // @23		Write as zero read as don't care
            volatile unsigned ACMD_ERR: 1;   // @24		Auto command error
            unsigned reserved4: 7;           // @25-31	Write as zero read as don't care
        };
        volatile uint32_t raw32;             // @0-31	Union to access all 32 bits as a uint32_t
    };
};

result_t interrupt_get_raw32(interrupt_t *interrupt, uint32_t *ret_val);

result_t interrupt_set_raw32(interrupt_t *interrupt, uint32_t val);

result_t interrupt_mask_raw32(interrupt_t *interrupt, uint32_t mask, bool *ret_val);

result_t interrupt_get_cmd_done(interrupt_t *interrupt, bool *ret_val);

result_t interrupt_get_data_done(interrupt_t *interrupt, bool *ret_val);

result_t interrupt_get_write_rdy(interrupt_t *interrupt, bool *ret_val);

result_t interrupt_get_read_rdy(interrupt_t *interrupt, bool *ret_val);


