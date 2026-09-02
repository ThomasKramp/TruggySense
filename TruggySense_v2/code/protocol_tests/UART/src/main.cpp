/**
 * Blink
 *
 * Turns on an LED on for one second,
 * then off for one second, repeatedly.
 * 
 * Source: https://docs.arduino.cc/learn/electronics/servo-motors/
 */

#include "Arduino.h"

#ifndef LED_BUILTIN
  #define LED_BUILTIN 13
#endif

bool read_serial = false;

void setup() {
    Serial.begin(115200);
    Serial2.begin(115200);

    // initialize LED digital pin as an output.
    pinMode(LED_BUILTIN, OUTPUT);

    digitalWrite(LED_BUILTIN, HIGH);

    delay(1000);

    //while (!Serial2) { }

    // Turn led off
    digitalWrite(LED_BUILTIN, LOW);
}

void loop() {
    if (read_serial){
        if (Serial2.available()) {
            char c = Serial2.read();
            Serial.write(c);
        }
    } else {
        Serial2.println("Teensy can send");
        delay(1000);
    }
}