#include <stdio.h>
#include <stdint.h>

//Simulation
#define PACKET_LENGTH 32
//uint8_t ibus_data[PACKET_LENGTH] = {0x20, 0x40, 0xDB, 0x5, 0xDC, 0x5, 0x54, 0x5, 0xDC, 0x5, 0xE8, 0x3, 0xD0, 0x7, 0xD2, 0x5, 0xE8, 0x3, 0xDC, 0x5, 0xDC, 0x5, 0xDC, 0x5, 0xDC, 0x5, 0xDC, 0x5, 0xDC, 0x5, 0xDA, 0xF3};
uint8_t ibus_data[] = {0x20, 0x40, 0xdc, 0x5, 0xf8, 0x5, 0xef, 0x3, 0xc5, 0x5, 0xe3, 0x5, 0xe1, 0x5, 0xe8, 0x3, 0xe8, 0x3, 0xe8, 0x3, 0xe8, 0x3, 0xdc, 0x5, 0xdc, 0x5, 0xdc, 0x5, 0xe8, 0x3, 0xfd, 0xf2};
uint8_t index = 0;
//Simulation

#define DATA_LENGTH (28 + 2) //14 channels of 2 byte + 2 start byte
#define START_FRAME_0 0x20
#define START_FRAME_1 0x40

enum decoder_states {
    detect,
    data,
    validate,
    decode,
    discard
};

decoder_states state = detect; //Start to detect data

uint16_t crc = 0;
uint8_t data_counter = 0;
uint8_t raw_data[DATA_LENGTH] = {0};
uint16_t crc_calculated = 0xFFFF;

int main() {
    data_counter = 0;
    while (index != PACKET_LENGTH ) {
        uint8_t received_data = ibus_data[index];

        switch (state) {
            case detect:
                if (received_data == START_FRAME_0) {
                    crc_calculated -= received_data;
                    data_counter++;
                }
                else if (data_counter == 1 && received_data == START_FRAME_1) {
                    crc_calculated -= received_data;
                    data_counter++;
                    state = data;
                }
                else {
                    state = discard;
                }
                break;
            case data:
                raw_data[data_counter] = received_data;
                crc_calculated -= raw_data[data_counter];
                data_counter++;

                if (data_counter == DATA_LENGTH) {
                    state = validate;
                }
                break;
            case validate:
                if (data_counter == DATA_LENGTH) {
                    crc = received_data;
                    data_counter++;
                }
                else if (data_counter == DATA_LENGTH + 1) {
                    crc |= (received_data << 8);
                    printf("%d\n", crc_calculated);
                    printf("%d\n", crc);
                    state = decode;
                }

                break;

            case decode:
                for (int i = 2; i < DATA_LENGTH-1; i+=2) {
                    printf("%d: %d ", i/2, raw_data[i] | raw_data[i+1] << 8);
                }

            case discard:
                data_counter = 0;
                crc_calculated = 0xFFFF;
                state = detect;
                //flush buffer
                break;
        }
        index++; //Simulate the index for the bus.read
    }
    printf("Hello, World!\n");
    return 0;
 }