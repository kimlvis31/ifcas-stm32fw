#include "ring_buffer.h"

// 링 버퍼 전역 변수 할당
UART_RingBuffer_t g_lrf_ring_buffer = {0};

void LRF_RingBuffer_Write(uint8_t *data, uint16_t size, uint32_t timestamp) {
    for (uint16_t i = 0; i < size; i++) {
        uint16_t next_head = (g_lrf_ring_buffer.head + 1) % RX_RING_BUF_SIZE;
        if (next_head != g_lrf_ring_buffer.tail) {
            g_lrf_ring_buffer.buffer[g_lrf_ring_buffer.head] = data[i];
            g_lrf_ring_buffer.timestamps[g_lrf_ring_buffer.head] = timestamp;
            g_lrf_ring_buffer.head = next_head;
        }
    }
}

uint8_t LRF_RingBuffer_Read(uint8_t *byte, uint32_t *timestamp) {
    if (g_lrf_ring_buffer.head == g_lrf_ring_buffer.tail) return 0;

    *byte = g_lrf_ring_buffer.buffer[g_lrf_ring_buffer.tail];
    *timestamp = g_lrf_ring_buffer.timestamps[g_lrf_ring_buffer.tail];
    g_lrf_ring_buffer.tail = (g_lrf_ring_buffer.tail + 1) % RX_RING_BUF_SIZE;
    return 1;
}
