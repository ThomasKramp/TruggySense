#ifndef H_
#define H_

#include "Arduino.h"
#include "I2C.h"

class INA219 {
    public:
        // Enums
        enum ADDRESS {
            ADDRESS_1 = 0x40,
            ADDRESS_2 = 0x41,
            ADDRESS_3 = 0x44,
            ADDRESS_4 = 0x45
        };

        // Constructor
        explicit INA219(const ADDRESS address, const uint8_t shunt_resistor, const float max_current, const float rail_voltage);

        // Public methods
        void     setConfig();

        float    getCurrent();

    private:
        // Registers
        #define CONFIG_REG       0x00    // R/W
        #define SHUNT_VOLT_REG   0x01    // R
        #define BUS_VOLT_REG     0x02    // R
        #define POWER_REG        0x03    // R
        #define CURRENT_REG      0x04    // R
        #define CALIBRATE_REG    0x05    // R/W

        // Private vars
        ADDRESS  _address;

        uint8_t  _shunt;
        uint16_t _config = 0x0000;
        float    _max_current;
        float    _pga_range;
        float    _current_LSB;
        float    _rail_voltage;

        // I2C buffers
        uint8_t i2c_write_buffer[2];
        uint8_t i2c_read_buffer[2];
};

#endif