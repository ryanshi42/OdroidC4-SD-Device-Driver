#pragma once

#include "result.h"

/**
 * PI SD CARD CID (Card Identification) register
 * The CID is Big Endian and secondly the Pi butchers it by not having CRC
 * So the CID appears shifted 8 bits right with first 8 bits reading zero
 */
typedef struct cid cid_t;
struct __attribute__((__packed__, aligned(4))) cid {
    union {
        struct __attribute__((__packed__, aligned(1))) {
            volatile uint8_t OID_Lo;
            volatile uint8_t OID_Hi;     // @0-15	Identifies the card OEM. The OID is assigned by the SD-3C, LLC
            volatile uint8_t MID;        // @16-23	Manufacturer ID, assigned by the SD-3C, LLC
            unsigned reserved: 8;        // @24-31	PI butcher with CRC removed these bits end up empty
        };
        volatile uint32_t raw32_0;       // @0-31	Union to access 32 bits as a uint32_t
    };
    union {
        struct __attribute__((__packed__, aligned(1))) {
            volatile char ProdName4: 8;  // @0-7		Product name character four
            volatile char ProdName3: 8;  // @8-15	Product name character three
            volatile char ProdName2: 8;  // @16-23	Product name character two
            volatile char ProdName1: 8;  // @24-31	Product name character one
        };
        volatile uint32_t raw32_1;       // @0-31	Union to access 32 bits as a uint32_t
    };
    union {
        struct __attribute__((__packed__, aligned(1))) {
            volatile unsigned SerialNumHi: 16; // @0-15	Serial number upper 16 bits
            volatile unsigned ProdRevLo: 4;    // @16-19	Product revision low value in BCD
            volatile unsigned ProdRevHi: 4;    // @20-23	Product revision high value in BCD
            volatile char ProdName5: 8;        // @24-31	Product name character five
        };
        volatile uint32_t raw32_2;             // @0-31	Union to access 32 bits as a uint32_t
    };
    union {
        struct __attribute__((__packed__, aligned(1))) {
            volatile unsigned ManufactureMonth: 4; // @0-3		Manufacturing date month (1=Jan, 2=Feb, 3=Mar etc)
            volatile unsigned ManufactureYear: 8;  // @4-11	Manufacturing dateyear (offset from 2000 .. 1=2001,2=2002,3=2003 etc)
            unsigned reserved1: 4;                 // @12-15 	Write as zero read as don't care
            volatile unsigned SerialNumLo: 16;     // @16-23	Serial number lower 16 bits
        };
        volatile uint32_t raw32_3;                 // @0-31	Union to access 32 bits as a uint32_t
    };
};

/**
 * Sets the raw32_0.
 * @param cid
 * @param val
 * @return
 */
result_t cid_set_raw32_0(cid_t *cid, uint32_t val);

/**
 * Sets the raw32_1.
 * @param cid
 * @param val
 * @return
 */
result_t cid_set_raw32_1(cid_t *cid, uint32_t val);

/**
 * Sets the raw32_2.
 * @param cid
 * @param val
 * @return
 */
result_t cid_set_raw32_2(cid_t *cid, uint32_t val);

/**
 * Sets the raw32_3.
 * @param cid
 * @param val
 * @return
 */
result_t cid_set_raw32_3(cid_t *cid, uint32_t val);
