/**
 * Blink
 *
 * Turns on an LED on for one second,
 * then off for one second, repeatedly.
 * 
 * Source: https://docs.arduino.cc/learn/electronics/servo-motors/
 */

#include "Arduino.h"
#include <Servo.h>

#ifndef LED_BUILTIN
  #define LED_BUILTIN 13
#endif

#ifndef LED_EXTERNAL
  #define LED_EXTERNAL 32
#endif

#ifndef LED_OPTO
  #define LED_OPTO 2
#endif

Servo servo;

void setup() {
    // initialize LED digital pin as an output.
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(LED_EXTERNAL, OUTPUT);
    servo.attach(LED_OPTO);

    digitalWrite(LED_BUILTIN, HIGH);

    //Serial.begin (115200);

    analogWriteResolution(12);  // analogWrite value 0 to 4095, or 4096 for high
    // Turn led off
    digitalWrite(LED_BUILTIN, LOW);
}

void loop() {
    // turn the LED on (HIGH is the voltage level)
    digitalWrite(LED_BUILTIN, HIGH);
    digitalWrite(LED_EXTERNAL, HIGH);
    servo.write(-4000);
    // wait for a second
    delay(1000);

    // turn the LED off by making the voltage LOW
    digitalWrite(LED_BUILTIN, LOW);
    digitalWrite(LED_EXTERNAL, LOW);
    servo.write(4000);

    // wait for a second
    delay(1000);
}