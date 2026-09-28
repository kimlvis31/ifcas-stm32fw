#ifndef TF02_PRO_H_
#define TF02_PRO_H_

#include <stdint.h>

#define TF02_PACKET_SIZE 9

typedef struct {
	uint32_t timestamp;
	uint16_t distance;
	uint16_t strength;
	float temperature;
} TF02_Data_t;

uint8_t Parse_TF02_PRO(TF02_Data_t *out_data);

#endif
