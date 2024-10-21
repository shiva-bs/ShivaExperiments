/*
 * @FileName: Uds_App_Interface.c
 * @ Author:
 * @ Date:
 * @ Project:Bootloader Platform
 * @ Organization: Actevia
 * @ Copyright (c) 2024 Actevia.
 * @ All rights reserved.
 */

/*
  @brief            Implementation Implementation of Application Interfaces

  @note             Functions are safe, as far as given pointers (with respect
                    to their lengths) point to valid memory ranges.

  @details          This module provides some of functions of Application Interfaces
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

#include <stdbool.h>
#include <Uds_App_Interface.h>

#include <Uds_DiagnosticAndCommunicationManagementFunctionalUnit.h>
#include <Uds_SessionAndSerivceControl.h>
#include <Uds_UploadDownloadFunctionalUnit.h>
#if (!(CHARON_CONFIG_DO_NOT_PRINT_WELCOME))
#include <Interface_Debug.h>
#endif

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
  extern bool uds_reset_request;

/*==============================================================================
                           LOCAL FUNCTION PROTOTYPES
==============================================================================*/
/**
* @Name-printUnnecessaryLargeWelcome
* @details- For displaying welcome mssg
* @param[out] - None
* @param[in]- None
* @return void
*/
 static void printUnnecessaryLargeWelcome (void);


/*==============================================================================
                                LOCAL FUNCTIONS
==============================================================================*/
 static void printUnnecessaryLargeWelcome (void)
 {
     /*Add Welcome Message if required*/
 }

/*==============================================================================
                                GLOBAL FUNCTIONS
==============================================================================*/

/*============Refer Uds_App_Interface.c file for function description============*/

void reset (void)
{
    sscReset();
    DiagnosticAndCommunicationManagementFunctionalUnit_reset();
    UploadDownloadFunctionalUnit_reset();
}

/*============Refer BootImage_Manager.h file for function description============*/

void init (ISocket_t systemCommunicationSocket)
{
    reset();
    sscInit(systemCommunicationSocket);
#if (!(CHARON_CONFIG_DO_NOT_PRINT_WELCOME))
    printUnnecessaryLargeWelcome();
#endif
}

/*============Refer Uds_App_Interface.c file for function description============*/

void task (void)
{
    /* Process Received Data */
    sscRcvMessage();

    /* Process SSC Layer */
    sscCyclic();

    /* Reset library if session has ended */
    if (sscGetSession() == sscType_timedOut)
    {
        reset();
		/*Diwakar: Reset System On timeout*/
		uds_reset_request= true;
    }

    return;
}

/* -------------------------------------------- END OF FILE -------------------------------------------- */
/* =================================================================================
                                              FILE LOG HISTORY
=====================================================================================
*  Date    Changes      Author     Change_Set
*
*
*************************************************************************************/

