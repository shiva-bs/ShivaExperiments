/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef UDS_INPUTOUTPUTCONTROLFUNCTIONALUNIT_H_
#define UDS_INPUTOUTPUTCONTROLFUNCTIONALUNIT_H_

/*
 * @FileName: Uds_DiagnosticAndCommunicationManagementFunctionalUnit.h
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


/* Includes ******************************************************************/

#include <stdbool.h>
#include <stdint.h>
#include <Uds_Types.h>

/* Exported macro ------------------------------------------------------------*/
#define STATIC_IO_CONTROL 0
#define DYNAMIC_IO_CONTROL 1

/* Exported types ------------------------------------------------------------*/
typedef struct IO_Control_Obj_t_public {
    bool (*returnControlToEcu)(void);
    bool (*resetToDefault)(void);
    bool (*freezeCurrentState)(void);
    bool (*shortTermAdjustment)(const uint8_t *buffer, uint32_t length);
} IO_Control_Obj_t;

#if STATIC_IO_CONTROL == 1 && DYNAMIC_IO_CONTROL == 1
    #error "Cannot activate both static and dynamic memory management."
#elif STATIC_IO_CONTROL == 1
    #define STATIC_IO_CONTROL_ARRAY_MAX_LENGTH 10
    typedef struct IO_Mask_Entry_t_public {
        uint16_t position;
        IO_Control_Obj_t ioObject;
    } IO_Mask_Entry_t;
    typedef struct IO_DID_Array_Entry_t_public {
        uint16_t DID;
        IO_Mask_Entry_t *io_mask_array;
        uint16_t ioMaskLength;
        uint32_t data_size;
    } IO_DataIdentifier_List_Entry_t;

    static IO_DataIdentifier_List_Entry_t io_control_list[STATIC_IO_CONTROL_ARRAY_MAX_LENGTH];
    static uint16_t current_io_control_list_length = 0;
#elif DYNAMIC_IO_CONTROL == 1
    typedef struct IO_Mask_Entry_t_public {
        uint16_t position;
        IO_Control_Obj_t ioObject;
        struct IO_Mask_Entry_t_public *next;
    } IO_Mask_Entry_t;
    typedef struct IO_DataIdentifier_List_Entry_t_public {
        uint16_t DID;
        IO_Mask_Entry_t *ioMaskList;
        uint16_t ioMaskLength;
        uint32_t dataSize;
        uint8_t *data;
        struct IO_DataIdentifier_List_Entry_t_public *next;
    } IO_DataIdentifier_List_Entry_t;

    bool InputOutputControlFunctionalUnit_addIOEntry(IO_DataIdentifier_List_Entry_t *entry);

    IO_DataIdentifier_List_Entry_t* InputOutputControlFunctionalUnit_removeIOEntry(uint16_t DID);

    bool InputOutputControlFunctionalUnit_clearIOEntries();
#else
    #error "You must use either static or dynamic memory location for io control."
#endif

/* Exported constants --------------------------------------------------------*/

/* Exported functions prototypes ---------------------------------------------*/
/**
* @Name- Uds_InputOutputControlFunctionalUnit_InputOutputControlByIdentifier
* @details- Handles IO cntrl by identifier
* @param[out] - NULL
* @param[in]- receiveBuffer, receiveBufferSize
* @return Pointer to the filled block.
*/
Uds_RespCode_t InputOutputControlFunctionalUnit_SrvInputOutputControlByIdentifier(const uint8_t * receiveBuffer, uint32_t receiveBufferSize);

#endif /* UDS_INPUTOUTPUTCONTROLFUNCTIONALUNIT_H_ */
