/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef SERVICELOOKUPTABLE_H_
#define SERVICELOOKUPTABLE_H_

/*
 * @FileName: Uds_ServiceLookupTable.h
 * @ Author:
 * @ Date:
 * @ Project:Bootloader Platform
 * @ Organization: Actevia
 * @ Copyright (c) 2024 Actevia.
 * @ All rights reserved.
 */

/*
  @brief            This Module holds the lookup Table to connect all relevant data of allowance and target FU for every service.

  @note             Functions are safe, as far as given pointers (with respect
                    to their lengths) point to valid memory ranges.

  @details          This Module holds the lookup Table to connect all relevant data of allowance and target FU for every service.
*/

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <Uds_Types.h>

/* Exported types ------------------------------------------------------------*/

typedef Uds_RespCode_t (*udsFunctionSignature)(const uint8_t * pData, uint32_t length);

typedef struct
{
    Uds_ServiceId_t                   sid;
    uint32_t                    sessionMask;
    udsFunctionSignature        serviceRunable;
    uint32_t                    emcryptionMask;
} serviceObject_t;

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported functions prototypes ---------------------------------------------*/
/**
* @Name-udsServiceLookupTable_getServiceObject
* @details- Function fetches the service object
* @param[out] - None
* @param[in]  - sid
* @return Pointer to the header.
* */
serviceObject_t* udsServiceLookupTable_getServiceObject (uint8_t sid);

/* Exported functions prototypes ---------------------------------------------*/
/**
* @Name-udsServiceLookupTable_getNameForServiceSid
* @details- Function fetches service identifier
* @param[out] - None
* @param[in]  - sid
* @return Pointer to the header.
* */
const char * udsServiceLookupTable_getNameForServiceSid (uint8_t sid);

/* Exported functions prototypes ---------------------------------------------*/
/**
* @Name-udsServiceLookupTable_getNameForReturnCode
* @details- Function fetches the return code
* @param[out] - None
* @param[in]  - returnCode
* @return Pointer to the header.
* */
const char * udsServiceLookupTable_getNameForReturnCode (Uds_RespCode_t returnCode);


#endif /* SERVICELOOKUPTABLE_H_ */
