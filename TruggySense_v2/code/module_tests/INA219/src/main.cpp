/**
 * INA219 test sketch
 *
 * Tests the connection of the INA219 module
 * by printing all the variables it senses.
 * 
 * Source: https://www.ti.com/lit/ds/symlink/ina219.pdf?ts=1772689222976&ref_url=https%253A%252F%252Fapp.ultralibrarian.com%252Fdetails%252Fdatasheet%252Fc9d4d1e5-34b7-11f0-b69d-024899f9dfe1%252FTexas-Instruments%252FINA219AIDR-Z
 */

#include "Arduino.h"
#include "INA219.h"

#ifndef LED_BUILTIN
  #define LED_BUILTIN 13
#endif

INA219 Rail_5V(INA219::ADDRESS_4, 100, 2, 5);

void setup() {
    Wire.begin();
    Serial.begin (115200);
    
    // Turn led on to indicate Ready state
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);

    // Wait for serial port to connect
    while (!Serial) { }

    Rail_5V.setConfig();

    // Turn led off
    digitalWrite(LED_BUILTIN, LOW);
}

uint16_t counter = 1;
void loop() {
    Serial.print(counter);
    Serial.print("\t");
    Serial.print("Receive:\t");
    Serial.print(Rail_5V.getCurrent());
    Serial.println(" A");
    counter += 1;
    delay(1000);
}