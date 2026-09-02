/**
 * @file LM75.cpp
 * @author Thomas Kramp (creator)
 * @brief Implementation of LM75.h.
 * @version 1.0
 * @date 2026-05-25
 * 
 * @copyright Copyright (c) 2026 Thomas Kramp, All rights reserved.
 * This project is released under the CC BY-NC-SA licence.
 * See licence.txt for more information.
 */

#include "LM75.h"

// #define PRINTDEBUG

bool LM75::init() {
    // Config register
    uint8_t  config = 0x00;

    // Fault detection:
    config |= (0b10 << 3);
    // Defines the amount of faulty samples (> T_OS) needed to activate the interrupt.

    // O.S. Polarity:
    config |= (0b1 << 2);
    // Set the interrupt to active high.

    // Comparator/Interrupt mode:
    config &= ~(0b0 << 1);
    // Set the system to comparator mode.

    // Shutdown:
    config &= ~((uint8_t)_faults << 0);
    // Shutdown the sensor.

    // Config Register
    _write_buffer[0] = config;
    _i2c->write(_address, LM75_CONFIG_REG, _write_buffer, 1);

    // T_OS Register
    // Temperature (T_OS) at/exceeding which an interrupt will be sent out.
    _write_buffer[0] = (_max_temp >> 1) & 0xFF;
    _write_buffer[1] = (_max_temp << 7) & 0xFF;
    _i2c->write(_address, LM75_TOS_REG, _write_buffer, 2);

    // T_HYST Register
    // Temperature (T_HYST) at/below which an interrupt will be disabled again.
    _write_buffer[0] = (_fall_temp >> 1) & 0xFF;
    _write_buffer[1] = (_fall_temp << 7) & 0xFF;
    _i2c->write(_address, LM75_THYST_REG, _write_buffer, 2);

    return true;
}

float LM75::getTemperature() {
    _i2c->read(_address, LM75_TEMP_REG, _read_buffer, 2);
    int16_t raw_temp = (_read_buffer[0] << 8) | _read_buffer[1];
    float temp = (raw_temp >> 7) / 2.0f;

    #ifdef PRINTDEBUG
        debugSerial.print("\tADC: " + String(raw_temp));
        debugSerial.print("\tReal temperature: " + String(temp) + "°C");
        debugSerial.print("\tMaximum temperature: " + String(_max_temp) + "°C");
        debugSerial.println("\tFall-back temperature:: " + String(_fall_temp) + "°C");
    #endif
    return temp;
}