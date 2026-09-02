/**
 * LM75 test sketch
 *
 * Tests the connection of the LM75 module
 * by printing all the variables it senses.
 * 
 * Source: https://www.ti.com/lit/ds/symlink/lm75b.pdf?ts=1774774475976&ref_url=https%253A%252F%252Fwww.google.com%252F
 */

#include "Arduino.h"
#include "LM75.h"

#ifndef LED_BUILTIN
  #define LED_BUILTIN 13
#endif

LM75 Temp_Jetson(LM75::ADDRESS_1, LM75::FUALTS_4, 45, 35);

void setup() {
    Wire.begin();
    Serial.begin(115200);
    
    // Turn led on to indicate Ready state
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);

    // Wait for serial port to connect
    while (!Serial) { }

    // Config Register
    Temp_Jetson.setConfig();

    // Turn led off
    digitalWrite(LED_BUILTIN, LOW);
}

uint16_t counter = 1;
void loop() {
    Serial.print(counter);
    Serial.print("\t");
    Serial.print("Receive:\t");
    Serial.print(Temp_Jetson.getTemperature());
    Serial.println(" °C");
    counter += 1;
    delay(1000);
}