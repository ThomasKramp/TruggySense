/**
 * Blink
 *
 * Turns on an LED on for one second,
 * then off for one second, repeatedly.
 * 
 * Source: https://docs.arduino.cc/learn/electronics/servo-motors/
 */

#include "Arduino.h"
#include "PWMServo.h"

#ifndef LED_BUILTIN
  #define LED_BUILTIN 13
#endif

#ifndef LED_OPTO
  #define LED_OPTO 4
#endif

PWMServo motor;

void setup() {
    Serial.begin(115200);
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
    
    motor.attach(LED_OPTO);
    motor.write(93);        // neutral
    Serial.println("Waiting for ESC to arm...");
    delay(5000);           // generous arming window — ESC will arm well within this
    Serial.println("ESC should be armed. Applying throttle.");
    digitalWrite(LED_BUILTIN, LOW);
    
    motor.write(98);       // minimum throttle throttle in setup
    delay(500);
    motor.write(90);        // back to neutral



    digitalWrite(LED_BUILTIN, LOW);
    motor.write(180);       // throttle = go
    Serial.println("Go");
    delay(1000);
}

void loop() {
    digitalWrite(LED_BUILTIN, HIGH);
    motor.write(93);        // neutral = stop
    Serial.println("Stop");
    delay(5000);

    digitalWrite(LED_BUILTIN, LOW);
    motor.write(111);       // throttle = go
    Serial.println("Go");
    delay(2500);
}