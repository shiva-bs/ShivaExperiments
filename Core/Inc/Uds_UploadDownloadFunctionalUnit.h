/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef UDS_UPLOADDOWNLOADFUNCTIONALUNIT_H_
#define UDS_UPLOADDOWNLOADFUNCTIONALUNIT_H_

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

/* Includes ******************************************************************/

#include <stdint.h>
#include <Uds_Types.h>

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported functions prototypes ---------------------------------------------*/

/**
 * Resets all internal Variables and
 * stops all ongoing Services.
 */
void UploadDownloadFunctionalUnit_reset (void);

/**
* @Name-
* @details-
* @param[out] -
* @param[in]-
* @return Pointer to the filled block.
* */
Uds_RespCode_t UploadDownloadFunctionalUnit_SrvRequestDownload (const uint8_t * receiveBuffer, uint32_t receiveBufferSize);

/**
* @Name-
* @details-
* @param[out] -
* @param[in]-
* @return Pointer to the filled block.
* */
Uds_RespCode_t UploadDownloadFunctionalUnit_SrvRequestUpload (const uint8_t * receiveBuffer, uint32_t receiveBufferSize);

/**
* @Name-
* @details-
* @param[out] -
* @param[in]-
* @return Pointer to the filled block.
* */
Uds_RespCode_t UploadDownloadFunctionalUnit_SrvTransferData (const uint8_t * receiveBuffer, uint32_t receiveBufferSize);

/**
* @Name-
* @details-
* @param[out] -
* @param[in]-
* @return Pointer to the filled block.
* */
Uds_RespCode_t UploadDownloadFunctionalUnit_SrvRequestTransferExit (const uint8_t * receiveBuffer, uint32_t receiveBufferSize);

/**
* @Name-
* @details-
* @param[out] -
* @param[in]-
* @return Pointer to the filled block.
* */
Uds_RespCode_t UploadDownloadFunctionalUnit_SrvRequestFileTransfer (const uint8_t * receiveBuffer, uint32_t receiveBufferSize);

#ifdef TEST

void UploadDownloadFunctionalUnit_setCurrentMemoryAddress (uint32_t newAddress);

void UploadDownloadFunctionalUnit_setRemainingMemoryLength (uint32_t newLength);

void UploadDownloadFunctionalUnit_setTransferDirection (uint8_t newDirection);

void UploadDownloadFunctionalUnit_setNextSequenceCounter (uint8_t newCounter);

uint32_t UploadDownloadFunctionalUnit_getCurrentMemoryAddress (void);

uint32_t UploadDownloadFunctionalUnit_getRemainingMemoryLength (void);

uint8_t UploadDownloadFunctionalUnit_getTransferDirection (void);

uint8_t UploadDownloadFunctionalUnit_getNextSequenceCounter (void);

#endif

#endif /* UDS_UPLOADDOWNLOADFUNCTIONALUNIT_H_ */

