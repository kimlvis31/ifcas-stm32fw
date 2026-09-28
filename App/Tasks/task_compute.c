#include "task_compute.h"
#include "sensor_fusion.h"
#include "logger.h"
#include "cmsis_os2.h"

// Core/main.c 혹은 app_freertos.c에 있는 큐 핸들 참조
extern osMessageQueueId_t sensorQueueHandle;

// 변수의 실체(메모리 할당)는 이곳에 선언
float g_distance = 0.0f;
float g_temperature = 0.0f;

void AppTask_Compute(void *argument) {
    UnifiedSensor_t rx_sensor;

    // 초기화가 필요한 로직이 있다면 이곳(for 루프 진입 전)에 작성

    for(;;) {
        // 1. 센서 데이터가 큐에 들어올 때까지 무한 대기 (Block 상태)
        if (osMessageQueueGet(sensorQueueHandle, &rx_sensor, NULL, osWaitForever) == osOK) {

            // 2. 비동기 로그 출력
            LOG_PRINT("[Calc] Time:%luus | Dist:%ucm | Temp:%.1fC",
                      rx_sensor.sync_timestamp,
                      rx_sensor.lrf.distance,
                      rx_sensor.lrf.temperature);

            // 3. 전역 변수 업데이트
            g_distance = (float)rx_sensor.lrf.distance;
            g_temperature = rx_sensor.lrf.temperature;

            // 4. Jetson CAN 타겟팅 데이터 동기화 및 사격 통제(IFCAS) 보조 연산 호출
            // App/Algorithms/ 디렉토리에 있는 순수 수학 함수들을 여기서 호출합니다.
            // 예: Target_Estimator_Run(&rx_sensor, &g_distance);
        }
    }
}
