/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "main.h"
#include "cmsis_os.h"
#include "usb_device.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <usbd_cdc_if.h>
#include <stdio.h>
#include <string.h>
#include "task.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim2;

/* Definitions for cliTask */
osThreadId_t cliTaskHandle;
const osThreadAttr_t cliTask_attributes = {
  .name = "cliTask",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for heartbeatTask */
osThreadId_t heartbeatTaskHandle;
const osThreadAttr_t heartbeatTask_attributes = {
  .name = "heartbeatTask",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for usbRxQueue */
osMessageQueueId_t usbRxQueueHandle;
const osMessageQueueAttr_t usbRxQueue_attributes = {
  .name = "usbRxQueue"
};
/* USER CODE BEGIN PV */
char txBuffer[512];
uint32_t lastHeartbeatTick =0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
void StartCliTask(void *argument);
void StartHeartbeatTask(void *argument);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
extern uint8_t UserRxBufferFS[APP_RX_DATA_SIZE];
extern uint32_t rec_Len;
extern uint8_t rec_Done;

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
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of usbRxQueue */
  usbRxQueueHandle = osMessageQueueNew (16, 64, &usbRxQueue_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of cliTask */
  cliTaskHandle = osThreadNew(StartCliTask, NULL, &cliTask_attributes);

  /* creation of heartbeatTask */
  heartbeatTaskHandle = osThreadNew(StartHeartbeatTask, NULL, &heartbeatTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
//	  if (HAL_GetTick() - lastHeartbeatTick >=500 ){
//		  lastHeartbeatTick = HAL_GetTick();
//		  HAL_GPIO_TogglePin(GPIOD,GPIO_PIN_13);
//	  }
//
//	  if(rec_Done==1){
//
//	  	  	rec_Done=0;
//
//	  	  	if (strncmp((char *)UserRxBufferFS,"HELP",4)==0){
//	  	  		int len =snprintf(txBuffer,sizeof(txBuffer),"\r\n--CLI COMNDS--\r\n"
//	  	  				"Help = show cmd menu\r\n"
//	  	  				"Uptime = show system run time\r\n"
//	  	  				"Led on/off = toggle led \r\n ");
//	  	  		CDC_Transmit_FS((uint8_t *)txBuffer,(uint16_t)len);
//	  	  	}
//
//
//	  	  	else if (strncmp((char *)UserRxBufferFS,"ON",2)==0){
//	  	  		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12,GPIO_PIN_SET);
//	  	  		CDC_Transmit_FS((uint8_t *)"LED ON\r\n",7);
//	  	  	}
//	  	  	else if(strncmp((char *)UserRxBufferFS,"OFF",3)==0){
//	  	  		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12,GPIO_PIN_RESET);
//	  	  		CDC_Transmit_FS((uint8_t *)"LED OFF\r\n",8);
//	  	  	}
//	  	  	else{
//	  	  		CDC_Transmit_FS((uint8_t *)"unknown cmd\r\n",12);
//
//	  	  	}
//
//	  	  }
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
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 83;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 4294967295;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12|GPIO_PIN_13, GPIO_PIN_RESET);

  /*Configure GPIO pins : PD12 PD13 */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartCliTask */
/**
  * @brief  Function implementing the cliTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartCliTask */
void StartCliTask(void *argument)
{
  /* init code for USB_DEVICE */
  MX_USB_DEVICE_Init();
  /* USER CODE BEGIN 5 */
  char rxCmd[64];

  /* Infinite loop */
  for(;;)
  {
    if (osMessageQueueGet(usbRxQueueHandle, rxCmd, NULL, osWaitForever) == osOK)
    {
      if (strncmp(rxCmd, "HELP", 4) == 0)
      {
        int len = snprintf(txBuffer, sizeof(txBuffer),
                           "\r\n--- REAL-TIME PROFILER SHELL ---\r\n"
                           "HELP   : Show command menu\r\n"
                           "UPTIME : Show OS uptime\r\n"
                           "TASKS  : CPU utilization & execution stats\r\n"
                           "MEM    : Task stack high-water marks\r\n"
                           "ON/OFF : Control Green LED\r\n"
                           "--------------------------------\r\n");
        CDC_Transmit_FS((uint8_t *)txBuffer, (uint16_t)len);
      }
      else if (strncmp(rxCmd, "TASKS", 5) == 0)
      {
        int headerLen = snprintf(txBuffer, sizeof(txBuffer),
                                 "\r\nTask\t\tAbs Time(us)\t%% Time\r\n"
                                 "-----------------------------------------\r\n");
        vTaskGetRunTimeStats(txBuffer + headerLen);
        CDC_Transmit_FS((uint8_t *)txBuffer, (uint16_t)strlen(txBuffer));
      }
      else if (strncmp(rxCmd, "MEM", 3) == 0)
      {
        int headerLen = snprintf(txBuffer, sizeof(txBuffer),
                                 "\r\nTask\t\tState\tPrio\tMin Free Stack\tNum\r\n"
                                 "-----------------------------------------------------\r\n");
        vTaskList(txBuffer + headerLen);
        CDC_Transmit_FS((uint8_t *)txBuffer, (uint16_t)strlen(txBuffer));
      }
      else if (strncmp(rxCmd, "UPTIME", 6) == 0)
      {
        uint32_t totalSec = osKernelGetTickCount() / 1000;
        uint32_t hours   = totalSec / 3600;
        uint32_t minutes = (totalSec % 3600) / 60;
        uint32_t seconds = totalSec % 60;

        int len = snprintf(txBuffer, sizeof(txBuffer),
                           "OS Uptime: %02lu:%02lu:%02lu\r\n",
                           hours, minutes, seconds);
        CDC_Transmit_FS((uint8_t *)txBuffer, (uint16_t)len);
      }
      else if (strncmp(rxCmd, "ON", 2) == 0)
      {
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);
        CDC_Transmit_FS((uint8_t *)"LED ON\r\n", 8);
      }
      else if (strncmp(rxCmd, "OFF", 3) == 0)
      {
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);
        CDC_Transmit_FS((uint8_t *)"LED OFF\r\n", 9);
      }
      else
      {
        CDC_Transmit_FS((uint8_t *)"Err: Unknown command\r\n", 22);
      }
    }
  }
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_StartHeartbeatTask */
/**
* @brief Function implementing the heartbeatTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartHeartbeatTask */
void StartHeartbeatTask(void *argument)
{
  /* USER CODE BEGIN StartHeartbeatTask */
  /* Infinite loop */
  for(;;)
  {
    HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13);
    // Yields the CPU to allow lower priority tasks or IDLE to run for 500ms
    osDelay(500);
  }
  /* USER CODE END StartHeartbeatTask */
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM1 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM1)
  {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

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
#ifdef USE_FULL_ASSERT
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
