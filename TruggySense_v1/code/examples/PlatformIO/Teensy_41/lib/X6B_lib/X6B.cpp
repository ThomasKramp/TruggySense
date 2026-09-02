#include "X6B.h"

HardwareSerial *s;

byte input;
bool pre_start_frame = false;
bool start_frame_detected = true;
bool alive = false;

const byte frame_length = (FRAME_LENGTH - 1) * 2;
uint8_t buffer[frame_length];
uint8_t buffer_index = 0;

uint16_t channel_defaults[] = {CH1_DEFAULT_VAL,
                              CH2_DEFAULT_VAL,
                              CH3_DEFAULT_VAL,
                              CH4_DEFAULT_VAL,
                              CH5_DEFAULT_VAL,
                              CH6_DEFAULT_VAL,
                              CH7_DEFAULT_VAL,
                              CH8_DEFAULT_VAL,
                              CH9_DEFAULT_VAL,
                              CH10_DEFAULT_VAL,
                              CH11_DEFAULT_VAL,
                              CH12_DEFAULT_VAL,
                              CH13_DEFAULT_VAL,
                              CH14_DEFAULT_VAL};

std::vector<uint16_t> channel(FRAME_LENGTH - 2); // Minus the checksum and header

long timer;
long deadTimer; // When no connection is present

void setDefaultChannelValue();

void initX6B(HardwareSerial &serial)
{
  s = &serial;
  s->begin(115200, SERIAL_8N1);

  // set default channel setting
  setDefaultChannelValue();
}

/**
--- return ---
0, no connection
1, connection and communication
2, requested to fast after one another. See: INTER_FRAME_STOP
**/
uint8_t readData()
{

  // Do not overrun the uart. No use as it only transmits every 7ms.
  if ((millis() - timer) < INTER_FRAME_STOP)
  {
    return 2;
  }
  timer = millis();

  while (s->available() > 0)
  {
    input = s->read(); // Read the buffer

    // Check if starting sequence pops up.
    if (input == (START_FRAME_SEQUENCE & 0xFF00) >> 8)
    {
      pre_start_frame = true;
    }

    // Check if starting sequence pops up.
    if (pre_start_frame && input == (START_FRAME_SEQUENCE & 0xFF))
    {
      pre_start_frame = false;
      start_frame_detected = true;
      alive = true; // The connection is alive
    }

    // If starting sequence has popped up, save each byte in teh buffer
    if (start_frame_detected && input != (START_FRAME_SEQUENCE & 0xFF))
    {
      buffer[buffer_index] = input;
      buffer_index++;

      if (buffer_index == frame_length)
      {
        start_frame_detected = false;
        buffer_index = 0;

        uint8_t ch_i = 0;
        for (uint8_t i = 0; i < frame_length; i += 2)
        {
          channel[ch_i] = buffer[i] | buffer[i + 1] << 8;

          if (channel[ch_i] <= 950 || channel[ch_i] >= 2050)
          {
            channel[ch_i] = channel_defaults[ch_i];
          }

          ch_i++;

          if (ch_i == 14)
            break;
        }
        return true;
      }
    }

    // Controlling to spot a dead time.
    deadTimer = millis();
  }

  if ((millis() - deadTimer) > DEAD_TIME)
  {
    alive = false;            // Let the controller know that the connection is dead
    setDefaultChannelValue(); // failsafe
    return false;
  }
  return alive;
}

uint8_t isAlive()
{
  return alive;
}

uint32_t getChannel(uint8_t index)
{
  if (index > 14)
    return 0;
  return channel[index];
}

/**
 * channel_number is 1-based
 * milliseconds goes from 950 to 2050
 */
uint8_t setChannelDefaultValue(uint8_t channel_number, uint16_t milliseconds)
{
  if (channel_number <= FRAME_LENGTH - 2 && channel_number > 0 && milliseconds <= 2050 && milliseconds >= 950)
  {
    channel_defaults[channel_number] = milliseconds;
    return 1;
  }
  return 0;
}

void setDefaultChannelValue()
{
  for (uint8_t i = 0; i < FRAME_LENGTH - 2; i++)
  {
    channel[i] = channel_defaults[i];
  }
}
