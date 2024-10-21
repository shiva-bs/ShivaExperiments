
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef UDS_TYPES_H
#define UDS_TYPES_H

/*
 * @FileName: Uds_Types.h
 * @ Author:
 * @ Date:
 * @ Project:Bootloader Platform
 * @ Organization: Actevia
 * @ Copyright (c) 2024 Actevia.
 * @ All rights reserved.
 */

/*
  @brief            UDS Data Types

  @note             Functions are safe, as far as given pointers (with respect
                    to their lengths) point to valid memory ranges.

  @details
*/


/** Define NULL for Compatibility across libraries and Compilers */
#ifndef NULL
#define NULL        ((void *) 0)
#endif

/** Helper Macro to define a Byte Packed struct depending on compiler */
#ifdef __GNUC__
#define PACKED_STRUCT(name)             \
struct __attribute__((packed)) name
#endif
#ifdef __arm__
#define PACKED_STRUCT(name)             \
struct __attribute__((packed)) name
#endif

/* Includes ------------------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
typedef enum
{
    uds_messageType_Diagnostics,
    uds_messageType_RemoteDiagnostics,
} Uds_MessageType_t;

typedef enum
{
    uds_targetAddressType_Physical,
    uds_targetAddressType_Functional,
} Uds_TargetAddressType_t;

typedef enum
{
    serviceId_DiagnosticSessionControl = 0x10u,
    serviceId_EcuReset = 0x11u,
    serviceId_ClearDiagnosticInformation = 0x14u,
    serviceId_ReadDtcInformation = 0x19u,
    serviceId_ReadDataByIdentifier = 0x22u,
    serviceId_ReadMemoryByAddress = 0x23u,
    serviceId_ReadScalingDataByIdentifier = 0x24u,
    serviceId_SecurityAccess = 0x27u,
    serviceId_CommunicationControl = 0x28u,
    serviceId_ReadDataByPeriodicIdentifier = 0x2Au,
    serviceId_DynamicallyDefineDataIdentifier = 0x2Cu,
    serviceId_WriteDataByIdentifier = 0x2Eu,
    serviceId_InputOutputControlByIdentifier = 0x2Fu,
    serviceId_RoutineControl = 0x31u,
    serviceId_RequestDownload = 0x34u,
    serviceId_RequestUpload = 0x35u,
    serviceId_TransferData = 0x36u,
    serviceId_RequestTransferExit = 0x37u,
    serviceId_RequestFileTransfer = 0x38u,
    serviceId_WriteMemoryByAddress = 0x3Du,
    serviceId_TesterPresent = 0x3Eu,
    serviceId_PositiveResponseMask = 0x40u,
    serviceId_NegativeResponse = 0x7Fu,
    serviceId_AccessTimingParameter = 0x83u,
    serviceId_SecuredDataTransmission = 0x84u,
    serviceId_ControlDtcSetting = 0x85u,
    serviceId_ResponseOnEvent = 0x86u,
    serviceId_LinkControl = 0x87u,
} Uds_ServiceId_t;

typedef enum
{
    uds_RespCode_PositiveResponse = 0u,
    uds_RespCode_GeneralReject = 0x10u,
    uds_RespCode_ServiceNotSupported = 0x11u,
    uds_RespCode_SubfunctionNotSupported = 0x12u,
    uds_RespCode_IncorrectMessageLengthOrInvalidFormat = 0x13u,
    uds_RespCode_ResponseTooLong = 0x14u,
    uds_RespCode_BusyRepeatRequest = 0x21u,
    uds_RespCode_ConditionsNotCorrect = 0x22u,
    uds_RespCode_RequestSequenceError = 0x24u,
    uds_RespCode_NoResponseFromSubnetComponent = 0x25u,
    uds_RespCode_FailurePreventsExecutionOfRequestedAction = 0x26u,
    uds_RespCode_RequestOutOfRange = 0x31u,
    uds_RespCode_SecurityAccessDenied = 0x33u,
    uds_RespCode_InvalidKey = 0x35u,
    uds_RespCode_ExceededNumberOfAttempts = 0x36u,
    uds_RespCode_RequiredTimeDelayNotExpired = 0x37u,
    uds_RespCode_UploadDownloadNotAccepted = 0x70u,
    uds_RespCode_TransferDataSuspended = 0x71u,
    uds_RespCode_GeneralProgrammingFailure = 0x72u,
    uds_RespCode_WrongBlockSequenceCounter = 0x73u,
    uds_RespCode_RequestCorrectlyReceived_ResponsePending = 0x78u,
    uds_RespCode_SubfunctionNotSupportedInActiveSession = 0x7Eu,
    uds_RespCode_ServiceNotSupportedInActiveSession = 0x7Fu,
    uds_RespCode_RpmTooHigh = 0x81u,
    uds_RespCode_RpmTooLow = 0x82u,
    uds_RespCode_EngineIsRunning = 0x83u,
    uds_RespCode_EngineIsNotRunning = 0x84u,
    uds_RespCode_EngineRunTimeTooLow = 0x85u,
    uds_RespCode_TemperatureTooHigh = 0x86u,
    uds_RespCode_TemperatureTooLow = 0x87u,
    uds_RespCode_VehicleSpeedTooHigh = 0x88u,
    uds_RespCode_VehicleSpeedTooLow = 0x89u,
    uds_RespCode_ThrottlePedalTooHigh = 0x8Au,
    uds_RespCode_ThrottlePedalTooLow = 0x8Bu,
    uds_RespCode_TransmissionNotInNeutral = 0x8Cu,
    uds_RespCode_TransmissionNotInGear = 0x8Du,
    uds_RespCode_BrakeSwitchesNotClosed = 0x8Fu,
    uds_RespCode_ShifterLeverNotInPark = 0x90u,
    uds_RespCode_TorqueConverterClutchLocked = 0x91u,
    uds_RespCode_VoltageTooHigh = 0x92u,
    uds_RespCode_VoltageTooLow = 0x93u,
} Uds_RespCode_t;

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/


/* Exported functions prototypes ---------------------------------------------*/
/**
* @Name-FunctionName
* @details-
* @param[out] -
* @param[in]  -
* @return
* */

#endif /* UDS_TYPES_H */

