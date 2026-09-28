#ifndef RING_BUFFER_H_
#define RING_BUFFER_H_

#include <stdint.h>

#define RX_RING_BUF_SIZE 128
#define DMA_BUF_SIZE     64

typedef struct {
	uint8_t buffer[RX_RING_BUF_SIZE];
	uint32_t timestamps[RX_RING_BUF_SIZE];
	volatile uint16_t head;
	volatile uint16_t tail;
} UART_RingBuffer_t;

void LRF_RingBuffer_Write(uint8_t *data, uint16_t size, uint32_t timestamp);
uint8_t LRF_RingBuffer_Read(uint8_t *byte, uint32_t *timestamp);

#endif
