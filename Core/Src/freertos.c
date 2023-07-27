/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "freertosUtils.h"
#include "usart.h"
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
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for xUart1Transmit */
osThreadId_t xUart1TransmitHandle;
const osThreadAttr_t xUart1Transmit_attributes = {
  .name = "xUart1Transmit",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for xUart2Transmit */
osThreadId_t xUart2TransmitHandle;
const osThreadAttr_t xUart2Transmit_attributes = {
  .name = "xUart2Transmit",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for xUart1RecievedQueue */
osMessageQueueId_t xUart1RecievedQueueHandle;
const osMessageQueueAttr_t xUart1RecievedQueue_attributes = {
  .name = "xUart1RecievedQueue"
};
/* Definitions for xUart2RecievedQueue */
osMessageQueueId_t xUart2RecievedQueueHandle;
const osMessageQueueAttr_t xUart2RecievedQueue_attributes = {
  .name = "xUart2RecievedQueue"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void xUart1TransmitTask(void *argument);
void xUart2TransmitTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

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
  /* creation of xUart1RecievedQueue */
  xUart1RecievedQueueHandle = osMessageQueueNew (16, sizeof(uint8_t), &xUart1RecievedQueue_attributes);

  /* creation of xUart2RecievedQueue */
  xUart2RecievedQueueHandle = osMessageQueueNew (16, sizeof(uint8_t), &xUart2RecievedQueue_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of xUart1Transmit */
  xUart1TransmitHandle = osThreadNew(xUart1TransmitTask, NULL, &xUart1Transmit_attributes);

  /* creation of xUart2Transmit */
  xUart2TransmitHandle = osThreadNew(xUart2TransmitTask, NULL, &xUart2Transmit_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_xUart1TransmitTask */
/**
  * @brief  Function implementing the xUart1Transmit thread.
  * wait byte from queue and send it to UART
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_xUart1TransmitTask */
void xUart1TransmitTask(void *argument)
{
  /* USER CODE BEGIN xUart1TransmitTask */
  /* Infinite loop */
  osStatus_t operationStatus = osOK;
  uint8_t recievedByte = 0;
  for(;;)
  {
	  operationStatus = osMessageQueueGet(xUart2RecievedQueueHandle, &recievedByte, NULL, osWaitForever);   // wait for message
	  if (operationStatus == osOK)
	  {
		  HAL_UART_Transmit_IT (getHuart1Handler(), &recievedByte, 1); //Cause we read per byte
	  }
  }
  /* USER CODE END xUart1TransmitTask */
}

/* USER CODE BEGIN Header_xUart2TransmitTask */
/**
* @brief Function implementing the xUart2Transmit thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_xUart2TransmitTask */
void xUart2TransmitTask(void *argument)
{
  /* USER CODE BEGIN xUart2TransmitTask */
  /* Infinite loop */
  osStatus_t operationStatus = osOK;
  uint8_t recievedByte = 0;
  for(;;)
  {
	  operationStatus = osMessageQueueGet(xUart1RecievedQueueHandle, &recievedByte, NULL, osWaitForever);   // wait for message
	  if (operationStatus == osOK)
	  {
		  HAL_UART_Transmit_IT (getHuart2Handler(), &recievedByte, 1); //Cause we read per byte
	  }
  }
  /* USER CODE END xUart2TransmitTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
/**
  * @brief  getter for huart1 queue handler.
  * @param  argument: Not used
  * @retval uart1Recieve queue handler
  */
osMessageQueueId_t getUart1RecievedQueueHandle()
{
	return xUart1RecievedQueueHandle;
}

/**
  * @brief  getter for huart2 queue handler.
  * @param  argument: Not used
  * @retval uart2Recieve queue handler
  */
osMessageQueueId_t getUart2RecievedQueueHandle()
{
	return xUart2RecievedQueueHandle;
}
/* USER CODE END Application */

