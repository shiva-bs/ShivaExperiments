
/*
 * @FileName: CanTp_Socket.c
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
#include <CanTp.h>
#include <CanTp_Socket.h>
#include "DriverCan.h"
#include <stdint.h>

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
                               LOCAL VARIABLES
==============================================================================*/
canTpLink cantp_link;
static uint8_t sendDataBuff[4096];
static uint8_t receiveDataBuff[4096];

/*==============================================================================
                               GLOBAL CONSTANTS
==============================================================================*/

/*==============================================================================
                               GLOBAL VARIABLES
==============================================================================*/
extern volatile uint8_t CAN_driverGetData;
/*==============================================================================
                           LOCAL FUNCTION PROTOTYPES
==============================================================================*/
/**
* @Name-numDataBytesAvailable
* @details-Fetch the number of bytes available in data
* @param[out] -None
* @param[in]  -None
* @return number of bytes of data available.
*/
static int32_t numDataBytesAvailable(void);

/**
* @Name-receiveData
* @details- Data which copy the received data to the given memory
* @param[out]- buf : Buffer to receive the data.
* @param[in]- len : Length of the data.
* @return status of data receive.
*/
static int32_t receiveData(uint8_t *buf, uint32_t len);

/**
* @Name-transmitData
* @details-
* @param[out] -
* @param[in]-buf : Buffer from where the data has to transmit.
* @param[in]- len : Length of the data.
* @return status of data transmission.
*/
static int32_t transmitData(const uint8_t *buf, uint32_t len);

/*==============================================================================
                               LOCAL CONSTANTS
==============================================================================*/
const ISocket_t isotp_socket =
{
    .numAvailableBytes = numDataBytesAvailable,
    .receive = receiveData,
    .transmit = transmitData
};

/*==============================================================================
                                LOCAL FUNCTIONS
==============================================================================*/

static int32_t numDataBytesAvailable(void)
{
    int32_t numBytes = 0;
    if (cantp_link.rcv_status == CAN_TP_RCV_STATUS_FULL)
    {
        numBytes = cantp_link.rcv_size;
    }
    return numBytes;
}

static int32_t receiveData(uint8_t *buf, uint32_t len)
{
    uint16_t outSize = 0;
    CanTp_Receive(&cantp_link, buf, len, &outSize);
    return outSize;
}

static int32_t transmitData(const uint8_t *buf, uint32_t len)
{
	canTp_Send(&cantp_link, buf, len);
    return len;
}

/*==============================================================================
                                GLOBAL FUNCTIONS
==============================================================================*/

/*============Refer CanTp_Socket.h file for function description============*/
void canTp_Socket_Init (void)
{
	canTp_Init_Link(&cantp_link, 0x780,
    		sendDataBuff, sizeof(sendDataBuff),
			receiveDataBuff, sizeof(receiveDataBuff)
    );
}
/*===============================================================================*/

/*============Refer CanTp_Socket.h file for function description============*/
void canTp_Socket_Task (void)
{

	canTp_Poll(&cantp_link);
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
