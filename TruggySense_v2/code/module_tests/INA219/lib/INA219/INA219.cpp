#include "INA219.h"

// Constructor
INA219::INA219(const ADDRESS address, const uint8_t shunt, const float max_current, const float rail_voltage) {
    _address        = address;
    _shunt          = shunt;
    _max_current    = max_current;
    _current_LSB    = max_current / 32768.0;
    _rail_voltage   = rail_voltage;
}

//#define PRINTDEBUG 1

void INA219::setConfig() {
    // current_LSB = max_current / 32768
    uint16_t calibration = (uint16_t)(0.04096 / (_current_LSB * (_shunt / 1000.0)));

    // Config register

    // Reset:
    _config &= ~(0b1 << 15);
    // Setting this bit to 1 will reset the device to default settings.

    // Bus voltage range & PGA:
    _pga_range = _shunt * _max_current;
    // V = R * I

    if (_pga_range >= 160) {            // Range 160mV - 320mV
        _config |= (0b111 << 11);
    } else if (_pga_range >= 80) {      // Range  80mV - 160mV
        _config |= (0b110 << 11);
    } else if (_pga_range >= 40) {      // Range  40mV -  80mV
        _config |= (0b101 << 11);
    } else if (_pga_range >= 20) {      // Range  20mV -  40mV
        _config |= (0b100 << 11);
    } else {                            // Range  10mV -  20mV
        _config |= (0b000 << 11);
    }

    if (_rail_voltage > 16)  _config |= (0b1 << 13);
    // Setting the BRNG bit to 1 will set the MAX BUS voltage to 32V.
    // Since the INA219 modules will only be used for 3V3 and 5V applications,
    // this value can be set to 16V, for more accurate measurements (if the current allows it).

    // ADC resolution:
    _config |= (0b00110011 << 3);
    // This will be kept at the default value 

    // Operating mode:
    _config |= (0b111 << 0);
    // Power draw is not a concern, thus we can measure continuously.

    i2c_write_buffer[0] = (_config >> 8) & 0xFF;
    i2c_write_buffer[1] = _config & 0xFF;
    i2c_write(_address, CONFIG_REG, i2c_write_buffer, 2);
    
    // Calibration Register
    i2c_write_buffer[0] = (calibration >> 8) & 0xFF;
    i2c_write_buffer[1] = calibration & 0xFF;
    i2c_write(_address, CALIBRATE_REG, i2c_write_buffer, 2);

    #ifdef PRINTDEBUG
        Serial.print("pga_range:\t");
        Serial.print(_pga_range);
        Serial.println(" mV");

        Serial.print("Configuration:\t");
        Serial.println(_config, BIN);
        
        Serial.print("Shunt:\t\t");
        Serial.print(_shunt_resistor);
        Serial.println(" mΩ");

        Serial.print("Calibration:\t");
        Serial.println(calibration);
    #endif
}

// Get functions
float INA219::getCurrent() {
    i2c_read(_address, CURRENT_REG, i2c_read_buffer, 2);
    int16_t raw_current = (i2c_read_buffer[0] << 8) | i2c_read_buffer[1];
    return raw_current * _current_LSB;
}