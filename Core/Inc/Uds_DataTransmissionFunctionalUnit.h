/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef UDS_DATATRANSMISSIONFUNCTIONALUNIT_H_
#define UDS_DATATRANSMISSIONFUNCTIONALUNIT_H_

/*
 * @FileName: Uds_DataTransmissionFunctionalUnit.h
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
                    the functionality of the Services namely, 0x22 0x2E
*/

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <Uds_Types.h>

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
Uds_RespCode_t DataTransmissionFunctionalUnit_SrvReadDataByIdentifier (uint8_t * receiveBuffer, uint32_t receiveBufferSize);

/**
* @Name-
* @details-
* @param[out] -
* @param[in]-
* @return Pointer to the filled block.
*/
Uds_RespCode_t DataTransmissionFunctionalUnit_SrvReadMemoryByAddress (uint8_t * receiveBuffer, uint32_t receiveBufferSize);

/**
* @Name-
* @details-
* @param[out] -
* @param[in]-
* @return Pointer to the filled block.
*/
Uds_RespCode_t DataTransmissionFunctionalUnit_SrvReadScalingDataByIdentifier (uint8_t * receiveBuffer, uint32_t receiveBufferSize);

/**
* @Name-
* @details-
* @param[out] -
* @param[in]-
* @return Pointer to the filled block.
*/
Uds_RespCode_t DataTransmissionFunctionalUnit_SrvReadDataByPeriodicIdentifier (uint8_t * receiveBuffer, uint32_t receiveBufferSize);

/**
* @Name-
* @details-
* @param[out] -
* @param[in]-
* @return Pointer to the filled block.
*/
Uds_RespCode_t DataTransmissionFunctionalUnit_SrvDynamicallyDefineDataIdentifier (uint8_t * receiveBuffer, uint32_t receiveBufferSize);

/**
* @Name-
* @details-
* @param[out] -
* @param[in]-
* @return Pointer to the filled block.
*/
Uds_RespCode_t DataTransmissionFunctionalUnit_SrvWriteDataByIdentifier (uint8_t * receiveBuffer, uint32_t receiveBufferSize);

/**
* @Name-
* @details-
* @param[out] -
* @param[in]-
* @return Pointer to the filled block.
*/
Uds_RespCode_t DataTransmissionFunctionalUnit_SrvWriteMemoryByAddress (uint8_t * receiveBuffer, uint32_t receiveBufferSize);

#endif /* UDS_DATATRANSMISSIONFUNCTIONALUNIT_H_ */
