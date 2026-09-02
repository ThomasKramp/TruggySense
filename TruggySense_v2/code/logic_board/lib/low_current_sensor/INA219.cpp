/**
 * @file INA219.cpp
 * @author Thomas Kramp (creator)
 * @brief Implementation of INA219.h
 * @version 1.0
 * @date 2026-05-25
 * 
 * @copyright Copyright (c) 2026 Thomas Kramp, All rights reserved.
 * This project is released under the CC BY-NC-SA licence.
 * See licence.txt for more information.
 */
#include "INA219.h"

// #define PRINTDEBUG

bool INA219::init() {
    uint16_t calibration = (uint16_t)(0.04096f / (_current_LSB * (_shunt_resistor / 1000.0)));

    // Config register
    uint16_t config = 0x0000;

    // Reset:
    config &= ~(0b1 << 15);
    // Setting this bit to 1 will reset the device to default settings.

    // Bus voltage range & PGA:
    float pga_range = _shunt_resistor * _max_current;
    // V = R * I

    if (pga_range >= 160) {            // Range 160mV - 320mV
        config |= (0b111 << 11);
    } else if (pga_range >= 80) {      // Range  80mV - 160mV
        config |= (0b110 << 11);
    } else if (pga_range >= 40) {      // Range  40mV -  80mV
        config |= (0b101 << 11);
    } else if (pga_range >= 20) {      // Range  20mV -  40mV
        config |= (0b100 << 11);
    } else {                            // Range  10mV -  20mV
        config |= (0b000 << 11);
    }

    if (_rail_voltage > 16)  config |= (0b1 << 13);
    // Setting the BRNG bit to 1 will set the MAX BUS voltage to 32V.
    // Since the INA219 modules will only be used for 3V3 and 5V applications,
    // this value can be set to 16V, for more accurate measurements (if the current allows it).

    // ADC resolution:
    config |= (0b00110011 << 3);
    // This will be kept at the default value 

    // Operating mode:
    config |= (0b111 << 0);
    // Power draw is not a concern, thus we can measure continuously.

    _write_buffer[0] = (config >> 8) & 0xFF;
    _write_buffer[1] = config & 0xFF;
    _i2c->write(_address, INA219_CONFIG_REG, _write_buffer, 2);
    
    // Calibration Register
    _write_buffer[0] = (calibration >> 8) & 0xFF;
    _write_buffer[1] = calibration & 0xFF;
    _i2c->write(_address, INA219_CALIBRATE_REG, _write_buffer, 2);
    return true;
}

// Get functions
float INA219::getCurrent() {
    _i2c->read(_address, INA219_CURRENT_REG, _read_buffer, 2);
    int16_t raw_current = (_read_buffer[0] << 8) | _read_buffer[1];
    float current = raw_current * _current_LSB;

    #ifdef PRINTDEBUG
        debugSerial.print("\tADC: " + String(raw_current >= 0 ? " " : "") + String(raw_current));
        debugSerial.print("\tCurrent: " + String(current >= 0 ? " " : "") + String(current) + "A\t");
        debugSerial.print("\tpga_range: " + String(_shunt_resistor * _max_current) + "mV");
        debugSerial.println("\tShunt: " + String(_shunt_resistor) + "mΩ");
    #endif
    return current;
}