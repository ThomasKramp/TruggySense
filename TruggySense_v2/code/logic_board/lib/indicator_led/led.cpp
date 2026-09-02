/**
 * @file led.cpp
 * @author Thomas Kramp (creator)
 * @brief Implementation of led.h
 * @version 1.0
 * @date 2026-05-25
 * 
 * @copyright Copyright (c) 2026 Thomas Kramp, All rights reserved.
 * This project is released under the CC BY-NC-SA licence.
 * See licence.txt for more information.
 */
#include "led.h"

// #define PRINTDEBUG

bool LED::init() {
    pinMode(_pin, OUTPUT);
    return true;
}

void LED::write(const bool value) {
    digitalWrite(_pin, value);

    #ifdef PRINTDEBUG
        debugSerial.println("\tRaw value: " + String(value));
    #endif
}
void LED::toggle() {
    _ledState = (_ledState == HIGH) ? LOW : HIGH;
    write(_ledState);
}