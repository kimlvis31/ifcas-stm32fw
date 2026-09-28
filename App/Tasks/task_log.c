#include "task_log.h"
#include "logger.h"
#include "cmsis_os2.h"

// app_freertos.c에 선언된 로그 큐 핸들 참조
extern osMessageQueueId_t logQueueHandle;

void AppTask_Log(void *argument) {
    LogMessage_t rx_log;

    for(;;) {
        // 1. 로그 메시지가 큐에 쌓일 때까지 무한 대기 (CPU 부하 0%)
        if (osMessageQueueGet(logQueueHandle, &rx_log, NULL, osWaitForever) == osOK) {

            // 2. 물리적 출력 (printf 또는 ITM / UART 전송)
            // printf("%s\n", rx_log.text);
        }
    }
}
