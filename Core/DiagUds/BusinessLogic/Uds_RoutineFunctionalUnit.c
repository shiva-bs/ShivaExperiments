/*
 * @FileName: Uds_RoutineFunctionalUnit.c
 * @ Author:
 * @ Date:
 * @ Project:Bootloader Platform
 * @ Organization: Actevia
 * @ Copyright (c) 2024 Actevia.
 * @ All rights reserved.
 */

/*
  @brief            Implementation of DCM Module functionalities

  @note             Functions are safe, as far as given pointers (with respect
                    to their lengths) point to valid memory ranges.

  @details          This module provides the implementation of SWCs which handles
                    the functionality of the Service namely, 0x31
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

#include <Interface_NvmDriver.h>
#include <Uds_NegativeResponse.h>
#include <Uds_RoutineFunctionalUnit.h>
#include <Uds_SessionAndSerivceControl.h>


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
/* Diwakar: Store the erased Division Info. This Division will be used for OTA update. This is a temporary workAround */
volatile driverFlash_Division_t Erased_Partion= driverFlash_Division_NULL;

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
* @Name-imgFetchHeader
* @details-
* @param[out] -
* @param[in]-
* @return Pointer to the filled block.
*/

/*==============================================================================
                                LOCAL FUNCTIONS
==============================================================================*/


/*==============================================================================
                                GLOBAL FUNCTIONS
==============================================================================*/

/*============Refer Uds_RoutineFunctionalUnit.h file for function description============*/

Uds_RespCode_t RoutineFunctionalUnit_SrvRoutineControl (const uint8_t * rcvDataBuff, uint32_t rcvDataBuffSize)
{
    Uds_RespCode_t response = uds_RespCode_PositiveResponse;

    if (rcvDataBuffSize < 4u)
    {
        response = uds_RespCode_IncorrectMessageLengthOrInvalidFormat;
    }
    else if ( (rcvDataBuff[1] == 0u) || (rcvDataBuff[1] > 3u) )
    {
        response = uds_RespCode_SubfunctionNotSupported;
    }
    else
    {
        uint16_t routine_ID = ((uint16_t)rcvDataBuff[2] << 8) | rcvDataBuff[3];
        if ( (routine_ID == 0xFF00u) && (rcvDataBuff[1] == 1u) )
        {
        	/*Diwakar: Workaround: Assigned the already erase partion. Reading a erased partion in assinedDivision api do not return 0xFFFF hence it erases the other Division*/
        	if(Erased_Partion == driverFlash_Division_NULL)
        	{
        	Erased_Partion=Assign_Division();
            }

            NvmDriver_erase(Erased_Partion);
        }

    }

    if (response != uds_RespCode_PositiveResponse)
    {
    	udsSendNegativeResponse(response, serviceId_RoutineControl);
    }
    else
    {
        uint8_t transmit_Buff[4] = {
                rcvDataBuff[0] | (uint8_t)serviceId_PositiveResponseMask,
                rcvDataBuff[1],
                rcvDataBuff[2],
                rcvDataBuff[3]
        };
        sscTxMessage(transmit_Buff, sizeof(transmit_Buff));
    }
    return response;
}

/* -------------------------------------------- END OF FILE -------------------------------------------- */
/* =================================================================================
                                              FILE LOG HISTORY
=====================================================================================
*  Date    Changes      Author     Change_Set
*
*
*************************************************************************************/
