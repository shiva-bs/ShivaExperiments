/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef CAN_TP_DEFINES_H
#define CAN_TP_DEFINES_H

/*
 * @FileName: CanTp_Defines.h
 * @ Author:
 * @ Date:
 * @ Project:Bootloader Platform
 * @ Organization: Actevia
 * @ Copyright (c) 2024 Actevia.
 * @ All rights reserved.
 */

/*
  @brief            Typedef used for CAN TP Imp

  @note

  @details
*/

/**************************************************************
 * compiler specific defines
 *************************************************************/
#ifdef __GNUC__
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#define CANTP_BYTE_ORDER_LITTLE_ENDIAN
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#else
#error "unsupported byte ordering"
#endif
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/**************************************************************
 * OS specific defines
 *************************************************************/
#ifdef _WIN32
#define snprintf _snprintf
#endif

#ifdef _WIN32
#define CANTP_BYTE_ORDER_LITTLE_ENDIAN
#define __builtin_bswap8  _byteswap_uint8
#define __builtin_bswap16 _byteswap_uint16
#define __builtin_bswap32 _byteswap_uint32
#define __builtin_bswap64 _byteswap_uint64
#endif

/**************************************************************
 * internal used defines
 *************************************************************/
#define CAN_TP_RET_OK           0
#define CAN_TP_RET_ERROR        -1
#define CAN_TP_RET_INPROGRESS   -2
#define CAN_TP_RET_OVERFLOW     -3
#define CAN_TP_RET_WRONG_SN     -4
#define CAN_TP_RET_NO_DATA      -5
#define CAN_TP_RET_TIMEOUT      -6
#define CAN_TP_RET_LENGTH       -7

/* Exported types ------------------------------------------------------------*/
/* CAN-TP sender status */
typedef enum {
    CAN_TP_SEND_STATUS_IDLE,
    CAN_TP_SEND_STATUS_INPROGRESS,
    CAN_TP_SEND_STATUS_ERROR,
} CanTpSendStatusTypes;

/* CAN-TP receiver status */
typedef enum {
    CAN_TP_RCV_STATUS_IDLE,
	CAN_TP_RCV_STATUS_INPROGRESS,
	CAN_TP_RCV_STATUS_FULL,
} CanTpReceiveStatusTypes;

/* CAN frame Definition */
#if defined(CANTP_BYTE_ORDER_LITTLE_ENDIAN)

typedef struct {
    uint8_t reserved_1:4;          /*Reserve 1*/            
    uint8_t pci_type:4;            /*PCI Type*/
    uint8_t reserved_2[7];         /*Reserve 2*/
} CanTpPciType;

/*
* single frame
* +-------------------------+-----+
* | byte #0                 | ... |
* +-------------------------+-----+
* | nibble #0   | nibble #1 | ... |
* +-------------+-----------+ ... +
* | PCIType = 0 | SF_DL     | ... |
* +-------------+-----------+-----+
*/
typedef struct {
    uint8_t sf_data_length:4;      /*Single Frame Data Length*/
    uint8_t sf_type:4;             /*Single Frame Type*/
    uint8_t sf_data[7];            /*Single Frame Data*/
} CanTpSingleFrame;

/*
* first frame
* +-------------------------+-----------------------+-----+
* | byte #0                 | byte #1               | ... |
* +-------------------------+-----------+-----------+-----+
* | nibble #0   | nibble #1 | nibble #2 | nibble #3 | ... |
* +-------------+-----------+-----------+-----------+-----+
* | PCIType = 1 | FF_DL                             | ... |
* +-------------+-----------+-----------------------+-----+
*/
typedef struct {
    uint8_t ff_data_length_high:4; /*First Frame Data Length High*/
    uint8_t ff_type:4;             /*First Frame Type*/
    uint8_t ff_data_length_low;    /*First Frame Data Length Low*/
    uint8_t ff_data[6];            /*First Frame Data*/
} CanTpFirstFrame;

/*
* consecutive frame
* +-------------------------+-----+
* | byte #0                 | ... |
* +-------------------------+-----+
* | nibble #0   | nibble #1 | ... |
* +-------------+-----------+ ... +
* | PCIType = 0 | SN        | ... |
* +-------------+-----------+-----+
*/
typedef struct {
    uint8_t cf_seq_num:4;          /*Sequence Number*/
    uint8_t cf_type:4;             /*Consecutive Frame Type*/
    uint8_t cf_data[7];            /*Consecutive Frame Data*/
} CanTpConsecutiveFrame;

/*
* flow control frame
* +-------------------------+-----------------------+-----------------------+-----+
* | byte #0                 | byte #1               | byte #2               | ... |
* +-------------------------+-----------+-----------+-----------+-----------+-----+
* | nibble #0   | nibble #1 | nibble #2 | nibble #3 | nibble #4 | nibble #5 | ... |
* +-------------+-----------+-----------+-----------+-----------+-----------+-----+
* | PCIType = 1 | FS        | BS                    | STmin                 | ... |
* +-------------+-----------+-----------------------+-----------------------+-----+
*/
typedef struct {
    uint8_t flow_status:4;         /*Flow Status*/
    uint8_t fc_type:4;             /*Flow Control Type*/
    uint8_t block_size;            /*Block Size*/
    uint8_t st_min;                /*Separation Time Minimum*/
    uint8_t reserved[5];           /*Reserved*/
} CanTpFlowControl;

#else

typedef struct {
    uint8_t pci_type:4;      /*PCI Type*/
    uint8_t reserved_1:4;    /*Reserve 1*/
    uint8_t reserved_2[7];   /*Reserve 2*/
} CanTpPciType;

/*
* single frame
* +-------------------------+-----+
* | byte #0                 | ... |
* +-------------------------+-----+
* | nibble #0   | nibble #1 | ... |
* +-------------+-----------+ ... +
* | PCIType = 0 | SF_DL     | ... |
* +-------------+-----------+-----+
*/
typedef struct {
    uint8_t sf_type:4;        /*Single Frame Type*/
    uint8_t sf_data_length:4; /*Single Frame Data Length*/
    uint8_t sf_data[7];       /*Single Frame Data*/
} CanTpSingleFrame;

/*
* first frame
* +-------------------------+-----------------------+-----+
* | byte #0                 | byte #1               | ... |
* +-------------------------+-----------+-----------+-----+
* | nibble #0   | nibble #1 | nibble #2 | nibble #3 | ... |
* +-------------+-----------+-----------+-----------+-----+
* | PCIType = 1 | FF_DL                             | ... |
* +-------------+-----------+-----------------------+-----+
*/
typedef struct {
    uint8_t ff_type:4;                /*First Frame Type*/
    uint8_t ff_data_length_high:4;    /*First Frame Data Length High*/
    uint8_t ff_data_length_low;       /*First Frame Data Length Low*/
    uint8_t ff_data[6];               /*First Frame Data*/
} CanTpFirstFrame;

/*
* consecutive frame
* +-------------------------+-----+
* | byte #0                 | ... |
* +-------------------------+-----+
* | nibble #0   | nibble #1 | ... |
* +-------------+-----------+ ... +
* | PCIType = 0 | SN        | ... |
* +-------------+-----------+-----+
*/
typedef struct {
    uint8_t cf_type:4;         /*Consecutive Frame Type*/
    uint8_t sequence_number:4; /*Sequence Number*/
    uint8_t cf_data[7];        /*Consecutive Frame Data*/
} CanTpConsecutiveFrame;

/*
* flow control frame
* +-------------------------+-----------------------+-----------------------+-----+
* | byte #0                 | byte #1               | byte #2               | ... |
* +-------------------------+-----------+-----------+-----------+-----------+-----+
* | nibble #0   | nibble #1 | nibble #2 | nibble #3 | nibble #4 | nibble #5 | ... |
* +-------------+-----------+-----------+-----------+-----------+-----------+-----+
* | PCIType = 1 | FS        | BS                    | STmin                 | ... |
* +-------------+-----------+-----------------------+-----------------------+-----+
*/
typedef struct {
    uint8_t fc_type:4;        /**/ Flow Control Type
    uint8_t flow_status:4;    /**/ Flow Status
    uint8_t block_size;        /**/ Block Size
    uint8_t st_min;            /**/ Separation Time Minimum
    uint8_t reserved[5];       /**/ Reserved
} CanTpFlowControl;

#endif

typedef struct {
    uint8_t ptr[8];
} CanTpDataArray;

typedef struct {
    union {
    	CanTpPciType          common;
    	CanTpSingleFrame      single_frame;
    	CanTpFirstFrame       first_frame;
    	CanTpConsecutiveFrame consecutive_frame;
        CanTpFlowControl      flow_control;
        CanTpDataArray        data_array;
    } as;
} CanTpMessage;

/**************************************************************
 * protocol specific defines
 *************************************************************/

/* Private: Protocol Control Information (PCI) types, for identifying each frame of an ISO-TP message.
 */
typedef enum {
	CAN_TP_PCI_TYPE_SINGLE             = 0x0,
	CAN_TP_PCI_TYPE_FIRST_FRAME        = 0x1,
	CAN_TP_PCI_TYPE_CONSECUTIVE_FRAME  = 0x2,
	CAN_TP_PCI_TYPE_FLOW_CONTROL_FRAME = 0x3
} CanTpProtocolControlInformation;

/* Private: Protocol Control Information (PCI) flow control identifiers.
 */
typedef enum {
    CAN_TP_PCI_FLOW_STATUS_CONTINUE = 0x0,
	CAN_TP_PCI_FLOW_STATUS_WAIT     = 0x1,
	CAN_TP_PCI_FLOW_STATUS_OVERFLOW = 0x2
} CanTpFlowStatus;
/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/
/* return logic true if 'a' is after 'b' */
#define CanTpIsTimeAfter(a,b) ((int32_t)((int32_t)(b) - (int32_t)(a)) < 0)

/*  invalid bs */
#define CANTP_INVALID_BS       0xFFFF


/* Private: network layer result code.
 */
#define CANTP_PROTOCOL_RESULT_OK            0
#define CANTP_PROTOCOL_RESULT_TIMEOUT_A    -1
#define CANTP_PROTOCOL_RESULT_TIMEOUT_BS   -2
#define CANTP_PROTOCOL_RESULT_TIMEOUT_CR   -3
#define CANTP_PROTOCOL_RESULT_WRONG_SN     -4
#define CANTP_PROTOCOL_RESULT_INVALID_FS   -5
#define CANTP_PROTOCOL_RESULT_UNEXP_PDU    -6
#define CANTP_PROTOCOL_RESULT_WFT_OVRN     -7
#define CANTP_PROTOCOL_RESULT_BUFFER_OVFLW -8
#define CANTP_PROTOCOL_RESULT_ERROR        -9
/* Exported functions prototypes ---------------------------------------------*/
/**
* @Name-FunctionName
* @details-
* @param[out] -
* @param[in]  -
* @return
* */

#endif /*CAN_TP_DEFINES_H*/

