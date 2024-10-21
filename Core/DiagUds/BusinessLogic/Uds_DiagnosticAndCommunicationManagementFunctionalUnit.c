/*
 * @FileName: Uds_DiagnosticAndCommunicationManagementFunctionalUnit.c
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

  @details          This module provides the implementations of SWCs which handles
                    the functionality of the Services namely, 0x10 0x11 0x27 0x28
                    0x3E 0x85.
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
#include <stdbool.h>
#include <Uds_DiagnosticAndCommunicationManagementFunctionalUnit.h>
#include <Uds_NegativeResponse.h>
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

const DefaultSessionTimings_t defaultTimings[sscType_amount] =
{
        {50,5000},      /* Default */
        {50,5000},      /* Programming */
        {50,5000},      /* Extended */
        {50,5000}       /* Security */
};
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
* @return Pointer to the filled block.
*/


/*==============================================================================
                                LOCAL FUNCTIONS
==============================================================================*/


/*==============================================================================
                                GLOBAL FUNCTIONS
==============================================================================*/

/*============Refer Uds_DiagnosticAndCommunicationManagementFunctionalUnit.h file for function description============*/

void DiagnosticAndCommunicationManagementFunctionalUnit_reset (void)
{

}

/*============Refer Uds_DiagnosticAndCommunicationManagementFunctionalUnit.h file for function description============*/

Uds_RespCode_t DiagnosticAndCommunicationManagementFunctionalUnit_SrvDiagnosticSessionControl (const uint8_t * receive_Buff, uint32_t receive_Buff_Size)
{
    Uds_RespCode_t response = uds_RespCode_PositiveResponse;
    PACKED_STRUCT(anonym)
    {
        uint8_t  sid;
        uint8_t  session;
        uint16_t p2;
        uint16_t p2star;
    } transmit_Buff;

    if (receive_Buff_Size != 2u)
    {
        LOG_ERROR("Unexpected message length.");
        response = uds_RespCode_IncorrectMessageLengthOrInvalidFormat;
    }
    else
    {
        uint8_t session = receive_Buff[1] & 0x7Fu;
        uint8_t responseSuppress = receive_Buff[1] & 0x80u;
        if ( (session >= (uint8_t)sscType_amount) || (session == (uint8_t)sscType_invalid) )
        {
            LOG_WARNING("Session 0x%x unknown or invalid!", session);
            response = uds_RespCode_SubfunctionNotSupported;
        }
        else
        {
            LOG_INFO("Changing Session to 0x%x.", session);

            if (responseSuppress == 0u)
            {
                transmit_Buff.sid = (uint8_t)serviceId_DiagnosticSessionControl | (uint8_t)serviceId_PositiveResponseMask;
                transmit_Buff.session = session;
                transmit_Buff.p2 =  (uint16_t)defaultTimings[session].p2;
                transmit_Buff.p2star = (uint16_t)(defaultTimings[session].p2star/10u);
                sscTxMessage((uint8_t*)&transmit_Buff, sizeof(transmit_Buff));
            }
            sscSetSession((sessionTypes_t)session, defaultTimings[session].p2, defaultTimings[session].p2star);
        }
    }
    if (response != uds_RespCode_PositiveResponse)
    {
    	udsSendNegativeResponse(response, serviceId_DiagnosticSessionControl);
    }
    return response;
}


/*============Refer Uds_DiagnosticAndCommunicationManagementFunctionalUnit.h file for function description============*/

Uds_RespCode_t DiagnosticAndCommunicationManagementFunctionalUnit_SrvEcuReset (const uint8_t * receive_Buff, uint32_t receive_Buff_Size)
{
 /*Diwakar: Implemented 11 01 service and subfunction*/
    uint8_t transmit_Buff[2];
    extern bool uds_reset_request;
    Uds_RespCode_t response = uds_RespCode_PositiveResponse;
    LOG_INFO("ECU Reset Service SID:0x11 Triggered");
    if (receive_Buff_Size != 2u)
     {
         LOG_ERROR("Unexpected message length.");
         response = uds_RespCode_IncorrectMessageLengthOrInvalidFormat;
     }
     else if  ( receive_Buff[1] != 0x01 )
     {
             LOG_WARNING("Session 0x%x unknown or invalid!", session);
             response = uds_RespCode_SubfunctionNotSupported;
     }

     else
     {
         uint8_t responseSuppress = receive_Buff[1] & 0x80u;
         if(responseSuppress==0)
         {
        uds_reset_request = true;
         transmit_Buff[0]= (uint8_t)serviceId_EcuReset | (uint8_t)serviceId_PositiveResponseMask;
         transmit_Buff[1]= receive_Buff[1];
         sscTxMessage((uint8_t*)&transmit_Buff, sizeof(transmit_Buff));

         }
         else
         {
        	 uds_reset_request = true;
         }
     }
    if (response != uds_RespCode_PositiveResponse)
      {
    	udsSendNegativeResponse(response, serviceId_EcuReset);
      }
    return response;
 }


/*============Refer Uds_DiagnosticAndCommunicationManagementFunctionalUnit.h file for function description============*/

Uds_RespCode_t DiagnosticAndCommunicationManagementFunctionalUnit_SrvSecurityAccess (const uint8_t * receive_Buff, uint32_t receive_Buff_Size)
{
    (void)receive_Buff;
    (void)receive_Buff_Size;
    LOG_INFO("Security Access SID:0x27 Triggered");
    return uds_RespCode_ServiceNotSupported;
}


/*============Refer Uds_DiagnosticAndCommunicationManagementFunctionalUnit.h file for function description============*/

Uds_RespCode_t DiagnosticAndCommunicationManagementFunctionalUnit_SrvCommunicationControl (const uint8_t * receive_Buff, uint32_t receive_Buff_Size)
{
    (void)receive_Buff;
    (void)receive_Buff_Size;
    LOG_INFO("Com Control Service SID:0x28 Triggered");
    return uds_RespCode_ServiceNotSupported;
}


/*============Refer Uds_DiagnosticAndCommunicationManagementFunctionalUnit.h file for function description============*/

Uds_RespCode_t DiagnosticAndCommunicationManagementFunctionalUnit_SrvTesterPresent (const uint8_t * receive_Buff, uint32_t receive_Buff_Size)
{
    Uds_RespCode_t response = uds_RespCode_PositiveResponse;

    if (receive_Buff_Size != 2u)
    {
        LOG_ERROR("Unexpected message length.");
        response = uds_RespCode_IncorrectMessageLengthOrInvalidFormat;
    }
    else if ( (receive_Buff[1] & 0x7Fu) != 0u)
    {
        LOG_ERROR("Subfunction is not 0x00 or 0x80.");
        response = uds_RespCode_SubfunctionNotSupported;
    }
    else
    {
        sscTesterPresentHeartbeat();
        if ( (receive_Buff[1] & 0x80u) == 0u)
        {
            uint8_t transmit_Buff[2] = {(uint8_t)serviceId_TesterPresent | (uint8_t)serviceId_PositiveResponseMask, 0u};
            sscTxMessage(transmit_Buff, sizeof(transmit_Buff));
        }
    }

    if (response != uds_RespCode_PositiveResponse)
    {
    	udsSendNegativeResponse(response, serviceId_TesterPresent);
    }

    LOG_INFO("Tester Present Service SID:0x3E Triggered");
    return response;
}


/*============Refer Uds_DiagnosticAndCommunicationManagementFunctionalUnit.h file for function description============*/

Uds_RespCode_t DiagnosticAndCommunicationManagementFunctionalUnit_SrvAccessTimingParameter (const uint8_t * receive_Buff, uint32_t receive_Buff_Size)
{
    (void)receive_Buff;
    (void)receive_Buff_Size;
    LOG_INFO("Access Timing Parameter Service SID:0x83 Triggered");
    return uds_RespCode_ServiceNotSupported;
}


/*============Refer Uds_DiagnosticAndCommunicationManagementFunctionalUnit.h file for function description============*/

Uds_RespCode_t DiagnosticAndCommunicationManagementFunctionalUnit_SrvSecuredDataTransmission (const uint8_t * receive_Buff, uint32_t receive_Buff_Size)
{
    (void)receive_Buff;
    (void)receive_Buff_Size;
    LOG_INFO("Secured Data Transmission Service SID:0x84 Triggered");
    return uds_RespCode_ServiceNotSupported;
}


/*============Refer Uds_DiagnosticAndCommunicationManagementFunctionalUnit.h file for function description============*/

Uds_RespCode_t DiagnosticAndCommunicationManagementFunctionalUnit_SrvControlDtcSetting (const uint8_t * receive_Buff, uint32_t receive_Buff_Size)
{
    (void)receive_Buff;
    (void)receive_Buff_Size;
    LOG_INFO("Control DTC Setting Service SID:0x85 Triggered");
    return uds_RespCode_ServiceNotSupported;
}


/*============Refer Uds_DiagnosticAndCommunicationManagementFunctionalUnit.h file for function description============*/

Uds_RespCode_t DiagnosticAndCommunicationManagementFunctionalUnit_SrvResponseOnEvent (const uint8_t * receive_Buff, uint32_t receive_Buff_Size)
{
    (void)receive_Buff;
    (void)receive_Buff_Size;
    LOG_INFO("Response On Event Service SID:0x86 Triggered");
    return uds_RespCode_ServiceNotSupported;
}



/*============Refer Uds_DiagnosticAndCommunicationManagementFunctionalUnit.h file for function description============*/

Uds_RespCode_t DiagnosticAndCommunicationManagementFunctionalUnit_SrvLinkControl (const uint8_t * receive_Buff, uint32_t receive_Buff_Size)
{
    (void)receive_Buff;
    (void)receive_Buff_Size;
    LOG_INFO("Link Control Service SID:0x87 Triggered");
    return uds_RespCode_ServiceNotSupported;
}


/*============Refer Uds_DiagnosticAndCommunicationManagementFunctionalUnit.h file for function description============*/

/*Diwakar:  Service 22 handler to return session*/
Uds_RespCode_t User_Handler_22_Service(const uint8_t * receive_Buff, uint32_t receive_Buff_Size)
{
	Uds_RespCode_t response = uds_RespCode_PositiveResponse;
	   PACKED_STRUCT(anonym)
	    {
	        uint8_t sid;
	        uint8_t did_Byte1;
	        uint8_t did_Byte2;
	        sessionTypes_t session;
	    } transmit_Buff;
     uint8_t TranmitBuffer_PartNumber[8]={'B','O','O','T','V','E','R',01};
	    if(receive_Buff_Size !=0x3)
	    {
	          LOG_ERROR("Unexpected message length.");
	          response = uds_RespCode_IncorrectMessageLengthOrInvalidFormat;
	    }
	    else
	    {
	       if(receive_Buff[1]==0xF1 && receive_Buff[2]==0x86)
	        {
	    	   transmit_Buff.session = sscGetSession();
	    	   transmit_Buff.sid=  (uint8_t)serviceId_ReadDataByIdentifier | (uint8_t)serviceId_PositiveResponseMask;
	    	   transmit_Buff.did_Byte1= receive_Buff[1];
	    	   transmit_Buff.did_Byte2= receive_Buff[2];
	    	   sscTxMessage((uint8_t*)&transmit_Buff, sizeof(transmit_Buff));

	        }
	       else if(receive_Buff[1]==0xF1 && receive_Buff[2]==0x24)
	       {
	   	    /*Part Number Implementation*/

	   	     sscTxMessage((uint8_t*)&TranmitBuffer_PartNumber, sizeof(TranmitBuffer_PartNumber));
	       }
	       else
	       {
	    	   response = uds_RespCode_RequestOutOfRange;
	       }


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
