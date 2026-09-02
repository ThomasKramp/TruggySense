#include <Arduino.h>
#include "ibus.h"


// #define PACKET_LENGTH 32
// #define DATA_LENGTH (28 + 2) // 14 channels of 2 byte + 2 start byte
// #define START_FRAME_0 0x20
// #define START_FRAME_1 0x40

// enum decoder_states
// {
//   DETECT,
//   DATA,
//   VALIDATE,
//   DECODE,
//   DISCARD
// };

// decoder_states state = DETECT; // Start to detect data

// uint16_t crc = 0;
// uint8_t data_counter = 0;
// uint8_t raw_data[DATA_LENGTH] = {0};
// uint16_t data[DATA_LENGTH/2 - 1] = {0};

// uint16_t crc_calculated = 0xFFFF;

// long timer = 0;

long timer;

IBUS ibus;
void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  // Serial8.begin(115200);
  ibus.init();
  while (Serial.available())
  {
    delay(200);
  }
  timer = millis();
}

void loop()
{
    delay(8);
    ibus.read();

    if(millis()-timer > 200)
    {
      // if(!ibus.isFailsafe())
      // {
        for (uint8_t i = 1; i <= 14; i++)
        {
          Serial.printf("%d: %d ", i, ibus.getChannel((IBUS::channel_source)i));
        }
        Serial.println();
      // }
      // else{
      //   Serial.println("failsafe");
      // }
    }

    // while (Serial8.available())
    // {
    //   uint8_t received_data = Serial8.read();
    //   // Serial.print("det ");
    //   // Serial.printf("dc:  %d  0x%x ",data_counter, received_data);
    //   // Serial.print(" det");
      
    //   switch (state)
    //   {
    //   case DETECT:
        
    //     if (received_data == START_FRAME_0)
    //     {
    //       crc_calculated -= received_data;
    //       data_counter++;
    //     }
    //     else if (data_counter == 1 && received_data == START_FRAME_1)
    //     {
    //       crc_calculated -= received_data;
    //       data_counter++;
    //       state = DATA;
    //     }
    //     else
    //     {
    //       data_counter = 0;
    //       crc_calculated = 0xFFFF;
    //       state = DETECT;
    //     }
    //     break;
    //   case DATA:
    //     raw_data[data_counter] = received_data;
    //     crc_calculated -= raw_data[data_counter];
    //     data_counter++;

    //     if (data_counter == DATA_LENGTH)
    //     {
    //       state = VALIDATE;
    //     }
    //     break;
    //   case VALIDATE:
    //     if (data_counter == DATA_LENGTH)
    //     {
    //       crc = received_data;
    //       data_counter++;
    //     }
    //     else if (data_counter == DATA_LENGTH + 1)
    //     {
    //       crc |= (received_data << 8);

    //       if (crc == crc_calculated)
    //         state = DECODE;
    //       else
    //         state = DISCARD;
    //     }
    //     break;

    //   case DECODE:
    //     for (int i = 2; i < DATA_LENGTH - 1; i += 2)
    //     {
    //       data[(i / 2)-1] = raw_data[i] | raw_data[i + 1] << 8;
    //       Serial.printf("%d: %d ", i / 2, raw_data[i] | raw_data[i + 1] << 8);
    //     }
    //     Serial.println();

    //     state = DISCARD;
    //     break;
    //   case DISCARD:
    //     data_counter = 0;
    //     crc_calculated = 0xFFFF;
    //     state = DETECT;
    //     // flush buffer

    //     break;
    //   }
    // }
}
