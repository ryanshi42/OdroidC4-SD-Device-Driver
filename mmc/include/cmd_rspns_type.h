#pragma once

enum cmd_rspns_type {
    CMD_NO_RESP = 0,        // no response
    CMD_136BIT_RESP = 1,    // 136 bits response
    CMD_48BIT_RESP = 2,     // 48 bits response
    CMD_BUSY48BIT_RESP = 3, // 48 bits response using busy
};
typedef enum cmd_rspns_type cmd_rspns_type_t;

