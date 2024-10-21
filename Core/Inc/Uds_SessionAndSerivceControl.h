/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef UDS_SERVICEANDSESSIONCONTROL_H_
#define UDS_SERVICEANDSESSIONCONTROL_H_

/*
 * @FileName: Uds_SessionAndServiceControl.h
 * @ Author:
 * @ Date:
 * @ Project:Bootloader Platform
 * @ Organization: Actevia
 * @ Copyright (c) 2024 Actevia.
 * @ All rights reserved.
 */

/*
  @brief            Implementation of the service and session control module

  @note             Functions are safe, as far as given pointers (with respect
                    to their lengths) point to valid memory ranges.

  @details          This Module handles the implementation of the service and session control module
*/

/* Includes ------------------------------------------------------------------*/

#include <ISocket.h>
#include <stdint.h>

/* Exported types ------------------------------------------------------------*/

typedef enum
{
    sscType_invalid,
    sscType_default,         /**< sscType_default */
    sscType_programming,     /**< sscType_programming */
    sscType_extended,        /**< sscType_extended */
    sscType_secured,         /**< sscType_secured */

    sscType_amount,          /**< The amount of valid session IDs */
    sscType_notStarted,      /**< There is no active session with a client */
    sscType_timedOut         /**< A session has timed out, a library wide reset is necessary */
} sessionTypes_t;

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported functions prototypes ---------------------------------------------*/
/**
* @Name-sscReset
* @details- Function handles reset
* @param[out] - None
* @param[in]  - None
* @return void
* */
void sscReset (void);

/**
* @Name-sscInit
* @details- Function handles initialization
* @param[out] - None
* @param[in]  - None
* @return void
* */
void sscInit (ISocket_t sscComSocket);

/**
* @Name-sscCyclic
* @details- Function handles periodic check of session
* @param[out] - None
* @param[in]  - None
* @return void
* */
void sscCyclic(void);

/**
* @Name-sscRcvMessage
* @details- Function handles ssc rx mssg
* @param[out] - None
* @param[in]  - None
* @return void
* */
void sscRcvMessage (void);

/**
* @Name-sscTxMessage
* @details- Function handles ssc tx mssg
* @param[out] - None
* @param[in]  - None
* @return void
* */
void sscTxMessage (uint8_t const * const pBuffer, uint32_t length);

/**
* @Name-sscSetSession
* @details- Function handles set session
* @param[out] - None
* @param[in]  - None
* @return None
* */
void sscSetSession (sessionTypes_t sessionType, uint32_t timeoutP2, uint32_t timeoutP2extended);

/**
* @Name-sscGetSession
* @details- Function fetches ssc session
* @param[out] - None
* @param[in]  - None
* @return Pointer to the header.
* */
sessionTypes_t sscGetSession (void);

/**
* @Name-sscTesterPresentHeartbeat
* @details- Function handles tester present
* @param[out] - None
* @param[in]  - None
* @return None
* */
void sscTesterPresentHeartbeat(void);

#endif /* UDS_SERVICEANDSESSIONCONTROL_H_ */
