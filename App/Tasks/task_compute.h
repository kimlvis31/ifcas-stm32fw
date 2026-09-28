#ifndef TASK_COMPUTE_H_
#define TASK_COMPUTE_H_

#include <stdint.h>

// 외부 모듈(예: CAN 송신 태스크)에서 이 변수를 읽을 수 있도록 extern 선언
extern float g_distance;
extern float g_temperature;

void AppTask_Compute(void *argument);

#endif
