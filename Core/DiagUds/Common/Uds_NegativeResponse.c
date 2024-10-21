/*
 * @FileName: Uds_negativeResponse.c
 * @ Author:
 * @ Date:
 * @ Project:Bootloader Platform
 * @ Organization: Actevia
 * @ Copyright (c) 2024 Actevia.
 * @ All rights reserved.
 */

/*
  @brief            Implementation for the Transfer of the negative Response message.

  @note             Functions are safe, as far as given pointers (with respect
                    to their lengths) point to valid memory ranges.

  @details          This Module handles the Transfer of the negative Response message.
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

#include <Interface_Debug.h>
#include <Uds_NegativeResponse.h>
#include <Uds_ServiceLookupTable.h>
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

/*============Uds_negativeResponse.h file for function description============*/

void udsSendNegativeResponse (Uds_RespCode_t ResponseCode, Uds_ServiceId_t RequestedSid)
{
    LOG_INFO("Sending error message %s (0x%x) for SID %s (0x%x)", ServiceLookupTable_getName_ReturnCode(ResponseCode), (uint8_t)ResponseCode, ServiceLookupTable_getName_ServiceSid((uint8_t)RequestedSid), (uint8_t)RequestedSid);
    uint8_t transmit_Buff[3] = {(uint8_t)serviceId_NegativeResponse, (uint8_t)RequestedSid, (uint8_t)ResponseCode};
    sscTxMessage(transmit_Buff, sizeof(transmit_Buff));
}

/* -------------------------------------------- END OF FILE -------------------------------------------- */
/* =================================================================================
                                              FILE LOG HISTORY
=====================================================================================
*  Date    Changes      Author     Change_Set
*
*
*************************************************************************************/
