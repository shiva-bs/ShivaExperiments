/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef DRIVER_FLASH_H
#define DRIVER_FLASH_H

/*
 * @FileName: DriverFlash.h
 * @ Author:
 * @ Date:
 * @ Project:Bootloader Platform
 * @ Organization: Actevia
 * @ Copyright (c) 2024 Actevia.
 * @ All rights reserved.
 */

/*
  @brief            Implementation of Driver Flashing

  @note             Functions are safe, as far as given pointers (with respect
                    to their lengths) point to valid memory ranges.

  @details          This module provides some of functions which helps to Flash the driver.
*/


/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
/* Exported types ------------------------------------------------------------*/
typedef enum 
{
	driverFlash_Division_NULL,
    driverFlash_Division_BL,
	driverFlash_Division_App,
	driverFlash_Division_Slot1,
	driverFlash_Division_Slot2,
    driverFlash_Division_count
} driverFlash_Division_t;


typedef enum
{
	DATA_TYPE_8=0,
	DATA_TYPE_16,
	DATA_TYPE_32,
}DataTypes;
/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/


/* Exported functions prototypes ---------------------------------------------*/
/**
 * @name driverFlash_FetchStartAddr
 * @brief Get start address of flash Division.
 * Physical start address of selected flash partiton.
 * @param Division The selected Division.
 * @return Physical start address.
 */
uint32_t driverFlash_FetchStartAddr(driverFlash_Division_t Division);

/**
 * @name driverFlash_FetchLen
 * @brief Get lengh of flash Division.
 * Lengh in bytes of selected flash partiton.
 * @param Division The selected Division.
 * @return Lengh in bytes.
 */
uint32_t driverFlash_FetchLen(driverFlash_Division_t Division);


/**
 * @name driverFlash_EraseDivision
 * @brief Perform Division erase.
 * Selected Division is erased and prepared for new
 * data.
 * @param Division The Division to erase.
 */
void driverFlash_EraseDivision(driverFlash_Division_t Division);

/**
 * @name driverFlash_WriteToMem
 * @brief Write data to flash memory.
 * Data is written to non volatile flash memory.
 * @param flashAddr The address to write to, must be 64bit aligned.
 * @param writeBuff The data to write.
 * @param dataSize size of data to be written.
 * @param dataType type of write data
 */
void driverFlash_WriteToMem(uint32_t flashAddr, void *writeBuff, uint32_t dataSize, DataTypes dataType);

/**
 * @name driverFlash_WriteToMem
 * @brief Read data from flash memory.
 * Data is read from non volatile flash memory.
 * @param Division The selected division
 * @param memOffset memory offset has to be considered 
 * @param readBuf read data buffer
 * @param dataType type of write data
 */
void driverFlash_ReadFromMem(driverFlash_Division_t Division, uint32_t memOffset, void *readBuf, uint32_t dataSize, DataTypes dataType);

/**
 * @name driverFlash_FetchSector
 * @brief Fetch sector for given division
 * Flash sector has been fetched for the given division
 * @param Division The slected division
 */
uint8_t driverFlash_FetchSector(driverFlash_Division_t Division);

#endif /* DRIVER_FLASH_H */
