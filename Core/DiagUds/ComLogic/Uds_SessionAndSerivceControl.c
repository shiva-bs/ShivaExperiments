/*
 * @FileName: Uds_SessionAndServiceControl.c
 * @ Author:
 * @ Date:
 * @ Project:Bootloader Platform
 * @ Organization: Actevia
 * @ Copyright (c) 2024 Actevia.
 * @ All rights reserved.
 */

/*
  @brief            Implementation of the Service and Session Control Module

  @note             Functions are safe, as far as given pointers (with respect
                    to their lengths) point to valid memory ranges.

  @details          This module provides the implementation of the Service and Session Control Module
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

#include <Interface_Clock.h>
#include <Interface_Debug.h>
#include <ISocket.h>
#include <stdbool.h>
#include <string.h>
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
/** Size of the UDS Receive Buffer in byte */
#define CHARON_RCV_BUFFER_SIZE      ((uint32_t) 4095u)

/** Size of the UDS Transmit Buffer in byte */
#define CHARON_TX_BUFFER_SIZE      ((uint32_t) 4095u)

#define DEFAULT_P2_SERVER            ((uint32_t) 50u)       /**< Default P2 Server Time (see ISO Requirements) */
#define DEFAULT_P2_STAR_SERVER       ((uint32_t) 5000u)     /**< Default P2* Server Time (see ISO Requirements) */
#define DEFAULT_S3_SERVER            ((uint32_t) 5000u)     /**< Default S3 Server Time (see ISO Requirements) */

/** Answer Requests are Request + Indication Bit */
#define UDS_RESPONSE_REQUEST_INDICATION_BIT_MASK         ((uint8_t) 0x40u)

/*==============================================================================
                  LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==============================================================================*/
/** Struct to store all timing required Data */
typedef struct
{
    uint32_t        p2Server;
    uint32_t        p2StarServer;
} ComTimeoutLimits_t;

/*==============================================================================
                               LOCAL CONSTANTS
==============================================================================*/

/*==============================================================================
                               LOCAL VARIABLES
==============================================================================*/
/** Stores the Currently Active Diagnostic Session */
static sessionTypes_t s_currentDiagnosticSession = sscType_default;

/** Pointer to last service that was requested pending */
static serviceObject_t * s_currentlyPendingService = NULL;

/** Timestamp for Pending Start */
static uint32_t s_pendingRequestStartTime = 0u;

/** Flag to indicate if the P2 Expired Message for Pending was already handled */
static bool s_p2PendingExceededHandled = false;

/** Timestamp for Diagnostic Session Timing */
static uint32_t s_diagnosticSessionTimestamp = 0u;

/** Adjustable timeouts, initialized with server default values */
static ComTimeoutLimits_t s_ttl =
{
        DEFAULT_P2_SERVER, DEFAULT_P2_STAR_SERVER
};

/** Stores the System given Communication Socket */
static ISocket_t s_systemComSocket = {NULL};

/** Charon Session and Communication Control Receive Buffer */
static uint8_t s_receiveBuffer[CHARON_RCV_BUFFER_SIZE];

/** Charon Session and Communication Control Transmit Buffer */
static uint8_t s_sendBuffer[CHARON_TX_BUFFER_SIZE];

/*==============================================================================
                               GLOBAL CONSTANTS
==============================================================================*/

/*==============================================================================
                               GLOBAL VARIABLES
==============================================================================*/

extern uint32_t bootAppMagicFlag;
extern bool uds_reset_request;

/*==============================================================================
                           LOCAL FUNCTION PROTOTYPES
==============================================================================*/
/**
* @Name-processReceivedMessage
* @details- Function handles reception of mssgs
* @param[out] - None
* @param[in]-pBuffer, length
* @return void
*/
static void processReceivedMessage (uint8_t const * const pBuffer, uint32_t length);

/**
* @Name-isServiceInSession
* @details- Function handles service sesssion
* @param[out] - None
* @param[in]- currentSession, pService
* @return Pointer to the filled block.
*/
static bool isServiceInSession (sessionTypes_t currentSession, const serviceObject_t * pService);

/**
* @Name-handleDiagnosticSession
* @details-Function handles Diagnostic sesssion
* @param[out] - None
* @param[in]- None
* @return Void
*/
static void handleDiagnosticSession (void);

/**
* @Name-handleResponsePending
* @details- Function handles pending responses
* @param[out] - None
* @param[in]- None
* @return Void
*/
static void handleResponsePending (void);

/**
* @Name-handleService
* @details-Function handles services
* @param[out] - None
* @param[in]-pExecutableService, pUdsMessage, length
* @return Pointer to the filled block.
*/
static Uds_RespCode_t handleService (const serviceObject_t * pExecutableService, uint8_t const * const pUdsMessage, uint32_t length);

/**
* @Name-sendMessage
* @details-Function handles sending of mssgs
* @param[out] - None
* @param[in]- pUdsMessage, length
* @return Pointer to the filled block.
*/
static void sendMessage (uint8_t const * const pUdsMessage, uint32_t length);

/*==============================================================================
                                LOCAL FUNCTIONS
==============================================================================*/

static bool isServiceInSession (sessionTypes_t currentSession, const serviceObject_t * pService)
{
    bool retval = false;
    uint32_t result = (uint32_t)((uint32_t)(1uL << currentSession) & pService->sessionMask);
    if(result > 0u)
    {
        retval = true;
    }

    return retval;
}

static void handleDiagnosticSession (void)
{
    /* Check if Session Timed Out */
    if(interface_clock_getTimeElapsed(s_diagnosticSessionTimestamp) >= DEFAULT_S3_SERVER)
    {
        LOG_WARNING("Session timed out, activating default session.");
        /* terminate Session */
        s_currentDiagnosticSession = sscType_timedOut;
    }
}

static void handleResponsePending (void)
{
    uint32_t tmp;

    /* Get all Time since Pending Request is started */
    tmp = interface_clock_getTimeElapsed(s_pendingRequestStartTime);

    /* Check if P2 was exceeded */
    if(!(s_p2PendingExceededHandled))
    {
        if(tmp >= s_ttl.p2Server)
        {
            s_p2PendingExceededHandled = true;
            udsSendNegativeResponse(uds_RespCode_RequestCorrectlyReceived_ResponsePending, s_currentlyPendingService->sid);
            s_pendingRequestStartTime = interface_clock_getTime();
        }
    }
    else /* Handle the P2* exceedings */
    {
        if(tmp >= s_ttl.p2StarServer)
        {
        	udsSendNegativeResponse(uds_RespCode_RequestCorrectlyReceived_ResponsePending, s_currentlyPendingService->sid);
            s_pendingRequestStartTime = interface_clock_getTime();
        }
        //TODO as far as i remember there was a maximum amount to do this, but i couldn't find it...
        // remark: you probably mean P4_Server, which is manufacturer specific
    }
}

static Uds_RespCode_t handleService (const serviceObject_t * pExecutableService, uint8_t const * const pUdsMessage, uint32_t length)
{
    Uds_RespCode_t retVal;

    /* Check if Service is supported */
    if (NULL != pExecutableService)
    {
        /* Check if Service is Supported in Current Session */
        if (isServiceInSession(s_currentDiagnosticSession, pExecutableService))
        {
            /* Execute Service */
            retVal = pExecutableService->serviceRunable(pUdsMessage, length);
        }
        else
        {
            /* Send Diag NRC */
            retVal = uds_RespCode_ServiceNotSupportedInActiveSession;
        }
    }
    else
    {
        /* Send not Supported NRC */
        retVal = uds_RespCode_ServiceNotSupported;
    }

    return retVal;
}

static void sendMessage (uint8_t const * const pUdsMessage, uint32_t length)
{
    uint32_t txLength;

    /**
     * FEATURE
     * Here needs to be evaluated if an encryption is active and the message needs encoding.
     */

    /* Transmit Message if it fits, otherwise trim */
    if(length >= CHARON_TX_BUFFER_SIZE)      //TODO Error Case?
    {
        txLength = CHARON_TX_BUFFER_SIZE;
    }
    else
    {
        txLength = length;
    }

    /* Copy to Buffer and start transfer */
    memcpy(s_sendBuffer, pUdsMessage, txLength);
    (void)s_systemComSocket.transmit(s_sendBuffer, txLength);
}

/*==============================================================================
                                GLOBAL FUNCTIONS
==============================================================================*/

/*============Uds_SessionAndServiceControl.h file for function description============*/

void sscReset (void)
{
    s_currentDiagnosticSession = sscType_default;
    s_currentlyPendingService = NULL;
    s_pendingRequestStartTime = 0u;
    s_p2PendingExceededHandled = false;
    s_diagnosticSessionTimestamp = 0u;
    s_ttl.p2Server = DEFAULT_P2_SERVER;
    s_ttl.p2StarServer = DEFAULT_P2_STAR_SERVER;
}

/*============Uds_SessionAndServiceControl.h file for function description============*/

void sscInit (ISocket_t sscComSocket)
{
    s_systemComSocket = sscComSocket;
}

/*============Uds_SessionAndServiceControl.h file for function description============*/

void sscCyclic (void)
{
    /* Check Diagnostic Session */
    if(sscType_default != s_currentDiagnosticSession)
    {
        handleDiagnosticSession();
    }

    /* Handle NRC Pending */
    if(NULL != s_currentlyPendingService)
    {
        handleResponsePending();
    }
    else
    {
        /* Reset Flag for P2 and P2* distinguish */
        s_p2PendingExceededHandled = false;
    }
}

/*============Uds_SessionAndServiceControl.h file for function description============*/

void sscRcvMessage (void)
{
    int32_t length;

    /* Get Message from Underlying Transport LL */
    length = s_systemComSocket.receive(s_receiveBuffer, sizeof(s_receiveBuffer));

    /**
     * FEATURE
     * Here needs to be evaluated if an encryption is active and the message needs translation.
     */

    /* Process Received Message if one was gotten and de-crypted */
    if(length > 0)
    {
        processReceivedMessage(s_receiveBuffer, (uint32_t)length);
    }
}

/*============Uds_SessionAndServiceControl.h file for function description============*/

static void processReceivedMessage (uint8_t const * const pBuffer, uint32_t length)
{
    serviceObject_t * pServiceObj = udsServiceLookupTable_getServiceObject((uint8_t)pBuffer[0]);   /* Get Service Object */
	Uds_RespCode_t retVal

    LOG_INFO("Message received, length: %i SID: %s", length, ServiceLookupTable_getName_ServiceSid((uint8_t)pBuffer[0]));

    /* Is a Service Pending, do not execute any other Requests except for Tester Present */
    if((NULL == s_currentlyPendingService)
            || (pServiceObj->sid == serviceId_TesterPresent))
    {
        retVal = handleService(pServiceObj, pBuffer, length);
        /* Check Return Value of Service Execution and Act accordingly */
        switch(retVal)
        {
        case uds_RespCode_ServiceNotSupportedInActiveSession:
        {
            LOG_WARNING("Wrong session for requested service, sending error message.");
            /* Answer NRC and Send */
            udsSendNegativeResponse(uds_RespCode_ServiceNotSupportedInActiveSession, pServiceObj->sid);
            break;
        }
        case uds_RespCode_ServiceNotSupported:
        {
            LOG_WARNING("Service not supported / not implemented, sending error message.");
            /* Construct Answer */
            Uds_ServiceId_t castedSid;
            /*
             * At this point the SID is extracted from the first Byte of the payload and the LINT warning is deliberately suppressed
             * since the ISO Requires to answer specifically with the SID that is not supported, even though it can be, that the SID
             * is not in the Enumeration.
             */
            castedSid = (Uds_ServiceId_t)pBuffer[0];  //TODO Suppress
            /* Send NRC */
            udsSendNegativeResponse(uds_RespCode_ServiceNotSupported, castedSid);
            break;
        }
        case uds_RespCode_RequestCorrectlyReceived_ResponsePending:
        {
            LOG_INFO("Service is executed, response pending.");
            /* Mark Ongoing Service and Begin with Pending Management */
            s_currentlyPendingService = pServiceObj;
            s_pendingRequestStartTime = interface_clock_getTime();
            break;
        }
        default:
        {
            //Do Nothing...
            LOG_INFO("Service returned %s.", ServiceLookupTable_getName_ReturnCode(retVal));
            break;
        }
        }
        if (retVal != uds_RespCode_RequestCorrectlyReceived_ResponsePending)
        {
            // completion of a service (which is indicated by not being "response pending") updates session timer
            s_diagnosticSessionTimestamp = interface_clock_getTime();
        }
    }
    else
    {
        /* Send Busy */
        LOG_WARNING("Server is busy, returning error code.");
        udsSendNegativeResponse(uds_RespCode_BusyRepeatRequest, pServiceObj->sid);
    }
}

/*============Uds_SessionAndServiceControl.h file for function description============*/

void sscTxMessage (uint8_t const * const pBuffer, uint32_t length)
{
    /* Check if a Request is pending */
    if(NULL != s_currentlyPendingService)
    {
        /* Copy First byte and Align to Request ID Matching to check if it was an ongoing Service that is now answered */
        uint8_t responeRequestId = (uint8_t)(((uint8_t)pBuffer[0]) & ((uint8_t)~UDS_RESPONSE_REQUEST_INDICATION_BIT_MASK));
        if(responeRequestId == (uint8_t)s_currentlyPendingService->sid)
        {
            LOG_INFO("Sending pending response, length %i", length);
            sendMessage(pBuffer, length);
            /* Reset Pending Request */
            s_currentlyPendingService = NULL;
        }
        else
        {
            // todo: how to handle this case?
            LOG_ERROR("Message SID %s (0x%x) does not match with pending SID %s (0x%x)", ServiceLookupTable_getName_ServiceSid(responeRequestId), responeRequestId, ServiceLookupTable_getName_ServiceSid((uint8_t)s_currentlyPendingService->sid), (uint8_t)s_currentlyPendingService->sid);
        }
    }
    else
    {
        /* Just send Synchronous Message */
        LOG_INFO("Sending synchronous response, length %i", length);
        sendMessage(pBuffer, length);
    }
}

/*============Uds_SessionAndServiceControl.h file for function description============*/

void sscSetSession (sessionTypes_t sessionType, uint32_t timeoutP2, uint32_t timeoutP2extended)
{
    if(sscType_default != sessionType)
    {
        /* Mark Beginning of an Diagnostic session that is not default session */
        s_diagnosticSessionTimestamp = interface_clock_getTime();
        /* Set new timings */
        s_ttl.p2Server = timeoutP2;
        s_ttl.p2StarServer = timeoutP2extended;
    }
    else
    {
        /* Fall Back to default timings */
        s_ttl.p2Server = DEFAULT_P2_SERVER;
        s_ttl.p2StarServer = DEFAULT_P2_STAR_SERVER;
    }

    /* Assign new Session Type */
    s_currentDiagnosticSession = sessionType;
    /*Diwakar: Reset Syetem in case default or extended session is received*/
    if(sscType_default == sessionType || sscType_extended == sessionType)
    {
    	/*Clear Magic flag*/
        /*Trigger Reset request*/
        bootAppMagicFlag=(uint32_t)0x0;
    	uds_reset_request= true;
    }

}

/*============Uds_SessionAndServiceControl.h file for function description============*/

sessionTypes_t sscGetSession (void)
{
    return s_currentDiagnosticSession;
}

/*============Uds_SessionAndServiceControl.h file for function description============*/

void sscTesterPresentHeartbeat (void)
{
    s_diagnosticSessionTimestamp = interface_clock_getTime();
}

/* -------------------------------------------- END OF FILE -------------------------------------------- */
/* =================================================================================
                                              FILE LOG HISTORY
=====================================================================================
*  Date    Changes      Author     Change_Set
*
*
*************************************************************************************/
