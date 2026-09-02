#include "main.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>

logging_data data;

constexpr int chunks_count = 8;
constexpr int chunk_size = 30 + 1;

void toChunk(logging_data* str, int buffer[chunks_count][chunk_size]);
void toStruct(int buffer[chunks_count][chunk_size], logging_data* str);

int main() {
    char* str = "robbe is cool       ";

    uint8_t* str_buffer = reinterpret_cast<uint8_t *>(str);
    uint8_t buffer[4][5] = {0};

    std::cout << str_buffer[0] << std::endl;
    int max_length = 20;
    int counter = 0;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 5; j++) {
                buffer[i][j] = str_buffer[counter]; //add real value
            counter++;
            if (counter >= max_length) {
                str_buffer++; //increase pointer by 1 byte
            }
        }
    }

    data.altitude = 10;
    data.latitude = 20;
    data.longitude = 30;
    data.v_cell_1 = 3.3;
    data.v_cell_2 = 4.4;
    data.v_cell_3 = 5.5;
    data.velocity = 3.14;
    data.n_failsafe = 20;
    data.i_esc_4 = 200;

    std::cout << sizeof(data) << std::endl;

    int rx_chunk[chunks_count][chunk_size] = {0};

    toChunk(&data, rx_chunk);

    for (int i = 0; i < 4; i++) {
        printf("packet numbers: %d \n\r", rx_chunk[i][0]);
    }

    //Here, the data should be transmitted and received based on the first index.
    //At the receiver, the index should be checked before saved.

    logging_data received_data{};
    toStruct(rx_chunk, &received_data);

    std::cout << received_data.altitude << std::endl;

    std::cout << (int)data.n_failsafe << "  ";
    std::cout << (int)received_data.n_failsafe << std::endl;

    std::cout << (int)data.i_esc_4 << "  ";
    std::cout << (int)received_data.i_esc_4 << std::endl;

    return 0;
}

uint8_t* encode(logging_data* data) {
    return reinterpret_cast<uint8_t *>(data);
}

logging_data* decode(uint8_t* data) {
    return reinterpret_cast<logging_data *>(data);
}

void toChunk(logging_data* str, int buffer[chunks_count][chunk_size])
{
    u_int8_t* str_buffer = reinterpret_cast<u_int8_t *>(str);

    std::cout << str_buffer[0] << std::endl;

    for (int i = 0; i < chunks_count; i++) {
         for (int j = 0; j < chunk_size; j++) {
             if (j == 0) {
                 buffer[i][j] = i;
             }
             else {
                 buffer[i][j] = *str_buffer; //add real value
                 str_buffer++; //increase pointer by 1 byte
             }
         }
     }
}
void toStruct(int buffer[chunks_count][chunk_size], logging_data* str)
{
    uint8_t rx_buffer[sizeof(logging_data)];
    int buffer_index = 0;
    for (int i = 0; i < chunks_count; i++) {
         for (int j = 1; j < chunk_size; j++) {
             rx_buffer[buffer_index] = buffer[i][j];
             std::cout << (buffer_index) << std::endl;
             buffer_index++;
         }
    }

    logging_data* temp = (logging_data*)rx_buffer;

    *str = *temp;
}