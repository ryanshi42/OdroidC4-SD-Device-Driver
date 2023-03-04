#pragma once

typedef enum {
    SD_OK = 0,              // NO error
    SD_ERROR = 1,           // General non specific SD error
    SD_TIMEOUT = 2,         // SD Timeout error
    SD_BUSY = 3,            // SD Card is busy
    SD_NO_RESP = 5,         // SD Card did not respond
    SD_ERROR_RESET = 6,     // SD Card did not reset
    SD_ERROR_CLOCK = 7,     // SD Card clock change failed
    SD_ERROR_VOLTAGE = 8,   // SD Card does not support requested voltage
    SD_ERROR_APP_CMD = 9,   // SD Card app command failed
    SD_CARD_ABSENT = 10,    // SD Card not present
    SD_READ_ERROR = 11,
    SD_MOUNT_FAIL = 12,
} sdhci_result_t;

