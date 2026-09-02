/**
 * @file X6B.cpp
 * @author Robbe Elsermans
 * @brief
 * @version 0.1
 * @date 2025-04-10
 *
 * @copyright Copyright (c) 2025 Robbe Elsermans, All rights reserved.
 * This project is released under the CC BY-NC-SA licence.
 * See licence.txt for more information
 *
 */

#include "X6B.h"

IBUS::IBUS()
{
}

IBUS::IBUS(HardwareSerial &serial)
{
  s = &serial;
  s->begin(115200, SERIAL_8N1);

  // set default channel setting
  setDefaultChannelValue();
  pre_start_frame = false;
  start_frame_detected = false;
  is_decoding = false;
  buffer_index = 0;
}

uint8_t IBUS::readData()
{
  // Do not overrun the uart. No use as it only transmits every 7ms.
  trigger_time = millis();
  if ((millis() - timer) < INTER_FRAME_STOP)
  {
    // Serial.println("To fast");
    return 2;
  }
  this->timer = millis();

  uint8_t input = 0;
  crc_calculated = 0xFFFF;
  while (s->available() > 0)
  {
    input = s->read(); // Read the buffer

    if ((!pre_start_frame && !start_frame_detected) && input == (START_FRAME_SEQUENCE & 0xFF00) >> 8)
    {
      crc_calculated -= input;
      pre_start_frame = true;
    }

    // Check if starting sequence pops up.
    if ((pre_start_frame && !start_frame_detected) && input == (START_FRAME_SEQUENCE & 0xFF))
    {

      start_frame_detected = true;
      alive = true; // The connection is alive
      crc_calculated -= input;
      continue;
    }

    // If starting sequence has popped up, save each byte in the buffer
    if ((start_frame_detected && pre_start_frame) || is_decoding) // && input != (START_FRAME_SEQUENCE & 0xFF))
    {
      is_decoding = true;
      pre_start_frame = false;
      start_frame_detected = false;
      alive = true;
      buffer[buffer_index] = input;

      if (buffer_index < NO_CRC_FRAME_LENGTH - 2)
        crc_calculated -= input;

      buffer_index++;
      if (buffer_index == NO_CRC_FRAME_LENGTH)
      {
        start_frame_detected = false;
        buffer_index = 0;
        uint16_t crc = buffer[29] | buffer[28] << 8;

        if (crc == crc_calculated)
        {
          uint8_t ch_i = 0;
          for (uint8_t i = 0; i < NO_CRC_FRAME_LENGTH; i += 2)
          {
            uint16_t temp = buffer[i] | buffer[i + 1] << 8;

            // if value is not what it is, keep previous value in channel buffer.
            if (temp >= 950 && temp <= 2050)
            {
              channel[ch_i] = temp;
            }

            ch_i++;

            if (ch_i == 14)
            {
              break;
            }
          }
          is_decoding = false;
          return true;
        }
        else
        {
          Serial.print(crc);
          Serial.print("  ");
          Serial.print(crc_calculated);
          Serial.println("No valid CRC");
          return 0;
        }
      }
    }

    // Controlling to spot a dead time.
    deadTimer = millis();
  }

  if ((millis() - deadTimer) > DEAD_TIME)
  {
    Serial.println(millis() - deadTimer);
    alive = false;            // Let the controller know that the connection is dead
    setDefaultChannelValue(); // failsafe
  }

  // if (this->deadlock)
  // {
  //   alive = false;            // Let the controller know that the connection is dead
  //   setDefaultChannelValue(); // failsafe
  //   return false;
  // }

  return alive;
}

uint8_t IBUS::isAlive()
{
  return alive;
}

uint32_t IBUS::getChannel(channel_source ch)
{
  if (ch > 14)
    return 0;
  return channel[ch];
}

uint8_t IBUS::setChannelDefaultValue(channel_source ch, uint16_t milliseconds)
{
  if (ch < FRAME_LENGTH - 2 && ch > 0 && milliseconds <= 2050 && milliseconds >= 950)
  {
    channel_defaults[ch] = milliseconds;
    return 1;
  }
  return 0;
}

void IBUS::setDefaultChannelValue()
{
  for (uint8_t i = 0; i < FRAME_LENGTH - 2; i++)
  {
    channel[i] = channel_defaults[i];
  }
}