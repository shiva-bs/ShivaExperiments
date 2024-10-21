/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef UDS_APP_INTERFACE_H_
#define UDS_APP_INTERFACE_H_

/*
 * @FileName: Uds_App_Interface.h
 * @ Author:
 * @ Date:
 * @ Project:Bootloader Platform
 * @ Organization: Actevia
 * @ Copyright (c) 2024 Actevia.
 * @ All rights reserved.
 */

/*
  @brief            It provides implementation of Interfaces provided to the Application to Handle the UDS Stack.

  @note             Functions are safe, as far as given pointers (with respect
                    to their lengths) point to valid memory ranges.

  @details          This Module Contains all Interfaces provided to the Application to Handle the UDS Stack.
*/

/* Includes ******************************************************************/

#include <ISocket.h>
#include <stdint.h>

/* Includes ------------------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported functions prototypes ---------------------------------------------*/
/**
* @Name-reset
* @details- Function handles reset
* @param[out] - None
* @param[in]  - None
* @return void
* */
void reset (void);

/**
* @Name-init
* @details- Function handles initialization
* @param[out] - None
* @param[in]  - None
* @return void
* */
void init (ISocket_t systemCommunicationSocket);

/**
* @Name-task
* @details- Function handles tasks
* @param[out] - None
* @param[in]  - None
* @return void
* */
void task (void);

#endif /* UDS_APP_INTERFACE_H_ */
