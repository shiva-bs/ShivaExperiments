/*
 * @FileName: Interface_NvmDriver.c
 * @ Author:
 * @ Date:
 * @ Project:Bootloader Platform
 * @ Organization: Actevia
 * @ Copyright (c) 2024 Actevia.
 * @ All rights reserved.
 */

/*
  @brief            Implementation of NVM Driver functionality

  @note             Functions are safe, as far as given pointers (with respect
                    to their lengths) point to valid memory ranges.

  @details          This module handles NVM Driver functionality
*/

/*==============================================================================
                               MISRA VIOLATIONS
==============================================================================*/


/*==============================================================================
                                 INCLUDE FILES
 1) system and project includes
 2) needed interfaces from external units
 3) internal and external interfaces from this unit
==============================================================================*/
#include "driverFlash.h"
#include <string.h>
#include "BootManager.h"
#include <BootImage_Manager.h>
#include <Interface_NvmDriver.h>


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

/*==============================================================================
                               GLOBAL CONSTANTS
==============================================================================*/

/*==============================================================================
                               GLOBAL VARIABLES
==============================================================================*/

extern volatile driverFlash_Division_t Erased_Partion;

/*==============================================================================
                           LOCAL FUNCTION PROTOTYPES
==============================================================================*/
/**
* @Name-
* @details-
* @param[out] -
* @param[in]-
* @return
*/

/*==============================================================================
                                LOCAL FUNCTIONS
==============================================================================*/

/*==============================================================================
                                GLOBAL FUNCTIONS
==============================================================================*/

/*============Refer Interface_NvmDriver.h file for function description============*/

bool NvmDriver_checkAddressRange (uint32_t address, uint32_t length)
{
    uint32_t startAddress = (uint32_t)driverFlash_FetchStartAddr(Erased_Partion);
    uint32_t endAddress = startAddress+ driverFlash_FetchLen(Erased_Partion);

    bool rangeValid = false;
    if ( (address >= startAddress) && ((address+length)<= endAddress) )
    {
        rangeValid = true;
    }
    return rangeValid;
}

/*============Interface_NvmDriver.h file for function description============*/

Uds_RespCode_t NvmDriver_write (uint32_t address, const uint8_t* data, uint32_t size)
{
    Uds_RespCode_t response = uds_RespCode_PositiveResponse;
    if ((address & 0x7) != 0)
    {
      /* result = uds_RespCode_GeneralProgrammingFailure;*/
    }
    else if ((size & 0x7) != 0)
    {
      /*  result = uds_RespCode_GeneralProgrammingFailure; */
    }
    else
    {

      /* drvFlash_write(address, data, size, DATA_TYPE_8); */

    }
    /*Type casting "data" to void to avoid compiler warning*/
    driverFlash_WriteToMem(address,(void*) data, size, DATA_TYPE_8);
    return response;
}

/*============Refer Interface_NvmDriver.h file for function description============*/

void NvmDriver_read (uint32_t address, uint8_t* data, uint32_t size)
{
    memcpy(data, (const void*)address, size);
}

/*============Refer Interface_NvmDriver.h file for function description============*/

void NvmDriver_erase (driverFlash_Division_t division)
{
	driverFlash_EraseDivision(division);
}

/*============Refer Interface_NvmDriver.h file for function description============*/

/* Diwakar: This function is used to find the inactive Division Address to flash OTA updates */
driverFlash_Division_t  Assign_Division(void)
{
	const BootImage_Header_t *hdr1 = NULL;
	const BootImage_Header_t *hdr2 = NULL;

	hdr1 = imgFetchHeader(IMG_SECTOR_1);
	hdr2 = imgFetchHeader(IMG_SECTOR_2);
	driverFlash_Division_t InactiveDivision =driverFlash_Division_NULL;
	/*Check if Slot1 SW version is the active Division or Empty*/
   if( hdr1 == NULL)
	{
		//select slot 1

	    	InactiveDivision=driverFlash_Division_Slot1;
	}
   else if(hdr2 == NULL)
   {
	   InactiveDivision=driverFlash_Division_Slot2;
   }

   else	if((hdr1->img_version > hdr2->img_version) )
	{
		//Select Slot 2

	    	InactiveDivision=driverFlash_Division_Slot2;
	}
   else
   {
	   InactiveDivision=driverFlash_Division_Slot1;
   }

	return InactiveDivision;
}

/* -------------------------------------------- END OF FILE -------------------------------------------- */
/* =================================================================================
                                              FILE LOG HISTORY
=====================================================================================
*  Date    Changes      Author     Change_Set
*
*
*************************************************************************************/
