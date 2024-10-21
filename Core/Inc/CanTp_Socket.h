/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef CANTP_SOCKET_H
#define CANTP_SOCKET_H

/*
 * @FileName: CanTp_Socket.h
 * @ Author:
 * @ Date:
 * @ Project:Bootloader Platform
 * @ Organization: Actevia
 * @ Copyright (c) 2024 Actevia.
 * @ All rights reserved.
 */

/*
  @brief            Implementation of Can TP functionalities

  @note             Functions are safe, as far as given pointers (with respect
                    to their lengths) point to valid memory ranges.

  @details          This module provides some of functions which create socket, transmit and receive data through CAN.
*/


/* Includes ------------------------------------------------------------------*/
#include <CanTp.h>
#include <ISocket.h>
/* Exported types ------------------------------------------------------------*/
extern canTpLink cantp_link;
/* Exported constants --------------------------------------------------------*/
extern const ISocket_t isotp_socket;
/* Exported macro ------------------------------------------------------------*/

/* Exported functions prototypes ---------------------------------------------*/
/**
* @Name-canTp_Socket_Init
* @details- brief Initializes ISO-TP and CAN peripheral.
* Call this function upon main init procedure.
* ISO-TP and CAN driver are initialized by this
* function.
* @param[out] - None
* @param[in]  - sector: image sector index
* @return Pointer to the header.
*/
void canTp_Socket_Init (void);

/**
* @Name-canTp_Socket_Task
* @details- @brief Cyclic handling of ISO-TP stuff.
* CAN bus is polled for new messages and
* ISO-TP is handled in this function.
* It must be called on a regular basis.
* @param[out] - None
* @param[in]  - sector: image sector index
* @return Pointer to the header.
*/
void canTp_Socket_Task (void);

#endif /* CANTP_SOCKET_H */
