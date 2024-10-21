/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef CANTP_USER_H
#define CANTP_USER_H

/*
 * @FileName: CanTp_User.h
 * @ Author:
 * @ Date:
 * @ Project:Bootloader Platform
 * @ Organization: Actevia
 * @ Copyright (c) 2024 Actevia.
 * @ All rights reserved.
 */

/*
  @brief            Implementation of Can TP Debug,send data and Get MS

  @note             Functions are safe, as far as given pointers (with respect
                    to their lengths) point to valid memory ranges.

  @details          This module provides some of functions which helps to debug, send data and get time in ms.
*/


/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/


/* Exported functions prototypes ---------------------------------------------*/
/**
* @Name-canTp_User_Debug
* @details-Functon prints the debug messages.
* @param[out] - None
* @param[in]  - message: Message as a string
* @return null.
* */
void canTp_User_Debug(const char* message, ...);

/**
* @Name-canTp_User_Send_Can
* @details- Function sends data via can
* @param[out] - None
* @param[in]  - arbitration_id: CAN Arbitration ID
* @param[in]  - data: send data buffer
* @param[in]  - size: send data length
* @return status of send data.
* */
int  canTp_User_Send_Can(const uint32_t arbitration_id,
                         const uint8_t* data, const uint8_t size);

/**
* @Name-canTp_User_GetMs
* @details- Function fetches time in ms.
* @param[out] - None
* @param[in]  - None
* @return time in ms.
* */
uint32_t canTp_User_GetMs(void);

#endif /* CANTP_USER_H */

