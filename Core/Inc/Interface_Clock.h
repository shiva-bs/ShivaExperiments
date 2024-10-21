/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef INTERFACE_CLOCK_H_
#define INTERFACE_CLOCK_H_

/*
 * @FileName: Interface_clock.h
 * @ Author:
 * @ Date:
 * @ Project:Bootloader Platform
 * @ Organization: Actevia
 * @ Copyright (c) 2024 Actevia.
 * @ All rights reserved.
 */

/*
  @brief            Implementation of Boot Image functionalities

  @note             Functions are safe, as far as given pointers (with respect
                    to their lengths) point to valid memory ranges.

  @details          This module provides some of functions which helps to fetch, validate and start the boot image.
*/

/* Includes ------------------------------------------------------------------*/

#include <stdint.h>

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/


/* Exported functions prototypes ---------------------------------------------*/
/**
* @Name- interface_clock_getTime
* @details- Process driver clock related functionality
* @param[out] - None
* @param[in]  - None
* @return uint32_t
* */
uint32_t interface_clock_getTime(void);

/**
* @Name- interface_clock_getTimeElapsed
* @details- Get time difference from given System time to moment of call
* @param[out] - None
* @param[in]  - sector: image sector index
* @return uint32_t
* */
uint32_t interface_clock_getTimeElapsed(uint32_t timestamp);

#endif /* INTERFACE_CLOCK_H_ */
