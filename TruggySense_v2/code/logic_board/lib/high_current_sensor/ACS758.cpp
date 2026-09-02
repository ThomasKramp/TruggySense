/**
 * @file ACS758.cpp
 * @author Robbe Elsermans (creator)
 * @author Thomas Kramp (revisor)
 * @brief Implementation of ACS758.h
 * @version 2.0
 * @date 2026-05-25
 * 
 * @copyright Copyright (c) 2025 Robbe Elsermans, All rights reserved.
 * This project is released under the CC BY-NC-SA licence.
 * See licence.txt for more information.
 */#include "ACS758.h"

// #define PRINTDEBUG

bool ACS758::init() {
    pinMode(_pin, INPUT);
    analogReadResolution(adcResolution);
    return true;
}

float ACS758::getCurrent() {
    float raw_voltage = 0, avr_current = 0;
    int16_t raw_adc = 0;

    for (uint8_t i = 0; i < sampleSize; i++) {
        raw_adc = analogRead(_pin);
        raw_voltage = (raw_adc * _supply_voltage) / (powf(2, adcResolution) - 1);
        if (_bidirectional) {
            avr_current += (raw_voltage - (_supply_voltage / 2.0f)) / _sensitivity;
        } else {
            avr_current += (raw_voltage - 0.6f) / _sensitivity;
        }
    }

    avr_current /= sampleSize;

    #ifdef PRINTDEBUG
        debugSerial.print("\tADC: " + String(raw_adc >= 0 ? " " : "") + String(raw_adc));
        debugSerial.print("\tVoltage: " + String(raw_voltage >= 0 ? " " : "") + String(raw_voltage) + "V");
        debugSerial.print("\tCurrent: " + String(avr_current >= 0 ? " " : "") + String(avr_current) + "A");
        debugSerial.print("\tReference: " + String(_supply_voltage) + "V");
        debugSerial.print("\tPIN: " + String(_pin) + "\t");
        debugSerial.println("\tSample size: " + String(sampleSize));
    #endif

    return avr_current;
}