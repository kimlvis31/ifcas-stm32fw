#include "task_read.h"
#include "tf02_pro.h"
#include "ring_buffer.h"
#include "sensor_fusion.h"
#include "main.h"
#include "cmsis_os2.h"

// Core Hardware + OS Handle Reference
extern UART_HandleTypeDef huart2;
extern osMessageQueueId_t sensorQueueHandle;

// DMA Buffer Reference From hw_callbacks.c
extern uint8_t g_dma_rx_buf[];
extern volatile HAL_StatusTypeDef g_dma_start_status;

void AppTask_Read(void *argument) {
	// 1. Sensor Data
	// 1.1. System Control
	static uint16_t batt_pack_voltage;

	// 1.2. Fire Control
    TF02_Data_t     parsed_lrf;
    UnifiedSensor_t sensor_box;





    // 2. DMA Initialization
    // --- 2.1. Clear Possible Error Flags Remaining In The UART2 Hardware Register.
    __HAL_UART_CLEAR_FLAG(&huart2, UART_CLEAR_OREF | UART_CLEAR_NEF | UART_CLEAR_FEF | UART_CLEAR_PEF);

    // --- 2.2. Initial Read-And-Disposal of Receive Data Register (RDR). (Some Error Flags Of STM32 UART Are Properly Cleared Only Once RDR Is Initially Read).
    volatile uint8_t dummy_flush = (uint8_t)huart2.Instance->RDR;
    (void)dummy_flush;

    // --- 2.3. Link DMA Receiver To UART.
    g_dma_start_status = HAL_UARTEx_ReceiveToIdle_DMA(&huart2, g_dma_rx_buf, DMA_BUF_SIZE);

    // --- 2.4. Disable Half-Transfer Interrupt (As Is Unnecessary And Thus To Avoid Meaningless Overhead).
    __HAL_DMA_DISABLE_IT(huart2.hdmarx, DMA_IT_HT);

    // --- 2.5. Activate Receiver By Raising RE (Receiver Enable) Bit of UART Control Register 1 (UART CR1) Only Once DMA Is Started.
    if (g_dma_start_status == HAL_OK) {
        SET_BIT(huart2.Instance->CR1, USART_CR1_RE);
    }





    // 3. Main Loop
    for (;;) {
    	//3.1. Successful Parsing Check
    	if (!Parse_TF02_PRO(&parsed_lrf)) {
    		osDelay(1);
    		continue;
    	}

    	//3.2. Signal Strength Check
    	if (parsed_lrf.strength < 100) {
    		osDelay(1);
    		continue;
    	}

    	//3.3. Sensor Data Collection
		sensor_box.sync_timestamp = parsed_lrf.timestamp;
		sensor_box.lrf = parsed_lrf;
		sensor_box.imu.accel_x = 0.0f;
		sensor_box.zoom_level = 1;

		//3.4. Sensor Data Queue Update
		if (sensorQueueHandle != NULL) {
			osMessageQueuePut(sensorQueueHandle, &sensor_box, 0, 0);
		}

		//3.5. Loop Delay
        osDelay(1);
    }
}
