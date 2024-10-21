/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include <CanTp_Socket.h>
#include "main.h"
#include "can.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "BootManager.h"
#include "driverClock.h"
#include <stdbool.h>
#include <Uds_App_Interface.h>
#include "driverFlash.h"
#include "DriverCan.h"
#include "Uds_DiagnosticAndCommunicationManagementFunctionalUnit.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
CAN_HandleTypeDef hcan1;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
const uint8_t Test_Bl_Signal[8]={'I','A','M','I','N','B','L','-'};
const uint8_t Test_Bl_ResetSignal[8]={'R','E','S','E','T','B','L','-'};
extern volatile driverFlash_Division_t Erased_Partion;
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
extern const DefaultSessionTimings_t defaultTimings[sscType_amount];
__attribute__((section(".noinit"))) volatile int bootAppMagicFlag;
bool uds_reset_request = false;


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();

  /* USER CODE BEGIN 2 */
  MX_CAN1_Init();
  HAL_CAN_Start(&hcan1);
  if (HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)
     {
   	    Error_Handler();
     }
  /*Reset Detected*/

  /*This shall be removed. To be used on during development phase*/
  /*Wait in Transition State for 1 sec*/
  HAL_Delay(1000);
  canTp_User_Send_Can(0x500,Test_Bl_ResetSignal,8);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	     if(bootAppMagicFlag==0xAABBCCDD)
	     {
	    	 canTp_User_Send_Can(0x500,Test_Bl_Signal,8);
	    	 canTp_Socket_Init();
	    	 init(isotp_socket);

	    	 /*Diwakar: Set programming session as Active and start S3 timer*/
	    	 sscSetSession(sscType_programming, defaultTimings[sscType_programming].p2, defaultTimings[sscType_programming].p2star);
	       /*Stay in Boot and wait for UDS to start*/
	    	while(1)
	    	{
	    		/*Call Diagnostics Stack and CAN Stack State Machine*/
	    		/*If download is successful,Clear Programming Signature Magic Flag and Reset*/
	    		/*If S3 Times Out Clear Programming Signature Magic Flag and Reset System*/
	    		canTp_Socket_Task();
	    		task();

	            if (uds_reset_request == true)
	            {
	                uint32_t entryTime = driverClock_FetchRunTime();
	                while (driverClock_FetchRunTime() - entryTime < 5)
	                {
	                	canTp_Socket_Task();
	                    task();
	                }
	                /*Clear Magic flag*/
	                bootAppMagicFlag=(uint32_t)0;
	                Erased_Partion= driverFlash_Division_NULL;
	                /*Reset System*/
	                driverClock_McuRst();
	            }
	       	    /*SYS Interrupt based CAN Messages Transmission for Test. Shall be removed and used only for development Phase*/
	       	    static uint32_t getMs_before= 0;
	       	    uint32_t getMs_Now= driverClock_FetchRunTime();
	       	    if ((getMs_Now >= getMs_before && (getMs_Now - getMs_before) > 1000) || \
	       	    (getMs_Now < getMs_before && (UINT32_MAX - getMs_before + getMs_Now) > 1000))
	       	     {
	       	      getMs_before=getMs_Now;
	       	  // HAL_GPIO_TogglePin(LED_GPIO_Port,LED_Pin);
	       	     }
	    	}

	     }
	     else
	     {
	    /*Jump to Valid and latest Application flashed.*/
	    	 bootManagerLoadImage();
	     }
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 100;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{

		uint8_t data[8];

	    uint8_t size = driverCan_FetchMessage(data);
	    if (size != 0)
	    {
	    	canTp_On_CanMessage(&cantp_link, data, size);
	    }
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
