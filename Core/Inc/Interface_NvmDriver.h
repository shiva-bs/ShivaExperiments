/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef INTERFACE_NVMDRIVER_H_
#define INTERFACE_NVMDRIVER_H_
/*
 * @FileName: Interface_NvmDriver.h
 * @ Author:
 * @ Date:
 * @ Project:Bootloader Platform
 * @ Organization: Actevia
 * @ Copyright (c) 2024 Actevia.
 * @ All rights reserved.
 */

/*
  @brief            Implementation of functions which are required to use Nvm.

  @note             Functions are safe, as far as given pointers (with respect
                    to their lengths) point to valid memory ranges.

  @details          This module handles all functions which are required to use Nvm.
*/

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include <Uds_Types.h>
#include "driverFlash.h"

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/


/* Exported functions prototypes ---------------------------------------------*/

/**
* @Name-NvmDriver_checkAddressRange
* @details- Function fetches and returns image header when passing image sector as input
* @param[out] - None
* @param[in]  - sector: image sector index
* @return Pointer to the header.
* */
bool NvmDriver_checkAddressRange (uint32_t address, uint32_t length);

/**
* @Name-NvmDriver_write
* @details- Function writes given data to the desired memory location.
* @param[out] - None
* @param[in]  - address, data, size
* @return void
* */
Uds_RespCode_t NvmDriver_write (uint32_t address, const uint8_t* data, uint32_t size);

/**
* @Name-NvmDriver_read
* @details- Function reads data to given buffer from desired memory location.
* @param[out] - None
* @param[in]  - address, data, size
* @return void
* */
void NvmDriver_read (uint32_t address, uint8_t* data, uint32_t size);

/**
* @Name-NvmDriver_erase
* @details- Function Erases non volatile memory.
* @param[out] - None
* @param[in]  - Division
* @return void
* */
void NvmDriver_erase (driverFlash_Division_t division);

/**
* @Name-Assign_Division
* @details- Function Identifies the Division where OTA updates can be flashed
* @param[out] - None
* @param[in]  - None
* @return enum object
* */
driverFlash_Division_t  Assign_Division(void);

#endif /* INTERFACE_NVMDRIVER_H_ */
