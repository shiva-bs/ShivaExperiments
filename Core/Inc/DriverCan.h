/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef DRIVER_CAN_H
#define DRIVER_CAN_H

/*
 * @FileName: DriverCan.h
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
#include <stdbool.h>
/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/


/* Exported functions prototypes ---------------------------------------------*/

/**
 * @name driverCan_FetchMessage
 * @brief Get CAN message from receive FIFO.
 * This function checks for new messages and returns the
 * data and the DLC, if anything was received
 * @note Only messages with CAN ID as configured in ::driverCan_init will be received.
 * @param data The receive buffer to fill. Must be 8 bytes wide.
 * @return The number of bytes in the received message.
 */
uint8_t driverCan_FetchMessage(uint8_t *msg_data);

/**
 * @name driverCan_TransmitMessage
 * @brief Sends a CAN frame on the bus.
 * A CAN message is put into transmit FIFO and scheduled for transmission.
 * The message is send asynchronously and might not be send yet when this
 * function returns.
 * @param canId The CAN ID to send the message with.
 * @param dlc The amount of bytes in the message.
 * @param data The message itself. This buffer can be invalidated after function returns.
 * @retval true Message scheduled successfully.
 * @retval false FIFO overflow, message not scheduled.
 */
bool driverCan_TransmitMessage(uint16_t canId, uint8_t dlc, const uint8_t *data);

#endif /* DRIVER_FLASH_H */
