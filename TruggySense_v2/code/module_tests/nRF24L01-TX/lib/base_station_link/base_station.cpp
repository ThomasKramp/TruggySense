#include "base_station.h"
#include "RF24.h"

RF24 radio(CE_PIN, CSN_PIN);

uint8_t BaseStation::init()
{
    if (!radio.begin())
    {
        Serial.println(F("radio hardware is not responding!!"));
        return 0;
    }

    radio.setPALevel(RF24_PA_LOW); // RF24_PA_MAX is default.
    // radio.setPALevel(RF24_PA_MAX);
    radio.setPayloadSize(chunk_size);
    radio.openWritingPipe(addresses[0]);
    radio.stopListening();

    return 1;
}

uint8_t BaseStation::sendData(logging_data_t *data)
{
    // Receiver needs to have the same struct such that it can be translated to the right structure
    radio.flush_tx();
    int rx_chunk[chunks_count][chunk_size] = {0};
    Serial.println("Sedning data");
    toChunk(data, rx_chunk);

    Serial.println("chunked");

    long send_delay_timer = millis();

    u_int8_t error = 0;
    for (uint8_t i = 0; i < chunks_count; i++)
    {

        Serial.print("writing ");
        Serial.println(i);
        error += radio.writeFast(&rx_chunk[i], chunk_size);
        Serial.print("error ");
        Serial.println(error);
        while(millis()-send_delay_timer < 500)
        {
            //yield();
            delay(1);
        }
        send_delay_timer = millis();
    }

    return error == chunks_count;
}

uint8_t receiveData(logging_data_t *data)
{
    
}

void BaseStation::toChunk(logging_data *str, int buffer[chunks_count][chunk_size])
{
    uint8_t *str_buffer = reinterpret_cast<uint8_t *>(str);

    for (int i = 0; i < chunks_count; i++)
    {
        for (int j = 0; j < chunk_size; j++)
        {
            if (j == 0)
            {
                buffer[i][j] = i;
            }
            else
            {
                buffer[i][j] = *str_buffer; // add real value
                str_buffer++;               // increase pointer by 1 byte
            }
        }
    }
}

void BaseStation::toStruct(int buffer[chunks_count][chunk_size], logging_data *str)
{
    uint8_t rx_buffer[sizeof(logging_data)];
    int buffer_index = 0;
    for (int i = 0; i < chunks_count; i++)
    {
        for (int j = 1; j < chunk_size; j++)
        {
            rx_buffer[buffer_index] = buffer[i][j];
            buffer_index++;
        }
    }

    logging_data *temp = (logging_data *)rx_buffer;

    *str = *temp;
}