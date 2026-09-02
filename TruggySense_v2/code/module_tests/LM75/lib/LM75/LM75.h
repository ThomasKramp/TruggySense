#ifndef H_
#define H_

#include "Arduino.h"
#include "I2C.h"

class LM75 {
    public:
        // Enums
        enum ADDRESS {
            ADDRESS_1 = 0x48,
            ADDRESS_2 = 0x49,
            ADDRESS_3 = 0x4A,
            ADDRESS_4 = 0x4B,
            ADDRESS_5 = 0x4C,
            ADDRESS_6 = 0x4D,
            ADDRESS_7 = 0x4E,
            ADDRESS_8 = 0x4F
        };
        enum FAULT_QUEUE {
            FUALTS_1, 
            FUALTS_2, 
            FUALTS_4, 
            FUALTS_6
        };

        // Constructor
        explicit LM75(const ADDRESS address, FAULT_QUEUE faults, uint8_t max_temp, uint8_t fall_temp);

        // Public methods
        void     setConfig();

        float    getTemperature();

    private:
        // Registers
        #define TEMP_REG   0x00    // R
        #define CONFIG_REG 0x01    // R/W
        #define TOS_REG    0x02    // R/W
        #define THYST_REG  0x03    // R/W

        // Private vars
        ADDRESS  _address;
        FAULT_QUEUE _faults;

        uint8_t  _config = 0x00;
        uint8_t  _max_temp;
        uint8_t  _fall_temp;
        float    _temperature;

        // I2C buffers
        uint8_t i2c_write_buffer[2];
        uint8_t i2c_read_buffer[2];
};

#endif