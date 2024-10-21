/*
 * @FileName: DevNullSocket.c
 * @ Author:
 * @ Date:
 * @ Project:Bootloader Platform
 * @ Organization: Actevia
 * @ Copyright (c) 2024 Actevia.
 * @ All rights reserved.
 */

/*
  @brief            Implementation of NULL Socket.

  @note             Functions are safe, as far as given pointers (with respect
                    to their lengths) point to valid memory ranges.

  @details          This module provides the functionality of NULL Socket.
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

#include <DevNullSocket.h>

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
                           LOCAL FUNCTION PROTOTYPES
==============================================================================*/
/**
* @Name- DevNull_numOfData
* @details- handles no. of data
* @param[out] - None
* @param[in]- None
* @return int32_t
*/
static int32_t DevNull_numOfData(void);

/**
* @Name-DevNull_transmit
* @details- transmit function
* @param[out] - None
* @param[in]-buf, len
* @return int32_t
*/
static int32_t DevNull_receive(uint8_t *buf, uint32_t len);

/**
* @Name-DevNull_transmit
* @details- transmit function
* @param[out] - None
* @param[in]-buf, len
* @return int32_t
*/
static int32_t DevNull_transmit(const uint8_t *buf, uint32_t len);

/*==============================================================================
                  LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==============================================================================*/
const ISocket_t DevNullSocket =
{
        DevNull_numOfData,
        DevNull_receive,
        DevNull_transmit
};

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
                                LOCAL FUNCTIONS
==============================================================================*/


static int32_t DevNull_numOfData(void)
{
    return 0;
}

static int32_t DevNull_receive(uint8_t *buf, uint32_t len)
{
    return 0;
}

static int32_t DevNull_transmit(const uint8_t *buf, uint32_t len)
{
    return len;
}

/*==============================================================================
                                GLOBAL FUNCTIONS
==============================================================================*/



/* -------------------------------------------- END OF FILE -------------------------------------------- */
/* =================================================================================
                                              FILE LOG HISTORY
=====================================================================================
*  Date    Changes      Author     Change_Set
*
*
*************************************************************************************/

