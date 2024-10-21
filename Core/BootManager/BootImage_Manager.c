/*
 * @FileName: BootImage_Manager.c
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
#include "BootImage_Manager.h"
#include <stdio.h>
#include "main.h"
#include "driverFlash.h"
#include "can.h"

#include "stm32f4xx_hal.h"
#include "crc.h"

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
* @Name-FunctionName
* @details-
* @param[out] -
* @param[in]-
* @return
*/

/*==============================================================================
                                LOCAL FUNCTIONS
==============================================================================*/


/*==============================================================================
                                GLOBAL FUNCTIONS
==============================================================================*/


/*============Refer BootImage_Manager.h file for function description============*/

const BootImage_Header_t *imgFetchHeader(BootImage_Sector_t sector)
{
	 const BootImage_Header_t *img_hdr = NULL;

	 switch (sector)
	 {
		 case IMG_SECTOR_1:
			 img_hdr = (const BootImage_Header_t *)(FLASH_AREA_IMAGE_1);
			 break;
		 case IMG_SECTOR_2:
			 img_hdr = (const BootImage_Header_t *)(FLASH_AREA_IMAGE_2);
			 break;
		 default:
			 break;
	 }

	 if (img_hdr && img_hdr->img_magic == IMAGE_MAGIC)
	 {
		 return img_hdr;
	 }
	 else
	 {
		/*!!!!!!!!!!!!!!!!!! Valid header not found in sector*/
		 return NULL;
	 }

}

/*===============================================================================*/

/*============Refer BootImage_Manager.h file for function description============*/

uint8_t performImgValidation(BootImage_Sector_t sector, const BootImage_Header_t *img_hdr)
{
	void *sector_addr = NULL;

	if(sector == IMG_SECTOR_1)
	{
		sector_addr = (uint32_t *)(FLASH_AREA_IMAGE_1);
	}
	else
	{
		sector_addr = (uint32_t *)(FLASH_AREA_IMAGE_2);
	}

	sector_addr += sizeof(BootImage_Header_t);
	uint32_t img_data_len = img_hdr->img_size;
	uint32_t crc_temp_a = crc32(sector_addr, img_data_len);
	uint32_t crc_temp_b = img_hdr->data_crc;
	if (crc_temp_a == crc_temp_b)
	{
	    return 0;
	}
	else
	{
		/*CRC not matching*/
	    return -1;
	}

}

/*===============================================================================*/

/*============Refer BootImage_Manager.h file for function description============*/

void imgStart(const BootImage_Header_t *img_hdr)
{


	/*Booting from sector*/
	void (*app_reset_handler)(void);


	uint32_t msp_value = 0;
	uint32_t reset_handler_address  = 0 ;

    /*get the stack pointer*/


	/*odd image version in sector1 and even image version in sector2*/
	if(img_hdr->img_version%2)
	{
		msp_value = *(volatile uint32_t *)(FLASH_AREA_IMAGE_1 + IMAGE_OFFSET);
	}
	else
	{
		msp_value = *(volatile uint32_t *)(FLASH_AREA_IMAGE_2 + IMAGE_OFFSET);

	}

	printf("msp_value %ld\r\n",msp_value);
	__set_MSP(msp_value);



	/*get the reset handler address*/
	if(img_hdr->img_version%2)
	{
		reset_handler_address  = *(volatile uint32_t *)(FLASH_AREA_IMAGE_1 + IMAGE_OFFSET + 4);

	}
	else
	{
		reset_handler_address = *(volatile uint32_t *)(FLASH_AREA_IMAGE_2 + IMAGE_OFFSET + 4);
	//	uint32_t effective_address=FLASH_AREA_IMAGE_2+(reset_handler_address-FLASH_AREA_IMAGE_1);
	//	reset_handler_address=effective_address;


	}


	app_reset_handler = (void *)reset_handler_address;



	/*disable each each peripheral to its reset state (to_test)
	 *also to disable CPU and peripheral interrupts*/
	CAN_HandleTypeDef hcan1 = getCanHandle();
	HAL_DeInit();
	HAL_CAN_MspDeInit(&hcan1);
	//HAL_CRC_MspDeInit();
	//HAL_UART_MspDeInit();
	__HAL_RCC_SYSCFG_CLK_DISABLE();
	__HAL_RCC_PWR_CLK_DISABLE();
	//__diable_irq();

	/*jump to reset handler of the image*/
	app_reset_handler();

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
