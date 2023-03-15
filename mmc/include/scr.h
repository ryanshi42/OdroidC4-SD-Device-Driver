#pragma once

#include <stdint.h>

#include "result.h"

enum sd_spec {
    SD_SPEC_1_101 = 0,  // ..enum..	Version 1.0-1.01
    SD_SPEC_11 = 1,     // ..enum..	Version 1.10
    SD_SPEC_2_3 = 2,    // ..enum..	Version 2.00 or Version 3.00 (check bit SD_SPEC3)
};
typedef enum sd_spec sd_spec_t;

enum scr_struct {
    SCR_VER_1 = 0,      // ..enum..	SCR version 1.0
};
typedef enum scr_struct scr_struct_t;

enum bus_width {
    BUS_WIDTH_1 = 1,    // ..enum..	Card supports bus width 1
    BUS_WIDTH_4 = 4,    // ..enum.. Card supports bus width 4
};
typedef enum bus_width bus_width_t;

enum sd_security {
    SD_SEC_NONE = 0,      // ..enum..	No Security
    SD_SEC_NOT_USED = 1,  // ..enum..	Security Not Used
    SD_SEC_101 = 2,       // ..enum..	SDSC Card (Security Version 1.01)
    SD_SEC_2 = 3,         // ..enum..	SDHC Card (Security Version 2.00)
    SD_SEC_3 = 4,         // ..enum..	SDXC Card (Security Version 3.xx)
};
typedef enum sd_security sd_security_t;

enum ex_security {
    EX_SEC_NONE = 0,      // ..enum..	No extended Security
};
typedef enum ex_security ex_security_t;

enum cmd_support {
    CMD_SUPP_SPEED_CLASS = 1,
    CMD_SUPP_SET_BLKCNT = 2,
};
typedef enum cmd_support cmd_support_t;

/**
 * SD CARD Configuration (SCR) register
 */
typedef struct scr scr_t;
struct __attribute__((__packed__, aligned(4))) scr {
    union {
        struct __attribute__((__packed__, aligned(1))) {
            volatile sd_spec_t SD_SPEC: 4;          // @0-3		SD Memory Card Physical Layer Specification version
            volatile scr_struct_t SCR_STRUCT: 4;    // @4-7		SCR structure version
            volatile bus_width_t BUS_WIDTH: 4;      // @8-11	SD Bus width
            volatile sd_security_t SD_SECURITY: 3;  // @12-14	Card security in use
            volatile unsigned DATA_AFTER_ERASE: 1;  // @15		Defines the data status after erase, whether it is 0 or 1
            unsigned reserved: 3;                   // @16-18	Write as zero read as don't care
            volatile ex_security_t EX_SECURITY: 4;  // @19-22	Extended security
            volatile unsigned SD_SPEC3: 1;          // @23		Spec. Version 3.00 or higher
            volatile cmd_support_t CMD_SUPPORT: 2;  // @24-25	CMD support
            unsigned reserved1: 6;                  // @26-63	Write as zero read as don't care
        };
        volatile uint32_t raw32_lo;                 // @0-31	Union to access low 32 bits as a uint32_t
    };
    volatile uint32_t raw32_hi;                     // @32-63	Access upper 32 bits as a uint32_t
};

/**
 * Sets the `raw32_lo` field of the scr register.
 * @param scr
 * @param val
 * @return
 */
result_t scr_set_raw32_lo(scr_t *scr, uint32_t val);

/**
 * Sets the `raw32_hi` field of the scr register.
 * @param scr
 * @param val
 * @return
 */
result_t scr_set_raw32_hi(scr_t *scr, uint32_t val);

/**
 * Gets the `BUS_WIDTH` field of the scr register.
 * @param scr
 * @param val
 * @return
 */
result_t scr_get_bus_width(scr_t *scr, bus_width_t *ret_val);

/**
 * Returns true if the card supports 1-bit bus width and false otherwise.
 * @param scr
 * @param ret_val
 * @return
 */
result_t scr_is_bus_width_1_supported(scr_t *scr, bool *ret_val);

/**
 * Returns true if the card supports 4-bit bus width and false otherwise.
 * @param scr
 * @param ret_val
 * @return
 */
result_t scr_is_bus_width_4_supported(scr_t *scr, bool *ret_val);

/**
 * Returns true if the card supports the SET_BLOCK_COUNT command and false otherwise.
 * @param scr
 * @param ret_val
 * @return
 */
result_t scr_is_set_block_count_cmd_supported(scr_t *scr, bool *ret_val);

/**
 * Returns true if the card supports the SPEED_CLASS_CONTROL command and false otherwise.
 * @param scr
 * @param ret_val
 * @return
 */
result_t scr_is_speed_class_control_cmd_supported(scr_t *scr, bool *ret_val);
