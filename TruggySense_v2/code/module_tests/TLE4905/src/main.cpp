/**
 * Analogue read
 *
 * Reads the analogue voltage off the battery cells.
 * These should be scaled via a voltage divider to a range of 0V - 3.3V.
 * 
 * Source: https://www.pjrc.com/teensy/tutorial4.html
 */

#include "Arduino.h"
#include "Encoder.h"

#ifndef LED_BUILTIN
  #define LED_BUILTIN 13
#endif

Encoder wheel_encoder;

void setup() {
    Serial.begin (115200);
    
    // Turn led on to indicate Ready state
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);

    // Wait for serial port to connect
    while (!Serial) { }

    // Initialize Wheel Encoder
    wheel_encoder = Encoder();

    // Turn led off
    digitalWrite(LED_BUILTIN, LOW);
}

float_t voltage;
void loop() {
    Serial.print("Front-Right wheel speed:");
    Serial.print(wheel_encoder.getWheelSpeed(FR));
    Serial.print(" rpm\n");
    Serial.print("Front-Left wheel speed:");
    Serial.print(wheel_encoder.getWheelSpeed(FL));
    Serial.print(" rpm\n");
    Serial.print("Rear-Right wheel speed:");
    Serial.print(wheel_encoder.getWheelSpeed(RR));
    Serial.print(" rpm\n");
    Serial.print("Rear-Left wheel speed:");
    Serial.print(wheel_encoder.getWheelSpeed(RL));
    Serial.print(" rpm\n");
    Serial.println("");
    delay(1000);
}