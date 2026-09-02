/*
 * FlySky Receiver
 *
 * This example shows ...
 * 
 * The circuit:
 * ** RX (IBUS) - 
 * 
 * Created  21 MAy 2020     by Ricardo Paiva
 * Modified 06 Mar 2026     by Thomas K.
 * 
 * This example code is in the public domain.
 * 
 * Source:  https://medium.com/@werneckpaiva/how-to-read-rc-signal-with-arduino-using-flysky-ibus-73448bc924eb
 * Video:
 *  - Receiver module:  https://www.youtube.com/watch?v=NuVQz7FCAZk
 *  - IBUS conversion:  https://www.youtube.com/watch?v=q80QijmQzOI
 */

#include "HardwareSerial.h"
#include "ibus.h"

IBUS FlySky;

void setup() {
  Serial.begin(115200);
  FlySky.init();
}

void loop() {
    FlySky.read();

    for (uint16_t i = 0; i < 14; i++){
        int value = FlySky.getChannel(i);
        Serial.print("Ch");
        Serial.print(i + 1);
        Serial.print(": \t");
        Serial.print(value);
        Serial.println();
    }

    FlySky.isFailsafe();
    Serial.println();
    delay(10);
}