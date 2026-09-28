/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : app_freertos.c
  * Description        : FreeRTOS applicative file
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
#include "app_freertos.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
#include <dcache.h>

/* Application Modules */
#include "sensor_fusion.h"
#include "logger.h"
#include "ring_buffer.h"

#include "task_system_monitor.h"
#include "task_read.h"
#include "task_compute.h"
#include "task_write.h"
#include "task_log.h"
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
/* Definitions for readTask */
osThreadId_t readTaskHandle;
const osThreadAttr_t readTask_attributes = {
  .name = "readTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 512 * 4
};
/* Definitions for compTask */
osThreadId_t compTaskHandle;
const osThreadAttr_t compTask_attributes = {
  .name = "compTask",
  .priority = (osPriority_t) osPriorityAboveNormal,
  .stack_size = 512 * 4
};
/* Definitions for writeTask */
osThreadId_t writeTaskHandle;
const osThreadAttr_t writeTask_attributes = {
  .name = "writeTask",
  .priority = (osPriority_t) osPriorityRealtime,
  .stack_size = 256 * 4
};
/* Definitions for logTask */
osThreadId_t logTaskHandle;
const osThreadAttr_t logTask_attributes = {
  .name = "logTask",
  .priority = (osPriority_t) osPriorityLow,
  .stack_size = 256 * 4
};
/* Definitions for systemMonitorTask */
osThreadId_t systemMonitorTaskHandle;
const osThreadAttr_t systemMonitorTask_attributes = {
  .name = "systemMonitorTask",
  .priority = (osPriority_t) osPriorityBelowNormal,
  .stack_size = 256 * 4
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
/* USER CODE END FunctionPrototypes */

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

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */
  /* creation of readTask */
  readTaskHandle = osThreadNew(StartReadTask, NULL, &readTask_attributes);

  /* creation of compTask */
  compTaskHandle = osThreadNew(StartCompTask, NULL, &compTask_attributes);

  /* creation of writeTask */
  writeTaskHandle = osThreadNew(StartWriteTask, NULL, &writeTask_attributes);

  /* creation of logTask */
  logTaskHandle = osThreadNew(StartLogTask, NULL, &logTask_attributes);

  /* creation of systemMonitorTask */
  systemMonitorTaskHandle = osThreadNew(StartSystemMonitorTask, NULL, &systemMonitorTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}
/* USER CODE BEGIN Header_StartReadTask */
/**
* @brief Function implementing the readTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartReadTask */
void StartReadTask(void *argument)
{
  /* USER CODE BEGIN readTask */
  AppTask_Read(argument);
  /* USER CODE END readTask */
}

/* USER CODE BEGIN Header_StartCompTask */
/**
* @brief Function implementing the compTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartCompTask */
void StartCompTask(void *argument)
{
  /* USER CODE BEGIN compTask */
  AppTask_Compute(argument);
  /* USER CODE END compTask */
}

/* USER CODE BEGIN Header_StartWriteTask */
/**
* @brief Function implementing the writeTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartWriteTask */
void StartWriteTask(void *argument)
{
  /* USER CODE BEGIN writeTask */
  AppTask_Write(argument);
  /* USER CODE END writeTask */
}

/* USER CODE BEGIN Header_StartLogTask */
/**
* @brief Function implementing the logTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartLogTask */
void StartLogTask(void *argument)
{
  /* USER CODE BEGIN logTask */
  AppTask_Log(argument);
  /* USER CODE END logTask */
}

/* USER CODE BEGIN Header_StartSystemMonitorTask */
/**
* @brief Function implementing the systemMonitorTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartSystemMonitorTask */
void StartSystemMonitorTask(void *argument)
{
  /* USER CODE BEGIN systemMonitorTask */
  AppTask_System_Monitor(argument);
  /* USER CODE END systemMonitorTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
/* USER CODE END Application */

