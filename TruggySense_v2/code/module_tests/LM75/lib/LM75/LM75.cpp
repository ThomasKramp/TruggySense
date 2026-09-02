#include "LM75.h"

// Constructor
LM75::LM75(const ADDRESS address, FAULT_QUEUE faults, uint8_t max_temp, uint8_t fall_temp) {
    _address    = address;
    _faults     = faults;
    _max_temp   = max_temp;
    _fall_temp  = fall_temp;
}

//#define PRINTDEBUG 1

void LM75::setConfig() {
    // Fault detection:
    _config |= (0b10 << 3);
    // Defines the amount of faulty samples (> T_OS) needed to activate the interrupt.

    // O.S. Polarity:
    _config |= (0b1 << 2);
    // Set the interrupt to active high.

    // Comparator/Interrupt mode:
    _config &= ~(0b0 << 1);
    // Set the system to comparator mode.

    // Shutdown:
    _config &= ~(_faults << 0);
    // Shutdown the sensor.

    // Config Register
    i2c_write_buffer[0] = _config;
    i2c_write(_address, CONFIG_REG, i2c_write_buffer, 1);

    // T_OS Register
    // Temperature (T_OS) at/exceeding which an interrupt will be sent out.
    i2c_write_buffer[0] = (_max_temp >> 1) & 0xFF;
    i2c_write_buffer[1] = (_max_temp << 7) & 0xFF;
    i2c_write(_address, TOS_REG, i2c_write_buffer, 2);

    // T_HYST Register
    // Temperature (T_HYST) at/below which an interrupt will be disabled again.
    i2c_write_buffer[0] = (_fall_temp >> 1) & 0xFF;
    i2c_write_buffer[1] = (_fall_temp << 7) & 0xFF;
    i2c_write(_address, THYST_REG, i2c_write_buffer, 2);

    #ifdef PRINTDEBUG
        Serial.print("Configuration:\t");
        Serial.println(_config, BIN);

        Serial.print("Maximum temperature:\t");
        Serial.print(_max_temp);
        Serial.println(" °C");
        
        Serial.print("Fall-back temperature:\t\t");
        Serial.print(_fall_temp);
        Serial.println(" °C");
    #endif
}

// Get functions
float LM75::getTemperature() {
    i2c_read(_address, TEMP_REG, i2c_read_buffer, 2);
    int16_t raw_temp = (i2c_read_buffer[0] << 8) | i2c_read_buffer[1];
    return (raw_temp >> 7) / 2.0;
}