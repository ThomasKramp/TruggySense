#include "ibus.h"

uint8_t IBUS::init()
{
    Serial8.begin(115200);

    state = DETECT; // Start to detect data

    crc = 0;
    data_counter = 0;

    crc_calculated = 0xFFFF;

    failsafe = true;
    dead_timer = 0;
    setDefaultChannelValue();
    return 1; // success
}
uint8_t IBUS::read()
{
    if ((millis() - read_timer) < INTER_FRAME_STOP)
    {
      Serial.println("To fast");
      return 2;
    }
    this->read_timer = millis();

    while (Serial8.available())
    {
        uint8_t received_data = Serial8.read();

        switch (state)
        {
        case DETECT:
            if (received_data == START_FRAME_0)
            {
                crc_calculated -= received_data;
                data_counter++;
            }
            else if (data_counter == 1 && received_data == START_FRAME_1)
            {
                crc_calculated -= received_data;
                data_counter++;
                state = DATA;
            }
            else
            {
                data_counter = 0;
                crc_calculated = 0xFFFF;
                state = DETECT;
            }
            break;
        case DATA:
            raw_data[data_counter] = received_data;
            crc_calculated -= raw_data[data_counter];
            data_counter++;

            if (data_counter == DATA_LENGTH)
            {
                state = VALIDATE;
            }
            break;
        case VALIDATE:
            if (data_counter == DATA_LENGTH)
            {
                crc = received_data;
                data_counter++;
            }
            else if (data_counter == DATA_LENGTH + 1)
            {
                crc |= (received_data << 8);

                if (crc == crc_calculated)
                    state = DECODE;
                else
                    state = DISCARD;
            }
            break;

        case DECODE:

            for (int i = 2; i < DATA_LENGTH - 1; i += 2)
            {
                data[(i / 2) - 1] = raw_data[i] | raw_data[i + 1] << 8;
                // Serial.printf("%d: %d ", i / 2, raw_data[i] | raw_data[i + 1] << 8);
            }
            // Serial.println();
            dead_timer = millis();
            state = DISCARD;
            break;
        case DISCARD:
            data_counter = 0;
            crc_calculated = 0xFFFF;
            state = DETECT;
            // flush buffer

            break;
        }
    }

    if (millis() - dead_timer > DEAD_TIME)
    {
        failsafe = true;
        setDefaultChannelValue();
    }
    else
    {
        failsafe = false;
    }

    return 1;
}
uint16_t IBUS::getChannel(channel_source channel_number)
{
    if (channel_number >= 1 && channel_number <= CHANNEL_COUNT)
    {
        return data[channel_number - 1];
    }
    return 0;
}

uint8_t IBUS::isFailsafe()
{
    return failsafe;
}

void IBUS::setDefaultChannelValue()
{
  for (uint8_t i = 0; i < CHANNEL_COUNT; i++)
  {
    data[i] = channel_defaults[i];
  }
}