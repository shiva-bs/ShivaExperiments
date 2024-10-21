/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef DRIVER_CLOCK_H
#define DRIVER_CLOCK_H

/*
 * @FileName: DriverClock.h
 * @ Author:
 * @ Date:
 * @ Project:Bootloader Platform
 * @ Organization: Actevia
 * @ Copyright (c) 2024 Actevia.
 * @ All rights reserved.
 */

/*
  @brief            Implementation of Can TP Debug,send data and Get MS

  @note             Functions are safe, as far as given pointers (with respect
                    to their lengths) point to valid memory ranges.

  @details          This module provides some of functions which helps to debug, send data and get time in ms.
*/


/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/


/* Exported functions prototypes ---------------------------------------------*/

/**
 * @name driverClock_Initialize
 * @brief Initialization of System Clock and Time Base.
 * This function should be called directly after POR.
 * No need to wait for data and bss initialitation.
 * All system clocks and dependencies are initialized here.
 */
void driverClock_Initialize(void);

/**
 * @name driverClock_FetchRunTime
 * @brief Runtime of System.
 * The time in milliseconds since power on reset is returned by this
 * function.
 * @return Time since POR in milliseconds.
 */
uint32_t driverClock_FetchRunTime(void);

/**
 * @name driverClock_FetchSysClock
 * @brief Frequency of Core.
 * This is the frequency, with wich the mcu is running.
 * @return The frequency in Hz.
 */
uint32_t driverClock_FetchSysClock(void);

/**
 * @name driverClock_AddTick
 * @brief Interrupt Callback for Time Measurement.
 * This function is only called in interrupt context of
 * systick (or any other timer) when 1 millisecond has
 * elapsed.
 */
void driverClock_AddTick(void);

/**
 * @name driverClock_McuRst
 * @brief Perform a software reset.
 * This function does not return.
 */
void driverClock_McuRst (void);

#endif /* DRIVER_CLOCK_H */

