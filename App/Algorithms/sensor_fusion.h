#ifndef SENSOR_FUSION_H_
#define SENSOR_FUSION_H_

#include "tf02_pro.h"
#include "imu_driver.h"

typedef struct {
	uint16_t zoom_level;

	uint32_t sync_timestamp;

	TF02_Data_t lrf;
	IMU_Data_t  imu;

} UnifiedSensor_t;

#endif
