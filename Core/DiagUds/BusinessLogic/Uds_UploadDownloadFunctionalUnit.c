/*
 * @FileName: Uds_UploadDownloadFunctionalUnit.c
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
                    the functionality of the Services namely, 0x34 0x35, 0x36, 0x37
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
#include <Interface_NvmDriver.h>
#include <Uds_NegativeResponse.h>
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
#define UDS_MAX_INPUT_FRAME_SIZE    4095u
#define UDS_MAX_OUTPUT_FRAME_SIZE   4095u

/*==============================================================================
                  LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==============================================================================*/
typedef enum Transfer_Direction_t_private
{
    transfer_idle,           /**< No Transfer Ongoing */
    transfer_download,       /**< Transfer Client -> Server */
    transfer_upload          /**< Transfer Server -> Client */
} Transfer_Direction_t;

/*==============================================================================
                               LOCAL CONSTANTS
==============================================================================*/


/*==============================================================================
                               LOCAL VARIABLES
==============================================================================*/
/* Current Transfer State */
static Transfer_Direction_t s_transferDirection = transfer_idle;
/* Stores the Memory Address used to send Data in Chunks */
static uint32_t currentMemAddress = 0uL;
/* Stores Transfer Amount remaining */
static uint32_t remainingMemLength = 0uL;
/* Stores Counter to Check Transfer amount and count */
static uint8_t nextSequenceCounter = 0uL;

/*==============================================================================
                               GLOBAL CONSTANTS
==============================================================================*/


/*==============================================================================
                               GLOBAL VARIABLES
==============================================================================*/
extern volatile driverFlash_Division_t Erased_Division;

/*==============================================================================
                           LOCAL FUNCTION PROTOTYPES
==============================================================================*/
/**
* @Name- requestTransfer
* @details- Handles the transfer of data to flash
* @param[out] - NULL
* @param[in]- direction, receiveBuffer, receiveBufferSize
* @return Pointer to the filled block.
*/
static Uds_RespCode_t requestTransfer(Transfer_Direction_t direction, const uint8_t * receiveBuffer, uint32_t receiveBufferSize);

/*==============================================================================
                                LOCAL FUNCTIONS
==============================================================================*/

static Uds_RespCode_t requestTransfer(Transfer_Direction_t direction, const uint8_t * receiveBuffer, uint32_t receiveBufferSize)
{
    const PACKED_STRUCT(anonym) {
        uint8_t sid;
        uint8_t dataFormatIdentifier;
        uint8_t addressAndLengthFormatIdentifier;
        uint8_t AddressInformation[8];
    } * receivedMessage = (const void*)receiveBuffer;

    /*driverFlash_Division_t SelectInactiveDivision =driverFlash_Division_NULL;*/
    Uds_RespCode_t result = uds_RespCode_PositiveResponse;

    uint8_t lengthOfMemoryLength = receivedMessage->addressAndLengthFormatIdentifier >> 4;
    uint8_t lengthOfMemoryAddress = receivedMessage->addressAndLengthFormatIdentifier & 0xFu;
    if (
            (lengthOfMemoryAddress == 0u) ||
            (lengthOfMemoryLength == 0u) ||
            (lengthOfMemoryAddress > 4u) ||
            (lengthOfMemoryLength > 4u) ||
            (receivedMessage->dataFormatIdentifier != 0x00u)
        )
    {
        LOG_ERROR("Format Identifier invalid: size of address = %i, size of length = %i.", lengthOfMemoryAddress, lengthOfMemoryLength);
        result = uds_RespCode_RequestOutOfRange;
    }
      /*Diwakar: changed +3 to +2 for testing*/
    else if ( (((uint32_t)lengthOfMemoryAddress + lengthOfMemoryLength) + 3u) != receiveBufferSize )
    {
        LOG_ERROR("Unexpected message length.");
        result = uds_RespCode_IncorrectMessageLengthOrInvalidFormat;
    }

    else
    {
        uint32_t memoryAddress = 0;
        uint32_t memoryLength = 0;
        for (uint8_t i = 0; i<lengthOfMemoryAddress; i++)
        {
            memoryAddress |= (uint32_t)(receivedMessage->AddressInformation[lengthOfMemoryAddress - (i+1u)]) << (i*8u);
        }

        for (uint8_t i = 0; i<lengthOfMemoryLength; i++)
        {
            memoryLength |= (uint32_t)receivedMessage->AddressInformation[(lengthOfMemoryAddress + lengthOfMemoryLength) - (i+1u)] << (i*8u);
        }
        LOG_INFO("Transfer Requested, address 0x%x, length 0x%x, direction %s.", memoryAddress, memoryLength, direction == transfer_download ? "download" : "upload");

        if ( false == NvmDriver_checkAddressRange(memoryAddress, memoryLength) )
        {
            LOG_ERROR("Requested memory is out of range.");
            result = uds_RespCode_RequestOutOfRange;
        }
        else if (s_transferDirection != transfer_idle)
        {
            LOG_ERROR("A transfer is already ongoing.");
            result = uds_RespCode_ConditionsNotCorrect;
        }
        else
        {
        	/*Get converted address*/
        	//uint32_t convertedAddress= (uint32_t)drvFlash_getStartAddress(Erased_Partion)+(memoryAddress-0x08020000);
        	/*Diwakar: select Erased Flash address for OTA updates*/
        //	SelectInactiveDivision= Erased_Partion;
        //	if(drvFlash_Division_NULL != SelectInactiveDivision)
        //	{
            currentMemAddress =memoryAddress;

        //	}
        //	else
        //	{
        //		currentMemAddress = (uint32_t)drvFlash_getStartAddress(Assign_Division());
        //	}
            remainingMemLength = memoryLength;
            s_transferDirection = direction;
            nextSequenceCounter = 1;
            uint8_t transmit_Buff[4] = {
                    receivedMessage->sid | (uint8_t)serviceId_PositiveResponseMask,
                    0x20,
                    (uint8_t)((UDS_MAX_INPUT_FRAME_SIZE >> 8u) & 0xFFu),
                    (uint8_t)(UDS_MAX_INPUT_FRAME_SIZE & 0xFFu)
            };
            sscTxMessage(transmit_Buff, sizeof(transmit_Buff));
        }

    }
    if (result != uds_RespCode_PositiveResponse)
    {
        Uds_ServiceId_t receivedSid;
        if (direction == transfer_download)
        {
            receivedSid = serviceId_RequestDownload;
        }
        else
        {
            receivedSid = serviceId_RequestUpload;
        }
        udsSendNegativeResponse(result, receivedSid);
    }
    return result;
}

/*==============================================================================
                                GLOBAL FUNCTIONS
==============================================================================*/

/*============Refer Uds_UploadDownloadFunctionalUnit.h file for function description============*/

void UploadDownloadFunctionalUnit_reset (void)
{
    s_transferDirection = transfer_idle;
    currentMemAddress = 0;
    remainingMemLength = 0;
    nextSequenceCounter = 0;
}

/*============Refer Uds_UploadDownloadFunctionalUnit.h file for function description============*/

Uds_RespCode_t UploadDownloadFunctionalUnit_SrvRequestDownload (const uint8_t * receiveBuffer, uint32_t receiveBufferSize)
{
    return requestTransfer(transfer_download, receiveBuffer, receiveBufferSize);
}

/*============Refer Uds_UploadDownloadFunctionalUnit.h file for function description============*/

Uds_RespCode_t UploadDownloadFunctionalUnit_SrvRequestUpload (const uint8_t * receiveBuffer, uint32_t receiveBufferSize)
{
    return requestTransfer(transfer_upload, receiveBuffer, receiveBufferSize);
}

/*============Refer Uds_UploadDownloadFunctionalUnit.h file for function description============*/

Uds_RespCode_t UploadDownloadFunctionalUnit_SrvTransferData (const uint8_t * receiveBuffer, uint32_t receiveBufferSize)
{
    const PACKED_STRUCT(anonym) {
        uint8_t sid;
        uint8_t blockSequenceCounter;
        uint8_t data[UDS_MAX_INPUT_FRAME_SIZE];
    } * receivedMessage = (const void*)receiveBuffer;

    Uds_RespCode_t result = uds_RespCode_PositiveResponse;

    if (receiveBufferSize > UDS_MAX_INPUT_FRAME_SIZE)
    {
        LOG_ERROR("Frame is too long! Maximum size is %i.", UDS_MAX_INPUT_FRAME_SIZE);
        result = uds_RespCode_IncorrectMessageLengthOrInvalidFormat;
    }
    else if (s_transferDirection == transfer_idle)
    {
        LOG_ERROR("Transfer Data was not expected. Forgot to request upload/download?");
        result = uds_RespCode_RequestSequenceError;
    }
    else if (remainingMemLength < (receiveBufferSize - 2u) )
    {
        LOG_ERROR("Too much data received!");
        result = uds_RespCode_TransferDataSuspended;
    }
    else if (receivedMessage->blockSequenceCounter != nextSequenceCounter)
    {
        LOG_ERROR("Sequence number not expected! Received: %i, Expected: %i.", receivedMessage->blockSequenceCounter, nextSequenceCounter);
        result = uds_RespCode_WrongBlockSequenceCounter;
    }
    else
    {
        if (s_transferDirection == transfer_download)
        {
            result = NvmDriver_write(currentMemAddress, receivedMessage->data, receiveBufferSize - 2u);
            if (result == uds_RespCode_PositiveResponse)
            {
                currentMemAddress += receiveBufferSize - 2u;
                remainingMemLength -= receiveBufferSize - 2u;
                nextSequenceCounter++;

                uint8_t transmit_Buff[2] = {
                        receivedMessage->sid | (uint8_t)serviceId_PositiveResponseMask,
                        receivedMessage->blockSequenceCounter
                };
                sscTxMessage(transmit_Buff, sizeof(transmit_Buff));
            }
            else
            {
                LOG_ERROR("NVM driver reported error while writing to flash.");
                /* negative response is sent at the end of the function, nothing to do here.*/
            }
        }
        else
        {
            uint8_t transmit_Buff[UDS_MAX_OUTPUT_FRAME_SIZE];
            uint32_t transmit_BuffSize = sizeof(transmit_Buff);
            if (remainingMemLength < (transmit_BuffSize - 2u) )
            {
                transmit_BuffSize = remainingMemLength + 2u;
            }
            NvmDriver_read(currentMemAddress, &(transmit_Buff[2]), transmit_BuffSize - 2u);
            currentMemAddress += transmit_BuffSize - 2u;
            remainingMemLength -= transmit_BuffSize - 2u;
            nextSequenceCounter++;

            transmit_Buff[0] = (uint8_t)serviceId_TransferData | (uint8_t)serviceId_PositiveResponseMask;
            transmit_Buff[1] = receivedMessage->blockSequenceCounter;
            sscTxMessage(transmit_Buff, transmit_BuffSize);
        }
    }
    if (result != uds_RespCode_PositiveResponse)
    {
    	udsSendNegativeResponse(result, serviceId_TransferData);
    }
    return result;
}

/*============Refer Uds_UploadDownloadFunctionalUnit.h file for function description============*/

Uds_RespCode_t UploadDownloadFunctionalUnit_SrvRequestTransferExit (const uint8_t * receiveBuffer, uint32_t receiveBufferSize)
{
    Uds_RespCode_t result = uds_RespCode_PositiveResponse;
    (void) receiveBuffer;

    if (receiveBufferSize > 1u)
    {
        LOG_ERROR("Unexpected message length.");
        result = uds_RespCode_IncorrectMessageLengthOrInvalidFormat;
    }
    else if (remainingMemLength != 0u)
    {
        LOG_ERROR("Transfer Exit received, but not all data was transferred.");
        result = uds_RespCode_RequestSequenceError;
    }
    else if (s_transferDirection == transfer_idle)
    {
        LOG_ERROR("No transfer ongoing, cannot exit transfer.");
        result = uds_RespCode_RequestSequenceError;
    }
    else
    {
        currentMemAddress = 0;
        remainingMemLength = 0;
        nextSequenceCounter = 0;
        s_transferDirection = transfer_idle;

        LOG_INFO("Exiting transfer mode.");
        uint8_t transmit_Buff[1] = {(uint8_t)serviceId_RequestTransferExit | (uint8_t)serviceId_PositiveResponseMask};
        sscTxMessage(transmit_Buff, sizeof(transmit_Buff));
    }
    if (result != uds_RespCode_PositiveResponse)
    {
    	udsSendNegativeResponse(result, serviceId_RequestTransferExit);
    }
    return result;
}

/*============Refer Uds_UploadDownloadFunctionalUnit.h file for function description============*/

Uds_RespCode_t UploadDownloadFunctionalUnit_SrvRequestFileTransfer (const uint8_t * receiveBuffer, uint32_t receiveBufferSize)
{
    (void)receiveBuffer;
    (void)receiveBufferSize;
    udsSendNegativeResponse(uds_RespCode_ServiceNotSupported, serviceId_RequestFileTransfer);
    return uds_RespCode_ServiceNotSupported;
}

#ifdef TEST
void UploadDownloadFunctionalUnit_setCurrentMemoryAddress (uint32_t newAddress)
{
    currentMemAddress = newAddress;
}
void UploadDownloadFunctionalUnit_setRemainingMemoryLength (uint32_t newLength)
{
    remainingMemLength = newLength;
}
void UploadDownloadFunctionalUnit_setTransferDirection (uint8_t newDirection)
{
    s_transferDirection = newDirection;
}
void UploadDownloadFunctionalUnit_setNextSequenceCounter (uint8_t newCounter)
{
    nextSequenceCounter = newCounter;
}
uint32_t UploadDownloadFunctionalUnit_getCurrentMemoryAddress (void)
{
    return currentMemAddress;
}
uint32_t UploadDownloadFunctionalUnit_getRemainingMemoryLength (void)
{
    return remainingMemLength;
}
uint8_t UploadDownloadFunctionalUnit_getTransferDirection (void)
{
    return s_transferDirection;
}
uint8_t UploadDownloadFunctionalUnit_getNextSequenceCounter (void)
{
    return nextSequenceCounter;
}

#endif


/* -------------------------------------------- END OF FILE -------------------------------------------- */
/* =================================================================================
                                              FILE LOG HISTORY
=====================================================================================
*  Date    Changes      Author     Change_Set
*
*
*************************************************************************************/
