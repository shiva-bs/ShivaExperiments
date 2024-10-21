/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef INC_BOOTMANAGER_H_
#define INC_BOOTMANAGER_H_

/*
 * @FileName: BootImage_Manager.h
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

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/


/* Exported functions prototypes ---------------------------------------------*/
/**
* @Name-bootManagerLoadImage
* @details- Function fetches and returns image header when passing image sector as input
* @param[out] - None
* @param[in]  - None
* @return Pointer to the header.
* */
void bootManagerLoadImage(void);

#endif /* INC_BOOTMANAGER_H_ */
