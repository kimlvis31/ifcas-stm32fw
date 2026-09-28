#ifndef LOGGER_H_
#define LOGGER_H_

#include "cmsis_os2.h"
#include <stdio.h>

typedef struct {
	char text[64];
} LogMessage_t;

extern osMessageQueueId_t logQueueHandle; // 외부(app_freertos.c)에 있는 큐 핸들 참조

#define LOG_PRINT(fmt, ...) do { \
	LogMessage_t log_msg; \
	snprintf(log_msg.text, sizeof(log_msg.text), fmt, ##__VA_ARGS__); \
	if (logQueueHandle != NULL) { \
		osMessageQueuePut(logQueueHandle, &log_msg, 0, 0); \
	} \
} while (0)

#endif
