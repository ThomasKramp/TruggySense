/**
 * @file base_station.cpp
 * @author Robbe Elsermans
 * @brief Implementation of base_station.h
 * @version 1.0
 * @date 2025-04-28
 * 
 * @copyright Copyright (c) 2025 Robbe Elsermans, All rights reserved.
 * This project is released under the CC BY-NC-SA licence.
 * See licence.txt for more information
 * 
 */

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

  // radio.setPALevel(RF24_PA_LOW); // RF24_PA_MAX is default.
  radio.setPALevel(RF24_PA_MAX);
  radio.openWritingPipe(path);
  radio.stopListening();

  if(!is_initialized)
  {
    tryouts = MAXIMUM_TRYOUT;
    is_initialized = 1;
  }

  return 1;
}

uint8_t BaseStation::sendData(logging_data_t data)
{
  // Serial.println(tryouts);
  if (tryouts > 0) // only try to send something if tryouts is not depleted yet.
  {
    // make a copy
    memcpy(&payload, &data, sizeof(data));

    uint8_t *str_buffer = reinterpret_cast<uint8_t *>(&payload);
    uint8_t buffer[chunks_count][chunk_size] = {0};

    toChunk(str_buffer, buffer);
    uint8_t error = 0;

    for (int i = 0; i < chunks_count; i++)
    {
      error += radio.write(&buffer[i], sizeof(buffer[i]));
    }
    Serial.println(error);
    if (error >= (int)chunks_count * (0.8)) // 20% dropout maximum allowed.
    {
      // Serial.println("increase");
      tryouts++; // Increase tryouts if we had like 1 failure between all good transmits.

      if (tryouts >= MAXIMUM_TRYOUT)
        tryouts = MAXIMUM_TRYOUT;
    }
    else
    {
      // Serial.println("decreace");
      tryouts--; // Decrease tryouts if failure happened that is 20% dropout of packets.
      if (tryouts <= 0)
      {
        tryouts = 0;
      }
    }
    if (error == 0) // The NRF is not responding at all. Try to restart it.
    {
      BaseStation::init();
    }
  }
  return tryouts != 0;
}

void BaseStation::toChunk(uint8_t *payload, uint8_t chunked_payload[chunks_count][chunk_size])
{
  int counter = 0;
  for (int i = 0; i < chunks_count; i++)
  {
    for (int j = 0; j < chunk_size; j++)
    {
      if (j == 0)
        chunked_payload[i][j] = (uint8_t)i;
      else
      {
        if (counter < payload_length)
        {
          chunked_payload[i][j] = *payload; // add real value
          payload++;
        }
        else
          chunked_payload[i][j] = 114; // padding
        counter++;
      }
    }
  }
}
