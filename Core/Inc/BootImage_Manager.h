/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef BOOTIMAGE_MANAGER_H
#define BOOTIMAGE_MANAGER_H

/*
 * @FileName: BootImage_Manager.h
 * @ Author:
 * @ Date:
 * @ Project:Bootloader Platform
 * @ Organization: Actevia
 * @ Copyright (c) 2024 Actevia.
 * @ All rights reserved.
 */

/*
  @brief            Implementation of Boot Image functionalities

  @note             Functions are safe, as far as given pointers (with respect
                    to their lengths) point to valid memory ranges.

  @details          This module provides some of functions which helps to fetch, validate and start the boot image.
*/


/* Includes ------------------------------------------------------------------*/
#include "types.h"

/* Exported types ------------------------------------------------------------*/
typedef struct __attribute__((packed)){
    uint16_t img_magic;
    uint16_t img_version;
    uint32_t img_size;
    uint32_t data_crc;

} BootImage_Header_t;


typedef enum {
	IMG_SECTOR_1 = 1,
	IMG_SECTOR_2 = 2

} BootImage_Sector_t;

/* Exported constants --------------------------------------------------------*/
#define IMAGE_MAGIC 0xABCD

#define FLASH_AREA_IMAGE_1   0x08020000        //sector 5 start
#define FLASH_AREA_IMAGE_2   0x08060000        //sector 7 start


#define IMAGE_OFFSET        0x400             //because vector table offset should be a multiple of 0x200

/* Exported macro ------------------------------------------------------------*/


/* Exported functions prototypes ---------------------------------------------*/
/**
* @Name-imgFetchHeader
* @details- Function fetches and returns image header when passing image sector as input
* @param[out] - None
* @param[in]  - sector: image sector index
* @return Pointer to the header.
* */

const BootImage_Header_t *imgFetchHeader(BootImage_Sector_t sector);

/**
* @Name-performImgValidation
* @details- Function validates image and returns status of image validation
* @param[out] - None
* @param[in]  - sector: image sector index
* @param[in]  - *img_hdr: image header
* @return status of image validation.
* */

uint8_t performImgValidation(BootImage_Sector_t sector, const BootImage_Header_t *img_hdr);

/**
* @Name-imgFetchHeader
* @details- Function returns image header when passing image sector as input
* @param[out] - None
* @param[in]  - *img_hdr: image header
* @return none.
* */

void imgStart(const BootImage_Header_t *img_hdr);

#endif /* BOOTIMAGE_MANAGER_H */
