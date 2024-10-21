/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef UDS_NEGATIVERESPONSE_H
#define UDS_NEGATIVERESPONSE_H

/*
 * @FileName: Uds_NegativeResponse.h
 * @ Author:
 * @ Date:
 * @ Project:Bootloader Platform
 * @ Organization: Actevia
 * @ Copyright (c) 2024 Actevia.
 * @ All rights reserved.
 */

/*
  @brief            Implementation of Negative Response function

  @note             Functions are safe, as far as given pointers (with respect
                    to their lengths) point to valid memory ranges.

  @details
*/


/* Includes ------------------------------------------------------------------*/

#include <stdint.h>
#include <Uds_Types.h>

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/


/* Exported functions prototypes ---------------------------------------------*/
/**
* @Name-udsSendNegativeResponse
* @details- Function sends negative response
* @param[out] - None
* @param[in]  - ResponseCode : Response code 
* @param[in]  - RequestedSid : Service ID
* @return null
* */
void udsSendNegativeResponse (Uds_RespCode_t ResponseCode, Uds_ServiceId_t RequestedSid);

#endif /* UDS_NEGATIVERESPONSE_H */
