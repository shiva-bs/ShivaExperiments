/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef UDS_STOREDDATATRANSMISSIONFUNCTIONALUNIT_H_
#define UDS_STOREDDATATRANSMISSIONFUNCTIONALUNIT_H_

/*
 * @FileName: Uds_StoredDataTransmissionFunctionalUnit.c
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
                    the functionality of the Services namely, 0x14 0x19
*/

/* Includes ******************************************************************/

/* Exported types ------------------------------------------------------------*/

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
uint32_t storedDataTransmissionFunctionalUnit_SrvClearDiagnosticInformation (uint8_t * receiveBuffer, uint32_t receiveBufferSize, uint8_t * transmit_Buff, uint32_t transmit_BuffSize);

/**
* @Name-
* @details-
* @param[out] -
* @param[in]-
* @return Pointer to the filled block.
*/
uint32_t storedDataTransmissionFunctionalUnit_SrvReadDtcInformation (uint8_t * receiveBuffer, uint32_t receiveBufferSize, uint8_t * transmit_Buff, uint32_t transmit_BuffSize);

#endif /* UDS_STOREDDATATRANSMISSIONFUNCTIONALUNIT_H_ */

