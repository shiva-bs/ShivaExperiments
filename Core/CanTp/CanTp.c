/*
 * @FileName: CanTp.c
 * @ Author:
 * @ Date:
 * @ Project:Bootloader Platform
 * @ Organization: Actevia
 * @ Copyright (c) 2024 Actevia.
 * @ All rights reserved.
 */

/*
  @brief            Implementation of CAN TP

  @note             Functions are safe, as far as given pointers (with respect
                    to their lengths) point to valid memory ranges.

  @details          This module provides some of functions to send and receive data through CAN TP.
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
#include <stdint.h>
#include "assert.h"
#include "CanTp.h"

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
#define ST_MIN_MAX          0x7F
#define ST_MIN_ONE_MS_START 0xF1
#define ST_MIN_ONE_MS_END   0xF9
#define ST_MIN_INVALID      0
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
* @Name-canTp_Ms_To_St_Min
* @details- Function to convert Frame separation time to us
* @param[out] -None
* @param[in]- m_sec : Frame separation time to convert
* @return converted time output.
*/
static uint8_t canTp_Ms_To_St_Min(uint8_t m_sec);

/**
* @Name-CanTp_St_Min_To_Ms
* @details- Function to convert Frame separation time to ms 
* @param[out] -None
* @param[in]- st_min : Frame separation time to convert
* @return converted time output.
*/
static uint8_t CanTp_St_Min_To_Ms(uint8_t st_min);

/**
* @Name-canTp_Send_Flow_Control
* @details- Function to control the flow of send data
* @param[out] -None
* @param[in]- cantp_link : CAN instance information
* @param[in]- flow_status : Status of data flow
* @param[in]- block_size : Data block size 
* @param[in]- st_min_ms : Frame separation time in ms
* @return status.
*/
static int canTp_Send_Flow_Control(canTpLink* cantp_link, uint8_t flow_status, uint8_t block_size, uint8_t st_min_ms);

/**
* @Name-canTp_send_single_frame
* @details- Function to send single frame data
* @param[out] -None
* @param[in]- cantp_link : CAN instance information
* @param[in]- arbit_id : arbitration ID
* @return status.
*/
static int canTp_send_single_frame(canTpLink* cantp_link, uint32_t arbit_id);

/**
* @Name-canTp_send_first_frame
* @details- Function to send first frame of data
* @param[out] -None
* @param[in]- cantp_link : CAN instance information
* @param[in]- arbit_id : arbitration ID
* @return status.
*/
static int canTp_send_first_frame(canTpLink* cantp_link, uint32_t arbit_id);

/**
* @Name-canTp_send_consecutive_frame
* @details- Function to send consecutive frames of data
* @param[out] -None
* @param[in]- cantp_link : CAN instance information
* @return status.
*/
static int canTp_send_consecutive_frame(canTpLink* cantp_link);

/**
* @Name-canTp_receive_single_frame
* @details- Function to receive single frame
* @param[out] -None
* @param[in]- cantp_link : CAN instance information
* @param[in]- cantp_msg : Data buffer to receive data
* @return status.
*/
static int canTp_receive_single_frame(canTpLink *cantp_link, CanTpMessage *cantp_msg, uint8_t len);

/**
* @Name-canTp_receive_first_frame
* @details- Function to receive first frame of data
* @param[out] -None
* @param[in]- cantp_link : CAN instance information
* @param[in]- cantp_msg : Data buffer to receive data
* @return status.
*/
static int canTp_receive_first_frame(canTpLink *cantp_link, CanTpMessage *cantp_msg, uint8_t len);

/**
* @Name-canTp_receive_consecutive_frame
* @details- Function to receive consecutive frames of data
* @param[out] -None
* @param[in]- cantp_link : CAN instance information
* @param[in]- cantp_msg : Data buffer to receive data
* @return status.
*/
static int canTp_receive_consecutive_frame(canTpLink *cantp_link, CanTpMessage *cantp_msg, uint8_t len);

/**
* @Name-canTp_receive_consecutive_frame
* @details- Function to control the flow of data while receiving
* @param[out] -None
* @param[in]- cantp_link : CAN instance information
* @param[in]- cantp_msg : Data buffer to receive data
* @return status.
*/
static int canTp_receive_flow_control_frame(canTpLink *cantp_link, CanTpMessage *cantp_msg, uint8_t len);
/*==============================================================================
                                LOCAL FUNCTIONS
==============================================================================*/

/*============Refer Local function protype sesction for description============*/

static uint8_t canTp_Ms_To_St_Min(uint8_t m_sec) 
{
    uint8_t st_min;

    st_min = m_sec;
    if (st_min > ST_MIN_MAX) 
    {
        st_min = ST_MIN_MAX;
    }

    return st_min;
}

/*===============================================================================*/

/*============Refer Local function proto-type section for description============*/

static uint8_t CanTp_St_Min_To_Ms(uint8_t st_min) 
{
    uint8_t m_sec;
    
    if (st_min >= ST_MIN_ONE_MS_START && st_min <= ST_MIN_ONE_MS_END) 
    {
        m_sec = 1;
    } 
    else if (st_min <= ST_MIN_MAX) 
    {
        m_sec = st_min;
    } 
    else 
    {
        m_sec = ST_MIN_INVALID;
    }

    return m_sec;
}

/*===============================================================================*/

/*============Refer Local function proto-type section for description============*/

static int canTp_Send_Flow_Control(canTpLink* cantp_link, uint8_t flow_status, uint8_t block_size, uint8_t st_min_ms) 
{

    CanTpMessage cantp_msg;
    int ret_status;

    /* setup message  */
    cantp_msg.as.flow_control.fc_type = CAN_TP_PCI_TYPE_FLOW_CONTROL_FRAME;
    cantp_msg.as.flow_control.flow_status = flow_status;
    cantp_msg.as.flow_control.block_size = block_size;
    cantp_msg.as.flow_control.st_min = canTp_Ms_To_St_Min(st_min_ms);

    /* send message */
#ifdef CAN_TP_FRAME_PADDING
    (void) memset(cantp_msg.as.flow_control.reserved, 0, sizeof(cantp_msg.as.flow_control.reserved));
    ret_status = canTp_User_Send_Can(cantp_link->transmit_arbitration_id, cantp_msg.as.data_array.ptr, sizeof(cantp_msg));
#else    
    ret_status = canTp_User_Send_Can(cantp_link->transmit_arbitration_id,
    		cantp_msg.as.data_array.ptr,
            3);
#endif

    return ret_status;
}

/*===============================================================================*/

/*============Refer Local function proto-type section for description============*/

static int canTp_send_single_frame(canTpLink* cantp_link, uint32_t arbit_id) 
{

	CanTpMessage message;
    int ret_status;

    /* multi frame message length must greater than 7  */
    assert(cantp_link->transmit_size <= 7);

    /* setup message  */
    message.as.single_frame.sf_type = CAN_TP_PCI_TYPE_SINGLE;
    message.as.single_frame.sf_data_length = (uint8_t) cantp_link->transmit_size;
    (void) memcpy(message.as.single_frame.sf_data, cantp_link->transmit_buffer, cantp_link->transmit_size);

    /* send message */
#ifdef CAN_TP_FRAME_PADDING
    (void) memset(message.as.single_frame.sf_data + cantp_link->transmit_size, 0, sizeof(message.as.single_frame.sf_data) - cantp_link->transmit_size);
    ret_status = canTp_User_Send_Can(arbit_id, message.as.data_array.ptr, sizeof(message));
#else
    ret_status = canTp_User_Send_Can(arbit_id,
            message.as.data_array.ptr,
            cantp_link->transmit_size + 1);
#endif

    return ret_status;
}

/*===============================================================================*/

/*============Refer Local function proto-type section for description============*/

static int canTp_send_first_frame(canTpLink* cantp_link, uint32_t arbit_id) 
{
    
	CanTpMessage cantp_msg;
    int ret_status;

    /* multi frame message length must greater than 7  */
    assert(cantp_link->transmit_size > 7);

    /* setup message  */
    cantp_msg.as.first_frame.ff_type = CAN_TP_PCI_TYPE_FIRST_FRAME;
    cantp_msg.as.first_frame.ff_data_length_low = (uint8_t) cantp_link->transmit_size;
    cantp_msg.as.first_frame.ff_data_length_high = (uint8_t) (0x0F & (cantp_link->transmit_size >> 8));
    (void) memcpy(cantp_msg.as.first_frame.ff_data, cantp_link->transmit_buffer, sizeof(cantp_msg.as.first_frame.ff_data));

    /* send message */
    ret_status = canTp_User_Send_Can(arbit_id, cantp_msg.as.data_array.ptr, sizeof(cantp_msg));
    if (CAN_TP_RET_OK == ret_status) {
        cantp_link->transmit_offset += sizeof(cantp_msg.as.first_frame.ff_data);
        cantp_link->transmit_seq_num = 1;
    }

    return ret_status;
}

/*===============================================================================*/

/*============Refer Local function proto-type section for description============*/

static int canTp_send_consecutive_frame(canTpLink* cantp_link) 
{
    
	CanTpMessage cantp_msg;
    uint16_t data_length;
    int ret_status;

    /* multi frame message length must greater than 7  */
    assert(cantp_link->transmit_size > 7);

    /* setup message  */
    cantp_msg.as.consecutive_frame.cf_type = CAN_TP_PCI_TYPE_CONSECUTIVE_FRAME;
    cantp_msg.as.consecutive_frame.cf_seq_num = cantp_link->transmit_seq_num;
    data_length = cantp_link->transmit_size - cantp_link->transmit_offset;
    if (data_length > sizeof(cantp_msg.as.consecutive_frame.cf_data)) {
        data_length = sizeof(cantp_msg.as.consecutive_frame.cf_data);
    }
    (void) memcpy(cantp_msg.as.consecutive_frame.cf_data, cantp_link->transmit_buffer + cantp_link->transmit_offset, data_length);

    /* send message */
#ifdef CAN_TP_FRAME_PADDING
    (void) memset(cantp_msg.as.consecutive_frame.cf_data + data_length, 0, sizeof(cantp_msg.as.consecutive_frame.cf_data) - data_length);
    ret_status = canTp_User_Send_Can(cantp_link->transmit_arbitration_id, cantp_msg.as.data_array.ptr, sizeof(cantp_msg));
#else
    ret_status = canTp_User_Send_Can(cantp_link->transmit_arbitration_id,
            cantp_msg.as.data_array.ptr,
            data_length + 1);
#endif
    if (CAN_TP_RET_OK == ret_status) {
        cantp_link->transmit_offset += data_length;
        if (++(cantp_link->transmit_seq_num) > 0x0F) {
            cantp_link->transmit_seq_num = 0;
        }
    }
    
    return ret_status;
}

/*===============================================================================*/

/*============Refer Local function proto-type section for description============*/

static int canTp_receive_single_frame(canTpLink *cantp_link, CanTpMessage *cantp_msg, uint8_t len) 
{
    /* check data length */
    if ((0 == cantp_msg->as.single_frame.sf_data_length) || (cantp_msg->as.single_frame.sf_data_length > (len - 1))) {
    	canTp_User_Debug("Single-frame length too small.");
        return CAN_TP_RET_LENGTH;
    }

    /* copying data */
    (void) memcpy(cantp_link->rcv_buffer, cantp_msg->as.single_frame.sf_data, cantp_msg->as.single_frame.sf_data_length);
    cantp_link->rcv_size = cantp_msg->as.single_frame.sf_data_length;
    
    return CAN_TP_RET_OK;
}

/*===============================================================================*/

/*============Refer Local function proto-type section for description============*/

static int canTp_receive_first_frame(canTpLink *cantp_link, CanTpMessage *cantp_msg, uint8_t len) 
{
    uint16_t payload_length;

    if (8 != len) {
    	canTp_User_Debug("First frame should be 8 bytes in length.");
        return CAN_TP_RET_LENGTH;
    }

    /* check data length */
    payload_length = cantp_msg->as.first_frame.ff_data_length_high;
    payload_length = (payload_length << 8) + cantp_msg->as.first_frame.ff_data_length_low;

    /* should not use multiple frame transmition */
    if (payload_length <= 7) {
    	canTp_User_Debug("Should not use multiple frame transmission.");
        return CAN_TP_RET_LENGTH;
    }
    
    if (payload_length > cantp_link->rcv_buf_size) {
    	canTp_User_Debug("Multi-frame response too large for receiving buffer.");
        return CAN_TP_RET_OVERFLOW;
    }
    
    /* copying data */
    (void) memcpy(cantp_link->rcv_buffer, cantp_msg->as.first_frame.ff_data, sizeof(cantp_msg->as.first_frame.ff_data));
    cantp_link->rcv_size = payload_length;
    cantp_link->rcv_offset = sizeof(cantp_msg->as.first_frame.ff_data);
    cantp_link->rcv_seq_num = 1;

    return CAN_TP_RET_OK;
}

/*===============================================================================*/

/*============Refer Local function proto-type section for description============*/

static int canTp_receive_consecutive_frame(canTpLink *cantp_link, CanTpMessage *cantp_msg, uint8_t len) 
{
    uint16_t pending_bytes;
    
    /* check sn */
    if (cantp_link->rcv_seq_num != cantp_msg->as.consecutive_frame.cf_seq_num) {
        return CAN_TP_RET_WRONG_SN;
    }

    /* check data length */
    pending_bytes = cantp_link->rcv_size - cantp_link->rcv_offset;
    if (pending_bytes > sizeof(cantp_msg->as.consecutive_frame.cf_data)) {
        pending_bytes = sizeof(cantp_msg->as.consecutive_frame.cf_data);
    }
    if (pending_bytes > len - 1) {
    	canTp_User_Debug("Consecutive frame too short.");
        return CAN_TP_RET_LENGTH;
    }

    /* copying data */
    (void) memcpy(cantp_link->rcv_buffer + cantp_link->rcv_offset, cantp_msg->as.consecutive_frame.cf_data, pending_bytes);

    cantp_link->rcv_offset += pending_bytes;
    if (++(cantp_link->rcv_seq_num) > 0x0F) {
        cantp_link->rcv_seq_num = 0;
    }

    return CAN_TP_RET_OK;
}

/*===============================================================================*/

/*============Refer Local function proto-type section for description============*/

static int canTp_receive_flow_control_frame(canTpLink *cantp_link, CanTpMessage *cantp_msg, uint8_t len) 
{
    /* check message length */
    if (len < 3) {
    	canTp_User_Debug("Flow control frame too short.");
        return CAN_TP_RET_LENGTH;
    }

    return CAN_TP_RET_OK;
}

/*===============================================================================*/

/*==============================================================================
                                GLOBAL FUNCTIONS
==============================================================================*/

/*============Refer CanTp.h file for function description============*/

int canTp_Send(canTpLink *cantp_link, const uint8_t payload[], uint16_t size) 
{
    return canTp_send_with_id(cantp_link, cantp_link->transmit_arbitration_id, payload, size);
}

/*===============================================================================*/

/*============Refer CanTp.h file for function description============*/

int canTp_send_with_id(canTpLink *cantp_link, uint32_t arbit_id, const uint8_t payload[], uint16_t size) 
{
    int ret_status;

    if (cantp_link == 0x0) {
    	canTp_User_Debug("Link is null!");
        return CAN_TP_RET_ERROR;
    }

    if (size > cantp_link->transmit_buf_size) {
    	canTp_User_Debug("Message size too large. Increase ISO_TP_MAX_MESSAGE_SIZE to set a larger buffer\n");
        char message[128];
        sprintf(&message[0], "Attempted to send %d bytes; max size is %d!\n", size, cantp_link->transmit_buf_size);
        return CAN_TP_RET_OVERFLOW;
    }

    if (CAN_TP_SEND_STATUS_INPROGRESS == cantp_link->transmit_status) {
    	canTp_User_Debug("Abort previous message, transmission in progress.\n");
        return CAN_TP_RET_INPROGRESS;
    }

    /* copy into local buffer */
    cantp_link->transmit_size = size;
    cantp_link->transmit_offset = 0;
    (void) memcpy(cantp_link->transmit_buffer, payload, size);

    if (cantp_link->transmit_size < 8) {
        /* send single frame */
        ret_status = canTp_send_single_frame(cantp_link, arbit_id);
    } else {
        /* send multi-frame */
        ret_status = canTp_send_first_frame(cantp_link, arbit_id);

        /* init multi-frame control flags */
        if (CAN_TP_RET_OK == ret_status) {
            cantp_link->transmit_bs_remain = 0;
            cantp_link->transmit_st_min = 0;
            cantp_link->transmit_wtf_count = 0;
            cantp_link->transmit_timer_st = canTp_User_GetMs();
            cantp_link->transmit_timer_bs = canTp_User_GetMs() + CAN_TP_DEFAULT_RESPONSE_TIMEOUT;
            cantp_link->transmit_protocol_result = CANTP_PROTOCOL_RESULT_OK;
            cantp_link->transmit_status = CAN_TP_SEND_STATUS_INPROGRESS;
        }
    }

    return ret_status;
}

/*===============================================================================*/

/*============Refer CanTp.h file for function description============*/

void canTp_On_CanMessage(canTpLink *cantp_link, uint8_t *data_buff, uint8_t data_len) 
{
	CanTpMessage cantp_msg;
    int ret_status;
    
    if (data_len < 2 || data_len > 8) {
        return;
    }

    memcpy(cantp_msg.as.data_array.ptr, data_buff, data_len);
    memset(cantp_msg.as.data_array.ptr + data_len, 0, sizeof(cantp_msg.as.data_array.ptr) - data_len);

    switch (cantp_msg.as.common.pci_type) {
        case CAN_TP_PCI_TYPE_SINGLE: {
            /* update protocol result */
            if (CAN_TP_RCV_STATUS_INPROGRESS == cantp_link->rcv_status) {
                cantp_link->rcv_protocol_result = CANTP_PROTOCOL_RESULT_UNEXP_PDU;
            } else {
                cantp_link->rcv_protocol_result = CANTP_PROTOCOL_RESULT_OK;
            }

            /* handle message */
            ret_status = canTp_receive_single_frame(cantp_link, &cantp_msg, data_len);
            
            if (CAN_TP_RET_OK == ret_status) {
                /* change status */
                cantp_link->rcv_status = CAN_TP_RCV_STATUS_FULL;
            }
            break;
        }
        case CAN_TP_PCI_TYPE_FIRST_FRAME: {
            /* update protocol result */
            if (CAN_TP_RCV_STATUS_INPROGRESS == cantp_link->rcv_status) {
                cantp_link->rcv_protocol_result = CANTP_PROTOCOL_RESULT_UNEXP_PDU;
            } else {
                cantp_link->rcv_protocol_result = CANTP_PROTOCOL_RESULT_OK;
            }

            /* handle message */
            ret_status = canTp_receive_first_frame(cantp_link, &cantp_msg, data_len);

            /* if overflow happened */
            if (CAN_TP_RET_OVERFLOW == ret_status) {
                /* update protocol result */
                cantp_link->rcv_protocol_result = CANTP_PROTOCOL_RESULT_BUFFER_OVFLW;
                /* change status */
                cantp_link->rcv_status = CAN_TP_RCV_STATUS_IDLE;
                /* send error message */
                canTp_Send_Flow_Control(cantp_link, CAN_TP_PCI_FLOW_STATUS_OVERFLOW, 0, 0);
                break;
            }

            /* if receive successful */
            if (CAN_TP_RET_OK == ret_status) {
                /* change status */
                cantp_link->rcv_status = CAN_TP_RCV_STATUS_INPROGRESS;
                /* send fc frame */
                cantp_link->rcv_bs_count = CAN_TP_DEFAULT_BLOCK_SIZE;
                canTp_Send_Flow_Control(cantp_link, CAN_TP_PCI_FLOW_STATUS_CONTINUE, cantp_link->rcv_bs_count, CAN_TP_DEFAULT_ST_MIN);
                /* refresh timer cs */
                cantp_link->rcv_timer_cr = canTp_User_GetMs() + CAN_TP_DEFAULT_RESPONSE_TIMEOUT;
            }
            
            break;
        }
        case CAN_TP_PCI_TYPE_CONSECUTIVE_FRAME: {
            /* check if in receiving status */
            if (CAN_TP_RCV_STATUS_INPROGRESS != cantp_link->rcv_status) {
                cantp_link->rcv_protocol_result = CANTP_PROTOCOL_RESULT_UNEXP_PDU;
                break;
            }

            /* handle message */
            ret_status = canTp_receive_consecutive_frame(cantp_link, &cantp_msg, data_len);

            /* if wrong sn */
            if (CAN_TP_RET_WRONG_SN == ret_status) {
                cantp_link->rcv_protocol_result = CANTP_PROTOCOL_RESULT_WRONG_SN;
                cantp_link->rcv_status = CAN_TP_RCV_STATUS_IDLE;
                break;
            }

            /* if success */
            if (CAN_TP_RET_OK == ret_status) {
                /* refresh timer cs */
                cantp_link->rcv_timer_cr = canTp_User_GetMs() + CAN_TP_DEFAULT_RESPONSE_TIMEOUT;
                
                /* receive finished */
                if (cantp_link->rcv_offset >= cantp_link->rcv_size) {
                    cantp_link->rcv_status = CAN_TP_RCV_STATUS_FULL;
                } else {
                    /* send fc when bs reaches limit */
                    if (0 == --cantp_link->rcv_bs_count) {
                        cantp_link->rcv_bs_count = CAN_TP_DEFAULT_BLOCK_SIZE;
                        canTp_Send_Flow_Control(cantp_link, CAN_TP_PCI_FLOW_STATUS_CONTINUE, cantp_link->rcv_bs_count, CAN_TP_DEFAULT_ST_MIN);
                    }
                }
            }
            
            break;
        }
        case CAN_TP_PCI_TYPE_FLOW_CONTROL_FRAME:
            /* handle fc frame only when sending in progress  */
            if (CAN_TP_SEND_STATUS_INPROGRESS != cantp_link->transmit_status) {
                break;
            }

            /* handle message */
            ret_status = canTp_receive_flow_control_frame(cantp_link, &cantp_msg, data_len);
            
            if (CAN_TP_RET_OK == ret_status) {
                /* refresh bs timer */
                cantp_link->transmit_timer_bs = canTp_User_GetMs() + CAN_TP_DEFAULT_RESPONSE_TIMEOUT;

                /* overflow */
                if (CAN_TP_PCI_FLOW_STATUS_OVERFLOW == cantp_msg.as.flow_control.flow_status) {
                    cantp_link->transmit_protocol_result = CANTP_PROTOCOL_RESULT_BUFFER_OVFLW;
                    cantp_link->transmit_status = CAN_TP_SEND_STATUS_ERROR;
                }

                /* wait */
                else if (CAN_TP_PCI_FLOW_STATUS_WAIT == cantp_msg.as.flow_control.flow_status) {
                    cantp_link->transmit_wtf_count += 1;
                    /* wait exceed allowed count */
                    if (cantp_link->transmit_wtf_count > CAN_TP_MAX_WFT_NUMBER) {
                        cantp_link->transmit_protocol_result = CANTP_PROTOCOL_RESULT_WFT_OVRN;
                        cantp_link->transmit_status = CAN_TP_SEND_STATUS_ERROR;
                    }
                }

                /* permit send */
                else if (CAN_TP_PCI_FLOW_STATUS_CONTINUE == cantp_msg.as.flow_control.flow_status) {
                    if (0 == cantp_msg.as.flow_control.block_size) {
                        cantp_link->transmit_bs_remain = CANTP_INVALID_BS;
                    } else {
                        cantp_link->transmit_bs_remain = cantp_msg.as.flow_control.block_size;
                    }
                    cantp_link->transmit_st_min = CanTp_St_Min_To_Ms(cantp_msg.as.flow_control.st_min);
                    cantp_link->transmit_wtf_count = 0;
                }
            }
            break;
        default:
            break;
    };
    
    return;
}

/*===============================================================================*/

/*============Refer CanTp.h file for function description============*/

int CanTp_Receive(canTpLink *cantp_link, uint8_t *payload, const uint16_t payload_size, uint16_t *out_size)
 {
    uint16_t copy_len;
    
    if (CAN_TP_RCV_STATUS_FULL != cantp_link->rcv_status) {
        return CAN_TP_RET_NO_DATA;
    }

    copy_len = cantp_link->rcv_size;
    if (copy_len > payload_size) {
        copy_len = payload_size;
    }

    memcpy(payload, cantp_link->rcv_buffer, copy_len);
    *out_size = copy_len;

    cantp_link->rcv_status = CAN_TP_RCV_STATUS_IDLE;

    return CAN_TP_RET_OK;
}

/*===============================================================================*/

/*============Refer CanTp.h file for function description============*/

void canTp_Init_Link(canTpLink *cantp_link, uint32_t transmit_id, uint8_t *transmit_data_buf, uint16_t transmit_data_len, uint8_t *rcv_data_buf, uint16_t rcv_data_len) 
{
    memset(cantp_link, 0, sizeof(*cantp_link));
    cantp_link->rcv_status = CAN_TP_RCV_STATUS_IDLE;
    cantp_link->transmit_status = CAN_TP_SEND_STATUS_IDLE;
    cantp_link->transmit_arbitration_id = transmit_id;
    cantp_link->transmit_buffer = transmit_data_buf;
    cantp_link->transmit_buf_size = transmit_data_len;
    cantp_link->rcv_buffer = rcv_data_buf;
    cantp_link->rcv_buf_size = rcv_data_len;
    
    return;
}

/*===============================================================================*/

/*============Refer CanTp.h file for function description============*/

void canTp_Poll(canTpLink *cantp_link) 
{
    int ret_status;

    /* only polling when operation in progress */
    if (CAN_TP_SEND_STATUS_INPROGRESS == cantp_link->transmit_status) {

        /* continue send data */
        if (/* send data if bs_remain is invalid or bs_remain large than zero */
        (CANTP_INVALID_BS == cantp_link->transmit_bs_remain || cantp_link->transmit_bs_remain > 0) &&
        /* and if st_min is zero or go beyond interval time */
        (0 == cantp_link->transmit_st_min || (0 != cantp_link->transmit_st_min && CanTpIsTimeAfter(canTp_User_GetMs(), cantp_link->transmit_timer_st)))) {
            
            ret_status = canTp_send_consecutive_frame(cantp_link);
            if (CAN_TP_RET_OK == ret_status) {
                if (CANTP_INVALID_BS != cantp_link->transmit_bs_remain) {
                    cantp_link->transmit_bs_remain -= 1;
                }
                cantp_link->transmit_timer_bs = canTp_User_GetMs() + CAN_TP_DEFAULT_RESPONSE_TIMEOUT;
                cantp_link->transmit_timer_st = canTp_User_GetMs() + cantp_link->transmit_st_min;

                /* check if send finish */
                if (cantp_link->transmit_offset >= cantp_link->transmit_size) {
                    cantp_link->transmit_status = CAN_TP_SEND_STATUS_IDLE;
                }
            } else {
                cantp_link->transmit_status = CAN_TP_SEND_STATUS_ERROR;
            }
        }

        /* check timeout */
        if (CanTpIsTimeAfter(canTp_User_GetMs(), cantp_link->transmit_timer_bs)) {
            cantp_link->transmit_protocol_result = CANTP_PROTOCOL_RESULT_TIMEOUT_BS;
            cantp_link->transmit_status = CAN_TP_SEND_STATUS_ERROR;
        }
    }

    /* only polling when operation in progress */
    if (CAN_TP_RCV_STATUS_INPROGRESS == cantp_link->rcv_status) {
        
        /* check timeout */
        if (CanTpIsTimeAfter(canTp_User_GetMs(), cantp_link->rcv_timer_cr)) {
            cantp_link->rcv_protocol_result = CANTP_PROTOCOL_RESULT_TIMEOUT_CR;
            cantp_link->rcv_status = CAN_TP_RCV_STATUS_IDLE;
        }
    }

    return;
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



