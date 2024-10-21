/*
 * @FileName: Uds_InputOutputControlFunctionalUnit.c
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
                    the functionality of the Service namely, 0x2F
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
#include <stdint.h>
#include <string.h>
#include <Uds_InputOutputControlFunctionalUnit.h>
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

#define IO_CTRL_RETURN_CONTROL_TO_ECU 0x00
#define IO_CTRL_RESET_TO_DEFAULT 0x01
#define IO_CTRL_FREEZE_CURRENT_STATE 0x02
#define IO_CTRL_SHORT_TERM_ADJUSTMENT 0x03

#if STATIC_IO_CONTROL == 1
IO_DID_Array_Entry_t *findDID(uint16_t DID);
#elif DYNAMIC_IO_CONTROL == 1
IO_DataIdentifier_List_Entry_t *findDID(uint16_t DID);
#else
#error "You must use either static or dynamic memory location for io control."
#endif

/*==============================================================================
                  LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==============================================================================*/


/*==============================================================================
                               LOCAL CONSTANTS
==============================================================================*/


/*==============================================================================
                               LOCAL VARIABLES
==============================================================================*/
static IO_DataIdentifier_List_Entry_t *io_control_list;

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
* @Name-processFlagStatus
* @details-Here, status of IO ctrl is being checked in this comp.
* @param[out] - data, data_length
* @param[in]- did_entry, currentMaskPosition, receivedCommand,
* @return Pointer to the filled block.
*/
static Uds_RespCode_t processFlagStatus(IO_DataIdentifier_List_Entry_t* did_entry, uint16_t currentMaskPosition, uint8_t receivedCommand, const uint8_t *data, uint32_t data_length);

/**
* @Name-processNegativeRespCode
* @details- NRC handling
* @param[out] - NULL
* @param[in]- message, DID, currentMaskPosition and responseCode
* @return Pointer to the filled block.
*/
static Uds_RespCode_t processNegativeRespCode(char *message, uint16_t DID, uint16_t currentMaskPosition, Uds_RespCode_t responseCode);

/**
* @Name-processMaskObject
* @details- Finds the IO object
* @param[out] - NULL
* @param[in]- did_object, position
* @return Pointer to the filled block.
*/
IO_Control_Obj_t *processMaskObject(IO_DataIdentifier_List_Entry_t *did_object, uint16_t position);


/*==============================================================================
                                LOCAL FUNCTIONS
==============================================================================*/

static Uds_RespCode_t processFlagStatus(IO_DataIdentifier_List_Entry_t* did_entry, uint16_t currentMaskPosition, uint8_t receivedCommand, const uint8_t *data, uint32_t data_length)
{
    // IO OBJECT AT MASK FLAG?
    IO_Control_Obj_t *ctrl_object = processMaskObject(did_entry, currentMaskPosition);
    if (ctrl_object == NULL)
    {
        // NOT PRESENT
        return processNegativeRespCode("IO DID 0x%04x does not have an IO Control Object at position %d.", did_entry->DID, currentMaskPosition, uds_RespCode_RequestOutOfRange);
    }
    // TODO: How can we simplify this?
    switch (receivedCommand)
    {
    case IO_CTRL_RETURN_CONTROL_TO_ECU:
        if (ctrl_object->returnControlToEcu == NULL)
        {
            return processNegativeRespCode("Could not find \"Return Control to ECU\" function of DID 0x%04x at mask position %d.", did_entry->DID, currentMaskPosition, uds_RespCode_RequestOutOfRange);
        }
        else if (!ctrl_object->returnControlToEcu())
        {
            return processNegativeRespCode("Couldn't return control to ECU of IO DID 0x%04x at mask position %d.", did_entry->DID, currentMaskPosition, uds_RespCode_ConditionsNotCorrect);
        }
    case IO_CTRL_RESET_TO_DEFAULT:
        if (ctrl_object->resetToDefault == NULL)
        {
            return processNegativeRespCode("Could not find \"Reset to Default\" function of DID 0x%04x at mask position %d.", did_entry->DID, currentMaskPosition, uds_RespCode_RequestOutOfRange);
        }
        else if (!ctrl_object->resetToDefault())
        {
            return processNegativeRespCode("Couldn't reset the state of IO DID 0x%04x at mask position %d.", did_entry->DID, currentMaskPosition, uds_RespCode_ConditionsNotCorrect);
        }
    case IO_CTRL_FREEZE_CURRENT_STATE:
        if (ctrl_object->freezeCurrentState == NULL)
        {
            return processNegativeRespCode("Could not find \"Freeze Current State \" function of DID 0x%04x at mask position %d.", did_entry->DID, currentMaskPosition, uds_RespCode_RequestOutOfRange);
        }
        else if (!ctrl_object->freezeCurrentState())
        {
            return processNegativeRespCode("Couldn't freeze the state of IO DID 0x%04x at mask position %d.", did_entry->DID, currentMaskPosition, uds_RespCode_ConditionsNotCorrect);
        }
    case IO_CTRL_SHORT_TERM_ADJUSTMENT:
        if (ctrl_object->shortTermAdjustment == NULL)
        {
            return processNegativeRespCode("Could not find \"Short Term Adjustment\" function of IO DID 0x%04x at mask position %d.", did_entry->DID, currentMaskPosition, uds_RespCode_RequestOutOfRange);
        }
        else if (!ctrl_object->shortTermAdjustment(data, data_length))
        {
            return processNegativeRespCode("Couldn't adjust the state of IO DID 0x%04x at mask position %d.", did_entry->DID, currentMaskPosition, uds_RespCode_ConditionsNotCorrect);
        }
    default:
        return uds_RespCode_RequestOutOfRange;
    }

}

static Uds_RespCode_t processNegativeRespCode(char *message, uint16_t DID, uint16_t currentMaskPosition, Uds_RespCode_t responseCode)
{
    // LOG_WARNING(message, DID, currentMaskPosition); /< @todo (spike) Wut? (<_< )
	udsSendNegativeResponse(responseCode, serviceId_InputOutputControlByIdentifier);
    return responseCode;
}

/*==============================================================================
                                GLOBAL FUNCTIONS
==============================================================================*/

/*============Refer Uds_InputOutputControlFunctionalUnit.h file for function description============*/

Uds_RespCode_t InputOutputControlFunctionalUnit_SrvInputOutputControlByIdentifier(const uint8_t * receiveBuffer, uint32_t receiveBufferSize)
{
    const uint8_t *saved_states;
    Uds_RespCode_t retVal = uds_RespCode_PositiveResponse;
    if (receiveBufferSize < 4)
    {
        LOG_ERROR("Unexpected message length.");
        udsSendNegativeResponse(uds_RespCode_IncorrectMessageLengthOrInvalidFormat, serviceId_InputOutputControlByIdentifier);
        return uds_RespCode_IncorrectMessageLengthOrInvalidFormat;
    }
    // CHECK IO DID
    // Maybe code a helper module that converts byte-arrays to uint16_t and vice versa
    uint16_t DID = receiveBuffer[1] << 8 | receiveBuffer[2];
    IO_DataIdentifier_List_Entry_t *did_entry = findDID(DID);
    // IS IO DID KNOWN?
    if (NULL == did_entry)
    {
        // NO
        LOG_WARNING("Could not find the IO DID 0x%04x", DID);
        // SEND NEGATIVE RESPONSE (REQUEST OUT OF RANGE)
        udsSendNegativeResponse(uds_RespCode_RequestOutOfRange, serviceId_InputOutputControlByIdentifier);
        // RETURN
        return uds_RespCode_RequestOutOfRange;
    }
    if (4 <= receiveBuffer[3])
    {
        LOG_WARNING("Value of inputOutputControlParameter (0x%x) is invalid.", receiveBuffer[3]);
        udsSendNegativeResponse(uds_RespCode_RequestOutOfRange, serviceId_InputOutputControlByIdentifier);
        return uds_RespCode_RequestOutOfRange;
    }
    uint16_t mask_pointer = 4;
    // SUBFUNCTION USES CONTROL STATES?
    if (IO_CTRL_SHORT_TERM_ADJUSTMENT == receiveBuffer[3])
    {
        // YES
        // SAVE STATES
        saved_states = &receiveBuffer[4];
        // INCREASE MASK POINTER BY STATES SIZE
        mask_pointer += did_entry->dataSize;
    }
    else 
    {
        saved_states = NULL;
    }
    // CHECK PRESENCE OF MASK
    const uint8_t *maskFlags;
    uint16_t maskFlags_Length = 1;
    if (receiveBufferSize > mask_pointer)
    {
        maskFlags = &receiveBuffer[mask_pointer];
        maskFlags_Length = receiveBufferSize - mask_pointer + 1;
    }
    else {
        maskFlags = (uint8_t[]){0x01};
    }
    uint16_t currentMaskPosition = 1;
    uint8_t currentFlag;
    for (uint16_t i = 0; i < maskFlags_Length; i++)
    {
        currentFlag = maskFlags[i];
        for (uint8_t j = 0; j < 8; currentFlag <<= 1, j++)
        {
            if ((currentFlag & 0x80) == 0x80)
            {
                retVal = processFlagStatus(did_entry, currentMaskPosition, receiveBuffer[3], saved_states, did_entry->dataSize);
                if (retVal != uds_RespCode_PositiveResponse)
                {
                    return retVal;
                }
            }
            currentMaskPosition++;
        }
    }
    uint8_t message[4 + did_entry->dataSize];
    message[0] = serviceId_InputOutputControlByIdentifier + serviceId_PositiveResponseMask;
    message[1] = DID >> 8;
    message[2] = DID;
    message[3] = receiveBuffer[3];
    memcpy(&message[4], did_entry->data, did_entry->dataSize);
    sscTxMessage(message, 4 + did_entry->dataSize);
    return retVal;
}

#if STATIC_IO_CONTROL == 1
IO_DID_Array_Entry_t *findDID(uint16_t DID)
{
    for (int i = 0; i < current_io_control_list_length; i++)
    {
        if (DID == io_control_list[i].DID)
        {
            return &io_control_list[i];
        }
    }
    return NULL;
}
IO_Control_Obj_t *processMaskObject(IO_DID_Array_Entry_t *did_object, uint16_t position)
{
    for (int i = 0; i < did_object->)
}
#elif DYNAMIC_IO_CONTROL == 1
bool InputOutputControlFunctionalUnit_addIOEntry(IO_DataIdentifier_List_Entry_t *entry) {
    if(io_control_list == NULL) {
        io_control_list = entry;
        return true;
    }
    IO_DataIdentifier_List_Entry_t *nextEntry = io_control_list;
    // To limit the user from adding too many entries a counter could be introduced here.
    // When the counter becomes greater than a threshold the function could return false then.
    while(nextEntry->next != NULL) {
        nextEntry = nextEntry->next;
    }
    nextEntry->next = entry;
    return true;
}

IO_DataIdentifier_List_Entry_t* InputOutputControlFunctionalUnit_removeIOEntry(uint16_t DID) {
    IO_DataIdentifier_List_Entry_t *retVal = NULL;
    if(io_control_list->DID == DID) {
        retVal = io_control_list;
        io_control_list = (io_control_list->next == NULL) ? NULL : io_control_list->next;
        return retVal;
    }
    IO_DataIdentifier_List_Entry_t *nextEntry = io_control_list;
    while(nextEntry->next != NULL) {
        if(nextEntry->next->DID == DID) {
            retVal = nextEntry->next;
            nextEntry->next = (nextEntry->next->next == NULL) ? NULL : nextEntry->next->next;
            return retVal;
        }
        nextEntry = nextEntry->next;
    }
    return NULL;
}

IO_DataIdentifier_List_Entry_t *findDID(uint16_t DID)
{
	IO_DataIdentifier_List_Entry_t *currentHead = io_control_list;
	while (currentHead != NULL)
	{
	   if (currentHead->DID == DID)
	       return currentHead;
	   currentHead = currentHead->next;
	}
	return NULL;
}

IO_Control_Obj_t *processMaskObject(IO_DataIdentifier_List_Entry_t *did_object, uint16_t position)
{
    IO_Mask_Entry_t *currentHead = did_object->ioMaskList;
    while (currentHead != NULL)
    {
        if (currentHead->position == position)
        {
            return &currentHead->ioObject;
        }
    }
    return NULL;
}

bool InputOutputControlFunctionalUnit_clearIOEntries() {
    io_control_list = NULL;
    return true;
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
