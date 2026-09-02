/**
 * Analogue read
 *
 * Reads the analogue voltage off the battery cells.
 * These should be scaled via a voltage divider to a range of 0V - 3.3V.
 * 
 * Source: https://www.pjrc.com/teensy/tutorial4.html
 */

#include "Arduino.h"

#ifndef LED_BUILTIN
  #define LED_BUILTIN 13
#endif

#define CELL_1 14 //  3.2V -  4.2V
#define CELL_2 15 //  6.4V -  8.4V
#define CELL_3 16 //  9.6V - 12.6V
#define CELL_4 17 // 12.8V - 16.8V
#define RAIL_3V3 40
#define RAIL_5V0 41

void getVoltage(uint8_t source, float_t max_volt, String name);

void setup() {
    Serial.begin (115200);
    
    // Turn led on to indicate Ready state
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);

    // Wait for serial port to connect
    while (!Serial) { }

    // Set pin to read analogue value
    analogReadResolution(12);
    pinMode(CELL_1, INPUT);
    pinMode(CELL_2, INPUT);
    pinMode(CELL_3, INPUT);
    pinMode(CELL_4, INPUT);
    pinMode(RAIL_3V3, INPUT);
    pinMode(RAIL_5V0, INPUT);

    // Turn led off
    digitalWrite(LED_BUILTIN, LOW);
    // 2.95 --> 2.65 / 2.95
    // 0.00 --> 0.65 / 0.40
}

void loop() {
    getVoltage(CELL_1,   4.2, "B-Cell 1");
    getVoltage(CELL_2,   8.4, "B-Cell 2");
    getVoltage(CELL_3,  12.6, "B-Cell 3");
    getVoltage(CELL_4,  16.8, "B-Cell 4");
    getVoltage(RAIL_3V3, 3.3, "3.3V Rail");
    getVoltage(RAIL_5V0, 5.0, "5.0V Rail");
    delay(1000);
}

float_t voltage;
void getVoltage(uint8_t source, float_t max_volt, String name) {
    voltage = analogRead(source) * max_volt / 4096;
    Serial.print(name + ":\t");
    Serial.print(voltage);
    Serial.print(" V\t");
    Serial.print("Percentile:");
    Serial.print(voltage * 100 / max_volt);
    Serial.println(" %\n");
}