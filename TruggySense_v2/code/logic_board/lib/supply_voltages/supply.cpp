/**
 * @file supply.cpp
 * @author Robbe Elsermans (creator)
 * @author Thomas Kramp (revisor)
 * @brief Implementation of supply.h
 * @version 2.0
 * @date 2026-05-25
 * 
 * @copyright Copyright (c) 2025 Robbe Elsermans, All rights reserved.
 * This project is released under the CC BY-NC-SA licence.
 * See licence.txt for more information.
 */

#include "supply.h"

// #define PRINTDEBUG

bool Supply::init() {
    pinMode(_pin, INPUT);
    analogReadResolution(adcResolution);
    
    return true;
}

float Supply::getVoltage() {
    float raw_data = 0, raw_voltage = 0, voltage = 0;
    for (uint8_t i = 0; i < sampleSize; i++) {
        raw_data = analogRead(_pin);
        raw_voltage += raw_data;
    }

    raw_voltage /= sampleSize;
    voltage = raw_voltage * _max_volt / (powf(2, adcResolution) - 1);

    #ifdef PRINTDEBUG
        debugSerial.print("\tADC: " + String(analogRead(_pin)));
        debugSerial.println("\tVoltage: " + String(voltage) + "V");
    #endif
    return voltage;
}