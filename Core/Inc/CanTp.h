/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef CANTP_H
#define CANTP_H

/*
 * @FileName: CanTp.h
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

  @details          This module provides some functions for CAN TP communication.
*/

#ifdef __cplusplus
#include <stdint.h>

extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <CanTp_Defines.h>
#include <CanTp_Cfg.h>
#include "CanTp_User.h"
/* Exported types ------------------------------------------------------------*/

/**
 * @brief Struct containing the data for linking an application to a CAN instance.
 * The data stored in this struct is used internally and may be used by software programs
 * using this library.
 */
typedef struct canTpLink {
    /* sender paramters */
    uint32_t                    transmit_arbitration_id; /* used to reply consecutive frame */
    /* message buffer */
    uint8_t*                    transmit_buffer;
    uint16_t                    transmit_buf_size;
    uint16_t                    transmit_size;
    uint16_t                    transmit_offset;
    /* multi-frame flags */
    uint8_t                     transmit_seq_num;
    uint16_t                    transmit_bs_remain; /* Remaining block size */
    uint8_t                     transmit_st_min;    /* Separation Time between consecutive frames, unit millis */
    uint8_t                     transmit_wtf_count; /* Maximum number of FC.Wait frame transmissions  */
    uint32_t                    transmit_timer_st;  /* Last time send consecutive frame */    
    uint32_t                    transmit_timer_bs;  /* Time until reception of the next FlowControl N_PDU
                                                   start at sending FF, CF, receive FC
                                                   end at receive FC */
    int                         transmit_protocol_result;
    uint8_t                     transmit_status;

    /* receiver paramters */
    uint32_t                    rcv_arbitration_id;
    /* message buffer */
    uint8_t*                    rcv_buffer;
    uint16_t                    rcv_buf_size;
    uint16_t                    rcv_size;
    uint16_t                    rcv_offset;
    /* multi-frame control */
    uint8_t                     rcv_seq_num;
    uint8_t                     rcv_bs_count; /* Maximum number of FC.Wait frame transmissions  */
    uint32_t                    rcv_timer_cr; /* Time until transmission of the next ConsecutiveFrame N_PDU
                                                     start at sending FC, receive CF 
                                                     end at receive FC */
    int                         rcv_protocol_result;
    uint8_t                     rcv_status;                                                     
} canTpLink;

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/


/* Exported functions prototypes ---------------------------------------------*/
/**
 * @Name-canTp_Init_Link
 * @brief Initialises the ISO-TP library.
 * @param cantp_link The @code canTpLink @endcode instance used for transceiving data.
 * @param transmit_id The ID used to send data to other CAN nodes.
 * @param transmit_data_buf A pointer to an area in memory which can be used as a buffer for data to be sent.
 * @param transmit_data_len The size of the buffer area.
 * @param rcv_data_buf A pointer to an area in memory which can be used as a buffer for data to be received.
 * @param rcv_data_len The size of the buffer area.
 */
void canTp_Init_Link(canTpLink *cantp_link, uint32_t transmit_id,
                     uint8_t *transmit_data_buf, uint16_t transmit_data_len,
                     uint8_t *rcv_data_buf, uint16_t rcv_data_len);

/**
 * @brief Polling function; call this function periodically to handle timeouts, send consecutive frames, etc.
 *
 * @param cantp_link The @code canTpLink @endcode instance used.
 */
void canTp_Poll(canTpLink *cantp_link);

/**
 * @brief Handles incoming CAN messages.
 * Determines whether an incoming message is a valid ISO-TP frame or not and handles it accordingly.
 *
 * @param cantp_link The @code canTpLink @endcode instance used for transceiving data.
 * @param data_buff The data received via CAN.
 * @param data_len The length of the data received.
 */
void canTp_On_CanMessage(canTpLink *cantp_link, uint8_t *data_buff, uint8_t data_len);

/**
 * @brief Sends ISO-TP frames via CAN, using the ID set in the initialising function.
 *
 * Single-frame messages will be sent immediately when calling this function.
 * Multi-frame messages will be sent consecutively when calling isotp_poll.
 *
 * @param cantp_link The @code canTpLink @endcode instance used for transceiving data.
 * @param payload The payload to be sent. (Up to 4095 bytes).
 * @param size The size of the payload to be sent.
 *
 * @return Possible return values:
 *  - @code ISOTP_RET_OVERFLOW @endcode
 *  - @code ISOTP_RET_INPROGRESS @endcode
 *  - @code ISOTP_RET_OK @endcode
 *  - The return value of the user shim function isotp_user_send_can().
 */
int canTp_Send(canTpLink *cantp_link, const uint8_t payload[], uint16_t size);

/**
 * @brief See @cantp_link isotp_send @endlink, with the exception that this function is used only for functional addressing.
 */
int canTp_send_with_id(canTpLink *cantp_link, uint32_t arbit_id, const uint8_t payload[], uint16_t size);

/**
 * @brief Receives and parses the received data and copies the parsed data in to the internal buffer.
 * @param cantp_link The @cantp_link canTpLink @endlink instance used to transceive data.
 * @param payload A pointer to an area in memory where the raw data is copied from.
 * @param payload_size The size of the received (raw) CAN data.
 * @param out_size A reference to a variable which will contain the size of the actual (parsed) data.
 *
 * @return Possible return values:
 *      - @cantp_link ISOTP_RET_OK @endlink
 *      - @cantp_link ISOTP_RET_NO_DATA @endlink
 */
int CanTp_Receive(canTpLink *cantp_link, uint8_t *payload, const uint16_t payload_size, uint16_t *out_size);

#ifdef __cplusplus
}
#endif

#endif /* CANTP_H*/
