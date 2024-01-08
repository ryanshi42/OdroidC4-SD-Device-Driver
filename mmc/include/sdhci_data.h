#pragma once



/**
 * SD CARD COMMAND RECORD
 */
typedef struct sdhci_data sdhci_data_t;
struct sdhci_data {
    union {
        char *dest;
        const char *src;
    };
    void* buffer;
    uint32_t blocks;
    uint32_t blocksize;
    uint32_t flags;
};
