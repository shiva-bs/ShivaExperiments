/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef UDS_ROUTINEFUNCTIONALUNIT_H_
#define UDS_ROUTINEFUNCTIONALUNIT_H_

/*
 * @FileName: Uds_RoutineFunctionalUnit.c
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
                    the functionality of the Service namely, 0x31
*/

/* Includes ******************************************************************/

#include <stdint.h>
#include <Uds_Types.h>

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported functions prototypes ---------------------------------------------*/
/**
* @Name- Uds_RoutineFunctionalUnit_RoutineControl
* @details- Handles Routine Control Functionality
* @param[out] - NULL
* @param[in]- receiveBuffer, receiveBufferSize
* @return Pointer to the filled block.
*/
Uds_RespCode_t RoutineFunctionalUnit_SrvRoutineControl (const uint8_t * receiveBuffer, uint32_t receiveBufferSize);

#endif /* UDS_ROUTINEFUNCTIONALUNIT_H_ */

