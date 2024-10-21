/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef CAN_TP_CFG
#define CAN_TP_CFG

/*
 * @FileName: CanTp_Cfg.h
 * @ Author:
 * @ Date:
 * @ Project:Bootloader Platform
 * @ Organization: Actevia
 * @ Copyright (c) 2024 Actevia.
 * @ All rights reserved.
 */

/*
  @brief            Macros for Conditional Compilation

  @note

  @details
*/

/* Includes ------------------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Max number of messages the receiver can receive at one time, this value 
 * is affected by can driver queue length
 */
#define CAN_TP_DEFAULT_BLOCK_SIZE   8

/* The STmin parameter value specifies the minimum time gap allowed between 
 * the transmission of consecutive frame network protocol data units
 */
#define CAN_TP_DEFAULT_ST_MIN       0

/* This parameter indicate how many FC N_PDU WTs can be transmitted by the 
 * receiver in a row.
 */
#define CAN_TP_MAX_WFT_NUMBER       1

/* Private: The default timeout to use when waiting for a response during a
 * multi-frame send or receive.
 */
#define CAN_TP_DEFAULT_RESPONSE_TIMEOUT 100

/* Private: Determines if by default, padding is added to ISO-TP message frames.
 */
#define CAN_TP_FRAME_PADDING

/* Exported functions prototypes ---------------------------------------------*/
/**
* @Name-FunctionName
* @details-
* @param[out] -
* @param[in]  -
* @return
* */

#endif /*CAN_TP_CFG*/


