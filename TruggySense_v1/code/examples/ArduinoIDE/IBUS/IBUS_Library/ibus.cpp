#include "ibus.h"


HardwareSerial *s;

byte input;
bool pre_start_frame = false;
bool start_frame_detected = true;
bool alive = false;

const byte frame_length = (FRAME_LENGTH - 1) * 2;
uint16_t buffer[frame_length];
uint8_t buffer_index = 0;

uint32_t channel[FRAME_LENGTH - 2];  //Minus the checksum and header

long timer;
long deadTimer;  //When no connection is present

void setDefaultChannelValue();

void initIBUS(HardwareSerial &serial) {
  s = &serial;
  s->begin(115200, SERIAL_8N1);

  //set default channel setting
  setDefaultChannelValue();
}

/**
--- return ---
0, no connection
1, connection and communication
2, requested to fast after one another. See: INTER_FRAME_STOP
**/
uint8_t readData() {

  //Do not overrun the uart. No use as it only transmits every 7ms.
  if ((millis() - timer) < INTER_FRAME_STOP) {
    return 2;
  }
  timer = millis();

  while (s->available() > 0) {
    input = s->read();  //Read the buffer

    //Check if starting sequence pops up.
    if (input == (START_FRAME_SEQUENCE & 0xFF00) >> 8) {
      pre_start_frame = true;
    }

    //Check if starting sequence pops up.
    if (pre_start_frame && input == (START_FRAME_SEQUENCE & 0xFF)) {
      pre_start_frame = false;
      start_frame_detected = true;
      alive = true;  //The connection is alive
    }

    //If starting sequence has popped up, save each byte in teh buffer
    if (start_frame_detected && input != (START_FRAME_SEQUENCE & 0xFF)) {
      buffer[buffer_index] = input;
      buffer_index++;

      if (buffer_index == frame_length) {
        start_frame_detected = false;
        buffer_index = 0;

        uint8_t ch_i = 0;
        for (uint8_t i = 0; i < frame_length; i += 2) {
          channel[ch_i] = buffer[i] | buffer[i + 1] << 8;
          ch_i++;

          if (ch_i == 14)
            break;
        }
        return true;
      }
    }

    //Controlling to spot a dead time.
    deadTimer = millis();
  }

  if ((millis() - deadTimer) > DEAD_TIME) {
    alive = false;  //Let the controller know that the connection is dead
    setDefaultChannelValue(); //failsafe
    return false;
  }
  return alive;
}

uint8_t isAlive() {
  return alive;
}

uint32_t getChannel(uint8_t index) {
  if (index > 14)
    return 0;
  return channel[index];
}

void setDefaultChannelValue() {
  channel[0] = CH1_DEFAULT_VAL;
  channel[1] = CH2_DEFAULT_VAL;
  channel[2] = CH3_DEFAULT_VAL;
  channel[3] = CH4_DEFAULT_VAL;
  channel[4] = CH5_DEFAULT_VAL;
  channel[5] = CH6_DEFAULT_VAL;
  channel[6] = CH7_DEFAULT_VAL;
  channel[7] = CH8_DEFAULT_VAL;
  channel[8] = CH9_DEFAULT_VAL;
  channel[9] = CH10_DEFAULT_VAL;
  channel[10] = CH11_DEFAULT_VAL;
  channel[11] = CH12_DEFAULT_VAL;
  channel[12] = CH13_DEFAULT_VAL;
  channel[13] = CH14_DEFAULT_VAL;
}
