/*-----------------------------------------------------------------------*/
/* Low level disk I/O module SKELETON for FatFs     (C)ChaN, 2019        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various existing      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "ff.h"            /* Obtains integer types */
#include "diskio.h"        /* Declarations of disk functions */

/* Definitions of physical drive number for each drive */
#define DEV_MMC        0    /* Example: Map MMC/SD card to physical drive 0 */
#define DEV_RAM        1    /* Example: Map Ramdisk to physical drive 1 */
#define DEV_USB        2    /* Example: Map USB MSD to physical drive 2 */

mmc_driver_client_t *fatfs_mmc_driver_client = NULL;
uint16_t global_mmc_block_size = 0;

DRESULT disk_sddf_init(
        mmc_driver_client_t *mmc_driver_client
) {
    fatfs_mmc_driver_client = mmc_driver_client;
    /* Get the block size from the device. */
    return disk_ioctl(
            DEV_MMC,
            GET_BLOCK_SIZE,
            &global_mmc_block_size
    );
}

/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status(
        BYTE pdrv        /* Physical drive nmuber to identify the drive */
) {
    DSTATUS stat = STA_NOINIT;
    int result;
    (void) result;

    switch (pdrv) {
        case DEV_RAM :
//		result = RAM_disk_status();

            // translate the reslut code here

            return stat;

        case DEV_MMC :
//		result = MMC_disk_status();

            // translate the reslut code here
            stat = 0;

            return stat;

        case DEV_USB :
//		result = USB_disk_status();

            // translate the reslut code here

            return stat;
    }
    return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize(
        BYTE pdrv                /* Physical drive nmuber to identify the drive */
) {
    DSTATUS stat = STA_NOINIT;
    int result;
    (void) result;

    switch (pdrv) {
        case DEV_RAM :
//		result = RAM_disk_initialize();

            // translate the reslut code here

            return stat;

        case DEV_MMC :
//		result = MMC_disk_initialize();

            // translate the reslut code here
            stat = 0;

            return stat;

        case DEV_USB :
//		result = USB_disk_initialize();

            // translate the reslut code here

            return stat;
    }
    return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read(
        BYTE pdrv,        /* Physical drive nmuber to identify the drive */
        BYTE *buff,        /* Data buffer to store read data */
        LBA_t sector,    /* Start sector in LBA */
        UINT count        /* Number of sectors to read */
) {
    DRESULT res = RES_ERROR;
    int result;
    (void) result;

    switch (pdrv) {
        case DEV_RAM :
            // translate the arguments here

//		result = RAM_disk_read(buff, sector, count);

            // translate the reslut code here

            return res;

        case DEV_MMC : {
            // translate the arguments here
            result_t res_mmc = mmc_driver_client_read(
                    fatfs_mmc_driver_client,
                    sector,
                    count,
                    global_mmc_block_size,
                    (char *) buff
            );
            if (result_is_err(res_mmc)) {
                result_printf(res_mmc);
                res = RES_ERROR;
            } else {
                res = RES_OK;
            }
            return res;
        }
        case DEV_USB :
            // translate the arguments here

//		result = USB_disk_read(buff, sector, count);

            // translate the reslut code here

            return res;
    }

    return RES_PARERR;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0

DRESULT disk_write(
        BYTE pdrv,            /* Physical drive nmuber to identify the drive */
        const BYTE *buff,    /* Data to be written */
        LBA_t sector,        /* Start sector in LBA */
        UINT count            /* Number of sectors to write */
) {
    DRESULT res = RES_ERROR;
    int result;
    (void) result;

    switch (pdrv) {
        case DEV_RAM :
            // translate the arguments here

//		result = RAM_disk_write(buff, sector, count);

            // translate the reslut code here

            return res;

        case DEV_MMC : {
            // translate the arguments here
            result_t res_mmc = mmc_driver_client_write(
                    fatfs_mmc_driver_client,
                    sector,
                    count,
                    global_mmc_block_size,
                    (char *) buff
            );
            if (result_is_err(res_mmc)) {
                result_printf(res_mmc);
                res = RES_ERROR;
            } else {
                res = RES_OK;
            }
            return res;
        }
        case DEV_USB :
            // translate the arguments here

//		result = USB_disk_write(buff, sector, count);

            // translate the reslut code here

            return res;
    }

    return RES_PARERR;
}

#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl(
        BYTE pdrv,        /* Physical drive nmuber (0..) */
        BYTE cmd,        /* Control code */
        void *buff        /* Buffer to send/receive control data */
) {
    DRESULT res = RES_ERROR;
    int result;
    (void) result;

    switch (pdrv) {
        case DEV_RAM :

            // Process of the command for the RAM drive

            return res;

        case DEV_MMC :
            switch (cmd) {
                case GET_SECTOR_COUNT: {
                    result_t res_mmc = mmc_driver_client_get_num_blocks(
                            fatfs_mmc_driver_client,
                            (uint64_t *) buff
                    );
                    if (result_is_err(res_mmc)) {
                        result_printf(res_mmc);
                        res = RES_ERROR;
                        break;
                    }
                    res = RES_OK;
                    break;
                }
                case GET_SECTOR_SIZE: {
                    break;
                }
                case GET_BLOCK_SIZE: {
                    result_t res_mmc = mmc_driver_client_get_block_size(
                            fatfs_mmc_driver_client,
                            (uint16_t *) buff
                    );
                    if (result_is_err(res_mmc)) {
                        result_printf(res_mmc);
                        res = RES_ERROR;
                        break;
                    }
                    res = RES_OK;
                    break;
                }
                case CTRL_SYNC: {
                    result_t res_mmc = mmc_driver_client_sync(
                            fatfs_mmc_driver_client
                    );
                    if (result_is_err(res_mmc)) {
                        result_printf(res_mmc);
                        res = RES_ERROR;
                        break;
                    }
                    res = RES_OK;
                    break;
                }
                case CTRL_TRIM: {
                    break;
                }
                default: {
                    break;
                }
            }
            // Process of the command for the MMC/SD card

            return res;

        case DEV_USB :

            // Process of the command the USB drive

            return res;
    }

    return RES_PARERR;
}

