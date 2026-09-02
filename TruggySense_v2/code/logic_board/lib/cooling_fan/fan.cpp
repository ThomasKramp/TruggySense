/**
 * @file fan.cpp
 * @author Thomas Kramp (creator)
 * @brief Implementation of fan.h
 * @version 1.0
 * @date 2026-05-25
 * 
 * @copyright Copyright (c) 2026 Thomas Kramp, All rights reserved.
 * This project is released under the CC BY-NC-SA licence.
 * See licence.txt for more information.
 */
#include "fan.h"

// #define PRINTDEBUG

bool Fan::init() {
    pinMode(_pin, OUTPUT);
    return true;
}

void Fan::write(const uint16_t value) {
    // Min value = 1000 --> 0
    // Max value = 2000 --> 255
    uint8_t pwm = (value - 1000) / 4;
    analogWrite(_pin, pwm);

    #ifdef PRINTDEBUG
        interrupts_enable();
        debugSerial.println("\tRaw value: " + String(value));
        debugSerial.println("\tMapped value: " + String(pwm));
        interrupts_disable();
    #endif
}

void Fan::write(const float temperature) {
    // Min value = 25 --> 0
    // Max value = 50 --> 255
    uint8_t pwm = (temperature > 50) ? 255 : (temperature - 25) * 10;
    analogWrite(_pin, pwm);

    #ifdef PRINTDEBUG
        interrupts_enable();
        debugSerial.println("\tTemperature: " + String(temperature));
        debugSerial.println("\tPWM signal: " + String(pwm));
        interrupts_disable();
    #endif
}