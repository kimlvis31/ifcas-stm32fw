#include "main.h" // HAL 드라이버 헤더
#include "hw_callbacks.h"
#include "ring_buffer.h"
#include "dcache.h"

// Core/main.c 등에 선언된 하드웨어 핸들 참조
extern UART_HandleTypeDef huart2;
extern TIM_HandleTypeDef htim5;
extern DCACHE_HandleTypeDef hdcache1;

// DMA 수신 버퍼와 상태 변수를 여기서 관리합니다.
uint8_t g_dma_rx_buf[DMA_BUF_SIZE] __attribute__((aligned(32)));
volatile HAL_StatusTypeDef g_dma_start_status = HAL_ERROR;

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size) {
    if (huart->Instance == USART2) {
        uint32_t trigger_time = __HAL_TIM_GET_COUNTER(&htim5);

        HAL_DCACHE_InvalidateByAddr(&hdcache1, (uint32_t *)g_dma_rx_buf, DMA_BUF_SIZE);
        LRF_RingBuffer_Write(g_dma_rx_buf, Size, trigger_time);

        __HAL_UART_CLEAR_FLAG(&huart2, UART_CLEAR_OREF | UART_CLEAR_NEF | UART_CLEAR_FEF | UART_CLEAR_PEF);
        g_dma_start_status = HAL_UARTEx_ReceiveToIdle_DMA(&huart2, g_dma_rx_buf, DMA_BUF_SIZE);
        __HAL_DMA_DISABLE_IT(huart->hdmarx, DMA_IT_HT);
    }
}
