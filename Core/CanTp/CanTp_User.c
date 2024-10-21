/*
 * @FileName: BootImage_Manager.c
 * @ Author:
 * @ Date:
 * @ Project:Bootloader Platform
 * @ Organization: Actevia
 * @ Copyright (c) 2024 Actevia.
 * @ All rights reserved.
 */

/*
  @brief            Implementation of Can TP Debug,send data and Get MS

  @note             Functions are safe, as far as given pointers (with respect
                    to their lengths) point to valid memory ranges.

  @details          This module provides some of functions which helps to debug, send data and get time in ms.
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
#include <CanTp_Defines.h>
#include <CanTp_User.h>
#include "DriverCan.h"
#include "driverClock.h"

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
* @Name-FuntionName
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


/*============Refer CanTp_User.h file for function description============*/
void canTp_User_Debug(const char* message, ...)
{
    (void) message;
    return;
}
/*===============================================================================*/

/*============Refer CanTp_User.h file for function description============*/
int canTp_User_Send_Can(const uint32_t arbitration_id, const uint8_t* data, const uint8_t size)
{
    bool result = driverCan_TransmitMessage(arbitration_id, size, data);
    return result ? CANTP_PROTOCOL_RESULT_OK : CANTP_PROTOCOL_RESULT_BUFFER_OVFLW;
}
/*===============================================================================*/

/*============Refer CanTp_User.h file for function description============*/
uint32_t canTp_User_GetMs(void)
{
    return driverClock_FetchRunTime();
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