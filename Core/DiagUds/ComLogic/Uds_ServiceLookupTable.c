/*
 * @FileName: Uds_ServiceLookupTable.c
 * @ Author:
 * @ Date:
 * @ Project:Bootloader Platform
 * @ Organization: Actevia
 * @ Copyright (c) 2024 Actevia.
 * @ All rights reserved.
 */

/*
  @brief            Implementation of the Service Lookup Table

  @note             Functions are safe, as far as given pointers (with respect
                    to their lengths) point to valid memory ranges.

  @details          This module offers the Implementation of the Service Lookup Table
*/

/*==============================================================================
                               MISRA VIOLATIONS
==============================================================================
*/


/*==============================================================================
                                 INCLUDE FILES
 1) system and project includes
 2) needed interfaces from external units
 3) internal and external interfaces from this unit
==============================================================================*/

#include <Uds_DiagnosticAndCommunicationManagementFunctionalUnit.h>
#include <Uds_InputOutputControlFunctionalUnit.h>
#include <Uds_RoutineFunctionalUnit.h>
#include <Uds_ServiceLookupTable.h>
#include <Uds_SessionAndSerivceControl.h>
#include <Uds_UploadDownloadFunctionalUnit.h>

/*==============================================================================
                            FILE VERSION CHECKS
 ==============================================================================*/

/*==============================================================================
                                  IMPORTS
==============================================================================*/

/*==============================================================================
                                LOCAL MACROS
==============================================================================*/
#define SESSION_DEFAULT			((uint32_t) 1u << (uint8_t)sscType_default)
#define SESSION_PROGRAMMING  	((uint32_t) 1u << (uint8_t)sscType_programming)
#define SESSION_EXTENDED		((uint32_t) 1u << (uint8_t)sscType_extended)
#define SESSION_SECURED			((uint32_t) 1u << (uint8_t)sscType_secured)

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

static serviceObject_t udsServiceLookupTable[] =
        /* SID */                                           /* Allowed Sessions */                                                          /* Service Function */                                                                          /* Encryption */
{       {serviceId_DiagnosticSessionControl,                  (SESSION_DEFAULT | SESSION_PROGRAMMING | SESSION_EXTENDED | SESSION_SECURED),   DiagnosticAndCommunicationManagementFunctionalUnit_SrvDiagnosticSessionControl,             0u},
        {serviceId_EcuReset,                                  (SESSION_DEFAULT | SESSION_PROGRAMMING | SESSION_EXTENDED | SESSION_SECURED),   DiagnosticAndCommunicationManagementFunctionalUnit_SrvEcuReset,                             0u},
        {serviceId_ClearDiagnosticInformation,                (SESSION_DEFAULT | SESSION_PROGRAMMING | SESSION_EXTENDED | SESSION_SECURED),   NULL,                                                                                           0u},
        {serviceId_ReadDtcInformation,                        (SESSION_DEFAULT | SESSION_PROGRAMMING | SESSION_EXTENDED | SESSION_SECURED),   NULL,                                                                                           0u},
        {serviceId_ReadDataByIdentifier,                      (SESSION_DEFAULT | SESSION_PROGRAMMING | SESSION_EXTENDED | SESSION_SECURED),   User_Handler_22_Service,                                                                        0u},
        {serviceId_ReadMemoryByAddress,                       (SESSION_DEFAULT | SESSION_PROGRAMMING | SESSION_EXTENDED | SESSION_SECURED),   NULL,                                                                                           0u},
        {serviceId_ReadScalingDataByIdentifier,               (SESSION_DEFAULT | SESSION_PROGRAMMING | SESSION_EXTENDED | SESSION_SECURED),   NULL,                                                                                           0u},
        {serviceId_SecurityAccess,                            (                  SESSION_PROGRAMMING |  SESSION_EXTENDED | SESSION_SECURED),   DiagnosticAndCommunicationManagementFunctionalUnit_SrvSecurityAccess,                       0u},
        {serviceId_CommunicationControl,                      (                  SESSION_PROGRAMMING | SESSION_EXTENDED | SESSION_SECURED),   DiagnosticAndCommunicationManagementFunctionalUnit_SrvCommunicationControl,                 0u},
        {serviceId_ReadDataByPeriodicIdentifier,              (                                        SESSION_EXTENDED | SESSION_SECURED),   NULL,                                                                                           0u},
        {serviceId_DynamicallyDefineDataIdentifier,           (SESSION_DEFAULT | SESSION_PROGRAMMING | SESSION_EXTENDED | SESSION_SECURED),   NULL,                                                                                           0u},
        {serviceId_WriteDataByIdentifier,                     (SESSION_DEFAULT | SESSION_PROGRAMMING | SESSION_EXTENDED | SESSION_SECURED),   NULL,                                                                                           0u},
        {serviceId_InputOutputControlByIdentifier,            (                                        SESSION_EXTENDED | SESSION_SECURED),   InputOutputControlFunctionalUnit_SrvInputOutputControlByIdentifier,                         0u},
        {serviceId_RoutineControl,                            (                  SESSION_PROGRAMMING | SESSION_EXTENDED | SESSION_SECURED),   RoutineFunctionalUnit_SrvRoutineControl,                                                    0u},
        {serviceId_RequestDownload,                           (                  SESSION_PROGRAMMING | SESSION_EXTENDED | SESSION_SECURED),   UploadDownloadFunctionalUnit_SrvRequestDownload,                                            0u},
        {serviceId_RequestUpload,                             (                  SESSION_PROGRAMMING | SESSION_EXTENDED | SESSION_SECURED),   UploadDownloadFunctionalUnit_SrvRequestUpload,                                              0u},
        {serviceId_TransferData,                              (                  SESSION_PROGRAMMING | SESSION_EXTENDED | SESSION_SECURED),   UploadDownloadFunctionalUnit_SrvTransferData,                                               0u},
        {serviceId_RequestTransferExit,                       (                  SESSION_PROGRAMMING | SESSION_EXTENDED | SESSION_SECURED),   UploadDownloadFunctionalUnit_SrvRequestTransferExit,                                        0u},
        {serviceId_RequestFileTransfer,                       (                  SESSION_PROGRAMMING | SESSION_EXTENDED | SESSION_SECURED),   UploadDownloadFunctionalUnit_SrvRequestFileTransfer,                                        0u},
        {serviceId_WriteMemoryByAddress,                      (                  SESSION_PROGRAMMING | SESSION_EXTENDED | SESSION_SECURED),   NULL,                                                                                           0u},
        {serviceId_TesterPresent,                             (SESSION_DEFAULT | SESSION_PROGRAMMING | SESSION_EXTENDED | SESSION_SECURED),   DiagnosticAndCommunicationManagementFunctionalUnit_SrvTesterPresent,                        0u},
        {serviceId_AccessTimingParameter,                     (                  SESSION_PROGRAMMING | SESSION_EXTENDED | SESSION_SECURED),   DiagnosticAndCommunicationManagementFunctionalUnit_SrvAccessTimingParameter,                0u},
        {serviceId_SecuredDataTransmission,                   (                                                           SESSION_SECURED),   DiagnosticAndCommunicationManagementFunctionalUnit_SrvSecuredDataTransmission,              0u},
        {serviceId_ControlDtcSetting,                         (                                        SESSION_EXTENDED | SESSION_SECURED),   DiagnosticAndCommunicationManagementFunctionalUnit_SrvControlDtcSetting,                    0u},
        {serviceId_ResponseOnEvent,                           (SESSION_DEFAULT | SESSION_PROGRAMMING | SESSION_EXTENDED | SESSION_SECURED),   DiagnosticAndCommunicationManagementFunctionalUnit_SrvResponseOnEvent,                      0u},
        {serviceId_LinkControl,                               (                                        SESSION_EXTENDED | SESSION_SECURED),   DiagnosticAndCommunicationManagementFunctionalUnit_SrvLinkControl,                          0u},
};

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

/*==============================================================================udsServiceLookupTable
                                LOCAL FUNCTIONS
==============================================================================*/


/*==============================================================================
                                GLOBAL FUNCTIONS
==============================================================================*/

/*============Refer Uds_ServiceLookupTable.c file for function description============*/

serviceObject_t* udsServiceLookupTable_getServiceObject (uint8_t sid)
{
    const uint32_t TableSize = (sizeof(udsServiceLookupTable) / sizeof(udsServiceLookupTable[0]));
    uint32_t numIterations = 32u - (uint32_t)__builtin_clz(TableSize);
    uint32_t iterationSize = TableSize / 2u;
    uint32_t TableIndex = iterationSize;
    serviceObject_t * result = NULL;

    for (;numIterations > 0u;numIterations--)
    {
        iterationSize = (iterationSize+1u) / 2u;

        uint8_t castedSidEnumeration = (uint8_t)udsServiceLookupTable[TableIndex].sid;// TODO(Check for Misra at this point)
        if (castedSidEnumeration == sid)
        {
            result = &udsServiceLookupTable[TableIndex];
            break;
        }
        else if (castedSidEnumeration > sid)
        {
            if (TableIndex != 0u)
            {
                TableIndex -= iterationSize;
            }
        }
        else
        {
            TableIndex += iterationSize;
            if (TableIndex > (TableSize-1u) )
            {
                TableIndex = TableSize-1u;
            }
        }
    }
    return result;
}


/*============Refer Uds_ServiceLookupTable.c file for function description============*/

const char * udsServiceLookupTable_getNameForServiceSid (uint8_t sid)
{
    const char * result = "unknown sid";
    Uds_ServiceId_t castedSid = (Uds_ServiceId_t)sid;
    switch(castedSid)
    {
    case serviceId_DiagnosticSessionControl:
        result = "DiagnosticSessionControl";
        break;
    case serviceId_EcuReset:
        break;
    case serviceId_ClearDiagnosticInformation:
        result = "ClearDiagnosticInformation";
        break;
    case serviceId_ReadDtcInformation:
        result = "ReadDtcInformation";
        break;
    case serviceId_ReadDataByIdentifier:
        result = "ReadDataByIdentifier";
        break;
    case serviceId_ReadMemoryByAddress:
        result = "ReadMemoryByAddress";
        break;
    case serviceId_ReadScalingDataByIdentifier:
        result = "ReadScalingDataByIdentifier";
        break;
    case serviceId_SecurityAccess:
        result = "SecurityAccess";
        break;
    case serviceId_CommunicationControl:
        result = "CommunicationControl";
        break;
    case serviceId_ReadDataByPeriodicIdentifier:
        result = "ReadDataByPeriodicIdentifier";
        break;
    case serviceId_DynamicallyDefineDataIdentifier:
        result = "DynamicallyDefineDataIdentifier";
        break;
    case serviceId_WriteDataByIdentifier:
        result = "WriteDataByIdentifier";
        break;
    case serviceId_InputOutputControlByIdentifier:
        result = "InputOutputControlByIdentifier";
        break;
    case serviceId_RoutineControl:
        result = "RoutineControl";
        break;
    case serviceId_RequestDownload:
        result = "RequestDownload";
        break;
    case serviceId_RequestUpload:
        result = "RequestUpload";
        break;
    case serviceId_TransferData:
        result = "TransferData";
        break;
    case serviceId_RequestTransferExit:
        result = "RequestTransferExit";
        break;
    case serviceId_RequestFileTransfer:
        result = "RequestFileTransfer";
        break;
    case serviceId_WriteMemoryByAddress:
        result = "WriteMemoryByAddress";
        break;
    case serviceId_TesterPresent:
        result = "TesterPresent";
        break;
    case serviceId_PositiveResponseMask:
        result = "PositiveResponseMask";
        break;
    case serviceId_NegativeResponse:
        result = "NegativeResponse";
        break;
    case serviceId_AccessTimingParameter:
        result = "AccessTimingParameter";
        break;
    case serviceId_SecuredDataTransmission:
        result = "SecuredDataTransmission";
        break;
    case serviceId_ControlDtcSetting:
        result = "ControlDtcSetting";
        break;
    case serviceId_ResponseOnEvent:
        result = "ResponseOnEvent";
        break;
    case serviceId_LinkControl:
        result = "LinkControl";
        break;

    default:
        // nothing to do
        break;
    }
    return result;
}


/*============Refer Uds_ServiceLookupTable.c file for function description============*/

const char * udsServiceLookupTable_getNameForReturnCode (Uds_RespCode_t returnCode)
{
    const char * result = "unknown return code";
    switch (returnCode)
    {
    case uds_RespCode_PositiveResponse:
        result = "PositiveResponse";
        break;
    case uds_RespCode_GeneralReject:
        result = "GeneralReject";
        break;
    case uds_RespCode_ServiceNotSupported:
        result = "ServiceNotSupported";
        break;
    case uds_RespCode_SubfunctionNotSupported:
        result = "SubfunctionNotSupported";
        break;
    case uds_RespCode_IncorrectMessageLengthOrInvalidFormat:
        result = "IncorrectMessageLengthOrInvalidFormat";
        break;
    case uds_RespCode_ResponseTooLong:
        result = "ResponseTooLong";
        break;
    case uds_RespCode_BusyRepeatRequest:
        result = "BusyRepeatRequest";
        break;
    case uds_RespCode_ConditionsNotCorrect:
        result = "ConditionsNotCorrect";
        break;
    case uds_RespCode_RequestSequenceError:
        result = "RequestSequenceError";
        break;
    case uds_RespCode_NoResponseFromSubnetComponent:
        result = "NoResponseFromSubnetComponent";
        break;
    case uds_RespCode_FailurePreventsExecutionOfRequestedAction:
        result = "FailurePreventsExecutionOfRequestedAction";
        break;
    case uds_RespCode_RequestOutOfRange:
        result = "RequestOutOfRange";
        break;
    case uds_RespCode_SecurityAccessDenied:
        result = "SecurityAccessDenied";
        break;
    case uds_RespCode_InvalidKey:
        result = "InvalidKey";
        break;
    case uds_RespCode_ExceededNumberOfAttempts:
        result = "ExceededNumberOfAttempts";
        break;
    case uds_RespCode_RequiredTimeDelayNotExpired:
        result = "RequiredTimeDelayNotExpired";
        break;
    case uds_RespCode_UploadDownloadNotAccepted:
        result = "UploadDownloadNotAccepted";
        break;
    case uds_RespCode_TransferDataSuspended:
        result = "TransferDataSuspended";
        break;
    case uds_RespCode_GeneralProgrammingFailure:
        result = "GeneralProgrammingFailure";
        break;
    case uds_RespCode_WrongBlockSequenceCounter:
        result = "WrongBlockSequenceCounter";
        break;
    case uds_RespCode_RequestCorrectlyReceived_ResponsePending:
        result = "RequestCorrectlyReceived_ResponsePending";
        break;
    case uds_RespCode_SubfunctionNotSupportedInActiveSession:
        result = "SubfunctionNotSupportedInActiveSession";
        break;
    case uds_RespCode_ServiceNotSupportedInActiveSession:
        result = "ServiceNotSupportedInActiveSession";
        break;
    case uds_RespCode_RpmTooHigh:
        result = "RpmTooHigh";
        break;
    case uds_RespCode_RpmTooLow:
        result = "RpmTooLow";
        break;
    case uds_RespCode_EngineIsRunning:
        result = "EngineIsRunning";
        break;
    case uds_RespCode_EngineIsNotRunning:
        result = "EngineIsNotRunning";
        break;
    case uds_RespCode_EngineRunTimeTooLow:
        result = "EngineRunTimeTooLow";
        break;
    case uds_RespCode_TemperatureTooHigh:
        result = "TemperatureTooHigh";
        break;
    case uds_RespCode_TemperatureTooLow:
        result = "TemperatureTooLow";
        break;
    case uds_RespCode_VehicleSpeedTooHigh:
        result = "VehicleSpeedTooHigh";
        break;
    case uds_RespCode_VehicleSpeedTooLow:
        result = "VehicleSpeedTooLow";
        break;
    case uds_RespCode_ThrottlePedalTooHigh:
        result = "ThrottlePedalTooHigh";
        break;
    case uds_RespCode_ThrottlePedalTooLow:
        result = "ThrottlePedalTooLow";
        break;
    case uds_RespCode_TransmissionNotInNeutral:
        result = "TransmissionNotInNeutral";
        break;
    case uds_RespCode_TransmissionNotInGear:
        result = "TransmissionNotInGear";
        break;
    case uds_RespCode_BrakeSwitchesNotClosed:
        result = "BrakeSwitchesNotClosed";
        break;
    case uds_RespCode_ShifterLeverNotInPark:
        result = "ShifterLeverNotInPark";
        break;
    case uds_RespCode_TorqueConverterClutchLocked:
        result = "TorqueConverterClutchLocked";
        break;
    case uds_RespCode_VoltageTooHigh:
        result = "VoltageTooHigh";
        break;
    case uds_RespCode_VoltageTooLow:
        result = "VoltageTooLow";
        break;

    default:
        // nothing to do
        break;
    }
    return result;
}

/* -------------------------------------------- END OF FILE -------------------------------------------- */
/* =================================================================================
                                              FILE LOG HISTORY
=====================================================================================
*  Date    Changes      Author     Change_Set
*
*
*************************************************************************************/
