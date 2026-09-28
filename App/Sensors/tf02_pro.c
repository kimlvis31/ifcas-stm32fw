#include "tf02_pro.h"
#include "ring_buffer.h"

uint8_t Parse_TF02_PRO(TF02_Data_t *out_data) {
    static uint8_t packet[TF02_PACKET_SIZE];
    static uint32_t packet_timestamps[TF02_PACKET_SIZE];
    static uint8_t state = 0;
    static uint8_t data_cnt = 0;
    uint8_t rx_byte;
    uint32_t rx_time;

    while (LRF_RingBuffer_Read(&rx_byte, &rx_time)) {
        switch (state) {
            case 0: if (rx_byte == 0x59) { packet[0] = rx_byte; packet_timestamps[0] = rx_time; state = 1; } break;
            case 1: if (rx_byte == 0x59) { packet[1] = rx_byte; packet_timestamps[1] = rx_time; data_cnt = 2; state = 2; } else state = 0; break;
            case 2:
                packet[data_cnt] = rx_byte; packet_timestamps[data_cnt] = rx_time; data_cnt++;
                if (data_cnt >= TF02_PACKET_SIZE) {
                    uint16_t checksum = 0;
                    for (uint8_t i = 0; i < 8; i++) checksum += packet[i];
                    state = 0;

                    if ((checksum & 0xFF) == packet[8]) { //Validation Success
                        out_data->timestamp   = packet_timestamps[8];
                        out_data->distance    = packet[2] | (packet[3] << 8);
                        out_data->strength    = packet[4] | (packet[5] << 8);
                        out_data->temperature = (((float)(packet[6] | (packet[7] << 8))) / 8.0f) - 256.0f;
                        return 1;
                    }
                }
                break;
        }
    }
    return 0;
}
