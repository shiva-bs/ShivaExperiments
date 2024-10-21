/*
 * @FileName: DriverFlash.c
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

/*==============================================================================
                               MISRA VIOLATIONS
==============================================================================*/
/*


*/

/*==============================================================================
                                 INCLUDE FILES
 1) system and project includes
 2) needed interfaces from external units
 3) internal and external interfaces from this unit
==============================================================================*/
#include <DriverFlash.h>
#include "stm32f4xx.h"

/*==============================================================================
                            FILE VERSION CHECKS
 ==============================================================================*/

/*==============================================================================
                                  IMPORTS
==============================================================================*/

/*==============================================================================
                                LOCAL MACROS
==============================================================================*/


/*==============================================================================
                  LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==============================================================================*/

/*==============================================================================
                               LOCAL CONSTANTS
==============================================================================*/

/*==============================================================================
                               LOCAL VARIABLES
==============================================================================*/
static const struct
{  uint8_t sector;
   uint32_t startaddress;
    uint32_t length;
} DivisionLookupTable [driverFlash_Division_count] = 
{
	{0,0x00000000, 0 * 0},
	{0,0x08000000, 128 * 1024},
    {5,0x08020000, 128 * 1024},
	{5,0x08020000, 128 * 1024},
    {7,0x08060000, 128 * 1024}
};/*Limitation: Each Division consist of single sector only. It can be later implemented to accomodate more sector per Division*/

/*==============================================================================
                               GLOBAL CONSTANTS
==============================================================================*/

/*==============================================================================
                               GLOBAL VARIABLES
==============================================================================*/

/*==============================================================================
                           LOCAL FUNCTION PROTOTYPES
==============================================================================*/
/**
* @Name-
* @details-
* @param[out] -
* @param[in]-
* @return.
*/

/*==============================================================================
                                LOCAL FUNCTIONS
==============================================================================*/


/*==============================================================================
                                GLOBAL FUNCTIONS
==============================================================================*/


/*============Refer DriverFlash.h file for function description============*/

const uint32_t driverFlash_FetchStartAddr(driverFlash_Division_t Division)
{
    return DivisionLookupTable[Division].startaddress;
}

/*===============================================================================*/

/*============Refer DriverFlash.h file for function description============*/

uint32_t driverFlash_FetchLen(driverFlash_Division_t Division)
{
    return DivisionLookupTable[Division].length;
}

/*===============================================================================*/

/*============Refer DriverFlash.h file for function description============*/

uint8_t driverFlash_FetchSector(driverFlash_Division_t Division)
{
    return DivisionLookupTable[Division].sector;
}

/*===============================================================================*/

/*============Refer DriverFlash.h file for function description============*/

void driverFlash_EraseDivision(driverFlash_Division_t Division)
{
   uint8_t getSector=driverFlash_FetchSector(Division);
   	HAL_FLASH_Unlock();
	//Erase the required Flash sector
	FLASH_Erase_Sector(getSector, FLASH_VOLTAGE_RANGE_3);
	HAL_FLASH_Lock();

}

/*===============================================================================*/

/*============Refer DriverFlash.h file for function description============*/

void driverFlash_WriteToMem(uint32_t flashAddr, void *writeBuff, uint32_t dataSize, DataTypes dataType)
{
	//Unlock Flash
	HAL_FLASH_Unlock();
	//Write to Flash
	 __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_PGSERR | FLASH_FLAG_PGPERR);
	switch(dataType)
	{
		case DATA_TYPE_8:
				for(uint32_t i=0; i<dataSize; i++)
				{

					HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, flashAddr , ((uint8_t *)writeBuff)[i]);
					flashAddr++;
				}
			break;
		
		case DATA_TYPE_16:
				for(uint32_t i=0; i<dataSize; i++)
				{
					HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, flashAddr , ((uint16_t *)writeBuff)[i]);
					flashAddr+=2;
				}
			break;
		
		case DATA_TYPE_32:
				for(uint32_t i=0; i<dataSize; i++)
				{
					HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, flashAddr , ((uint32_t *)writeBuff)[i]);
					flashAddr+=4;
				}
			break;
	}
	//Lock the Flash space
	HAL_FLASH_Lock();
}

/*===============================================================================*/

/*============Refer DriverFlash.h file for function description============*/

void driverFlash_ReadFromMem(driverFlash_Division_t Division, uint32_t memOffset, void *readBuf, uint32_t dataSize, DataTypes dataType)
{
	uint32_t flashAddress = (driverFlash_FetchStartAddr(Division) + memOffset);
	
	switch(dataType)
	{
		case DATA_TYPE_8:
				for(uint32_t i=0; i<dataSize; i++)
				{
					*((uint8_t *)readBuf + i) = *(uint8_t *)flashAddress;
					flashAddress++;
				}
			break;
		
		case DATA_TYPE_16:
				for(uint32_t i=0; i<dataSize; i++)
				{
					*((uint16_t *)readBuf + i) = *(uint16_t *)flashAddress;
					flashAddress+=2;
				}
			break;
		
		case DATA_TYPE_32:
				for(uint32_t i=0; i<dataSize; i++)
				{
					*((uint32_t *)readBuf + i) = *(uint32_t *)flashAddress;
					flashAddress+=4;
				}
			break;
	} 

}

/*===============================================================================*/

/* -------------------------------------------- END OF FILE -------------------------------------------- */
/* =================================================================================
                                              FILE LOG HISTORY
=====================================================================================
*  Date    Changes      Author     Change_Set
*
*
*************************************************************************************/


