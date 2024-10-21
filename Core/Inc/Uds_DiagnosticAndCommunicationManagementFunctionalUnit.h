/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef UDS_DIAGNOSTICANDCOMMUNICATIONMANAGEMENTFUNCTIONALUNIT_H_
#define UDS_DIAGNOSTICANDCOMMUNICATIONMANAGEMENTFUNCTIONALUNIT_H_

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

  @details          This module provides the implementations of SWCs which handles
                    the functionality of the Services namely, 0x10 0x11 0x27 0x28
                    0x3E 0x85.
*/

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <Uds_SessionAndSerivceControl.h>
#include <Uds_Types.h>

/* Exported types ------------------------------------------------------------*/
typedef struct DefaultSessionTimings_t_private
{
    uint32_t p2;            /**< Response Timeout */
    uint32_t p2star;        /**< Extended Response Timing */
} DefaultSessionTimings_t;

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported functions prototypes ---------------------------------------------*/
/**
* @Name-
* @details-
* @param[out] -
* @param[in]-
* @return Pointer to the filled block.
*/
void DiagnosticAndCommunicationManagementFunctionalUnit_reset (void);

/**
* @Name-
* @details-
* @param[out] -
* @param[in]-
* @return Pointer to the filled block.
*/
Uds_RespCode_t DiagnosticAndCommunicationManagementFunctionalUnit_SrvDiagnosticSessionControl (const uint8_t * receive_Buff, uint32_t receive_Buff_Size);

/**
* @Name-
* @details-
* @param[out] -
* @param[in]-
* @return Pointer to the filled block.
*/
Uds_RespCode_t DiagnosticAndCommunicationManagementFunctionalUnit_SrvEcuReset (const uint8_t * receive_Buff, uint32_t receive_Buff_Size);

/**
* @Name-
* @details-
* @param[out] -
* @param[in]-
* @return Pointer to the filled block.
*/
Uds_RespCode_t DiagnosticAndCommunicationManagementFunctionalUnit_SrvSecurityAccess (const uint8_t * receive_Buff, uint32_t receive_Buff_Size);

/**
* @Name-
* @details-
* @param[out] -
* @param[in]-
* @return Pointer to the filled block.
*/
Uds_RespCode_t DiagnosticAndCommunicationManagementFunctionalUnit_SrvCommunicationControl (const uint8_t * receive_Buff, uint32_t receive_Buff_Size);

/**
* @Name-
* @details-
* @param[out] -
* @param[in]-
* @return Pointer to the filled block.
*/
Uds_RespCode_t DiagnosticAndCommunicationManagementFunctionalUnit_SrvTesterPresent (const uint8_t * receive_Buff, uint32_t receive_Buff_Size);

/**
* @Name-
* @details-
* @param[out] -
* @param[in]-
* @return Pointer to the filled block.
*/
Uds_RespCode_t DiagnosticAndCommunicationManagementFunctionalUnit_SrvAccessTimingParameter (const uint8_t * receive_Buff, uint32_t receive_Buff_Size);

/**
* @Name-
* @details-
* @param[out] -
* @param[in]-
* @return Pointer to the filled block.
*/
Uds_RespCode_t DiagnosticAndCommunicationManagementFunctionalUnit_SrvSecuredDataTransmission (const uint8_t * receive_Buff, uint32_t receive_Buff_Size);

/**
* @Name-
* @details-
* @param[out] -
* @param[in]-
* @return Pointer to the filled block.
*/
Uds_RespCode_t DiagnosticAndCommunicationManagementFunctionalUnit_SrvControlDtcSetting (const uint8_t * receive_Buff, uint32_t receive_Buff_Size);

/**
* @Name-
* @details-
* @param[out] -
* @param[in]-
* @return Pointer to the filled block.
*/
Uds_RespCode_t DiagnosticAndCommunicationManagementFunctionalUnit_SrvResponseOnEvent (const uint8_t * receive_Buff, uint32_t receive_Buff_Size);

/**
* @Name-
* @details-
* @param[out] -
* @param[in]-
* @return Pointer to the filled block.
*/
Uds_RespCode_t DiagnosticAndCommunicationManagementFunctionalUnit_SrvLinkControl (const uint8_t * receive_Buff, uint32_t receive_Buff_Size);

/**
* @Name-
* @details-
* @param[out] -
* @param[in]-
* @return Pointer to the filled block.
*/
Uds_RespCode_t User_Handler_22_Service(const uint8_t * receive_Buff, uint32_t receive_Buff_Size);

#endif /* CHARON_DIAGNOSTICANDCOMMUNICATIONMANAGEMENTFUNCTIONALUNIT_H_ */

