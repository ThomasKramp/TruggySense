/**
 * Analogue read
 *
 * Reads the analogue voltage off the battery cells.
 * These should be scaled via a voltage divider to a range of 0V - 3.3V.
 * 
 * Source: https://www.homemade-circuits.com/50a-high-current-sensor-ic-acs758-circuit-diagram/
 */

#include "Arduino.h"
#include "ACS758.h"

#ifndef LED_BUILTIN
  #define LED_BUILTIN 13
#endif

ACS758* high_current_reader;

void setup() {
    Serial.begin (115200);
    
    // Turn led on to indicate Ready state
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);

    // Wait for serial port to connect
    while (!Serial) { }

    // Initialize high current sensors
    high_current_reader = new ACS758(3.3);

    // Turn led off
    digitalWrite(LED_BUILTIN, LOW);
}

float_t voltage;
void loop() {
    Serial.print(analogRead(22));
    Serial.print("\tHigh current sensor:\t");
    Serial.print(high_current_reader->getCurrent(FR));
    Serial.println(" A");
    delay(1000);
}