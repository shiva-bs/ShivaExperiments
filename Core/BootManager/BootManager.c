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
#include <BootImage_Manager.h>
#include <BootManager.h>
#include <stdio.h>

#include "stm32f4xx_hal.h"

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
extern  volatile int bootAppMagicFlag;
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


/*============Refer BootManager.h file for function description============*/
void bootManagerLoadImage(void)
{
	const BootImage_Header_t *hdr1 = NULL;
	const BootImage_Header_t *hdr2 = NULL;

	int valid1 = -1;
	int valid2 = -1;

	hdr1 = imgFetchHeader(IMG_SECTOR_1);
	hdr2 = imgFetchHeader(IMG_SECTOR_2);

	/*check if image_magic not matching*/
	if(hdr1 == NULL && hdr2 == NULL)
	{
		/*Valid image header not found*/

		bootAppMagicFlag=0xAABBCCDD;
		HAL_Delay(5000);
		/*"Restarting the system....*/
		NVIC_SystemReset();
		while(1);
	}


    /*get the validity(CRC verification) of each non-null image*/
    if(hdr1 != NULL)
    {
    	valid1 = performImgValidation(IMG_SECTOR_1, hdr1);
    }

    if(hdr2 != NULL)
    {
    	valid2 = performImgValidation(IMG_SECTOR_2, hdr2);
    }


    /*if both failed CRC verification*/
	if(valid1 != 0 && valid2 != 0)
	{
	   /*No valid image found....*/
	   /*Stay in Boot*/
		bootAppMagicFlag=0xAABBCCDD;
		HAL_Delay(5000);
	    /*Restarting the system....\r\n");*/
		NVIC_SystemReset();
		while(1);
	}



	/*print the App version for each image*/
	if(valid1 == 0)
	{
		  /*App found with version - Add Debug logs*/
	}

	if(valid2 == 0)
	{
		  /*App found with version - Add Debug logs*/
	}


	/*if image version at slot 1 is greater and image is valid, boot from it*/
    if((hdr1->img_version > hdr2->img_version) && valid1 == 0 )
    {
    	/*!!!!!!!!!!!!!! booting image from slot 1 !!!!!!!!!!!!!!!*/
    	/*Image Version...*/
    	imgStart(hdr1);
    }
    //image version at slot 2 is greater
    else if((hdr1->img_version < hdr2->img_version) && valid2 == 0)
    {
    	/*!!!!!!!!!!!!!! booting image from slot 2 !!!!!!!!!!!!!!!*/
    	/*Image Version */
    	imgStart(hdr2);
    }
    else /*in any other case, boot from whichever is valid*/
    {
        if(valid1 == 0)
        {
        	/*!!!!!!!!!!!!!! booting image from slot 1 !!!!!!!!!!!!!!!*/
        	/*Image Version*/
        	imgStart(hdr1);
        }
        else
        {
        	/*!!!!!!!!!!!!!! booting image from slot 2 !!!!!!!!!!!!!!!*/
        	/*Image Version*/
        	imgStart(hdr2);
        }
    }

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
