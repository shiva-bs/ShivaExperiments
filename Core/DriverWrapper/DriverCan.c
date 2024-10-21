/*
 * @FileName: DriverCan.c
 * @ Author:
 * @ Date:
 * @ Project:Bootloader Platform
 * @ Organization: Actevia
 * @ Copyright (c) 2024 Actevia.
 * @ All rights reserved.
 */

/*
  @brief            Implementation of Can Driver

  @note             Functions are safe, as far as given pointers (with respect
                    to their lengths) point to valid memory ranges.

  @details          This module provides some of functions get and send Can messages.
*/

/*==============================================================================
                               MISRA VIOLATIONS
==============================================================================*/
/*


*/

/*==============================================================================
                                 INCLUDE FILES
 1) system and project includes
 2) needed interfaces from external units
 3) internal and external interfaces from this unit
==============================================================================*/
#include <DriverCan.h>
#include "can.h"

/*==============================================================================
                            FILE VERSION CHECKS
 ==============================================================================*/

/*==============================================================================
                                  IMPORTS
==============================================================================*/

/*==============================================================================
                                LOCAL MACROS
==============================================================================*/


/*==============================================================================
                  LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==============================================================================*/

/*==============================================================================
                               LOCAL CONSTANTS
==============================================================================*/

/*==============================================================================
                               LOCAL VARIABLES
==============================================================================*/

/*==============================================================================
                               GLOBAL CONSTANTS
==============================================================================*/

/*==============================================================================
                               GLOBAL VARIABLES
==============================================================================*/

/*==============================================================================
                           LOCAL FUNCTION PROTOTYPES
==============================================================================*/
/**
* @Name-
* @details-
* @param[out] -
* @param[in]-
* @return.
*/

/*==============================================================================
                                LOCAL FUNCTIONS
==============================================================================*/


/*==============================================================================
                                GLOBAL FUNCTIONS
==============================================================================*/


/*============Refer DriverCan.h file for function description============*/

uint8_t driverCan_FetchMessage(uint8_t *msg_data)
{
	CAN_RxHeaderTypeDef   rx_Header;
	uint8_t               rx_Data[8];

	if (HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &rx_Header, rx_Data) != HAL_OK)
	  {
	    Error_Handler();
	  }


        msg_data[0] = rx_Data[0];
        msg_data[1] = rx_Data[1] ;
        msg_data[2] = rx_Data[2] ;
        msg_data[3] = rx_Data[3] ;
        msg_data[4] = rx_Data[4] ;
        msg_data[5] = rx_Data[5] ;
        msg_data[6] = rx_Data[6] ;
        msg_data[7] = rx_Data[7] ;

    
    return rx_Header.DLC;
}

/*============Refer DriverCan.h file for function description============*/

/*===============================================================================*/

bool driverCan_TransmitMessage(uint16_t canId, uint8_t dlc, const uint8_t *data)
{
	CAN_TxHeaderTypeDef   tx_Header;
	uint8_t               tx_Data[8];
	uint32_t              tx_Mailbox;

	tx_Header.IDE = CAN_ID_STD;
	tx_Header.StdId = canId;
	tx_Header.RTR = CAN_RTR_DATA;
	tx_Header.DLC = dlc;

	tx_Data[0] = (uint8_t)(data[0]);
	tx_Data[1] = (uint8_t)(data[1]);
	tx_Data[2] = (uint8_t)(data[2]);
	tx_Data[3] = (uint8_t)(data[3]);
	tx_Data[4] = (uint8_t)(data[4]);
	tx_Data[5] = (uint8_t)(data[5]);
	tx_Data[6] = (uint8_t)(data[6]);
	tx_Data[7] = (uint8_t)(data[7]);

    bool tranmit_success = false;
    for (uint8_t i = 0; i<3; i++)
    {
    	if (HAL_CAN_AddTxMessage(&hcan1, &tx_Header, tx_Data, &tx_Mailbox) != HAL_OK)
    	  {
    	   Error_Handler ();
    	  }
    	else
    	  {
    		tranmit_success=true;
    		break;
    	  }


    }
    return tranmit_success;
}

/*===============================================================================*/

/* -------------------------------------------- END OF FILE -------------------------------------------- */
/* =================================================================================
                                              FILE LOG HISTORY
=====================================================================================
*  Date    Changes      Author     Change_Set
*
*
*************************************************************************************/

