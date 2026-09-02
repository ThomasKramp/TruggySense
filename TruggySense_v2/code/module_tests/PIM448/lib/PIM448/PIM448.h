#ifndef PIM448_H_
#define PIM448_H_

#include "Arduino.h"
#include "I2C.h"

class PIM448 {
    public:
        // Enums
        enum ADDRESS {
            ADDRESS_1 = 0x68,
            ADDRESS_2 = 0x69
        };
        enum GYRO_FS_SEL {
            DPS_250, 
            DPS_500, 
            DPS_1000, 
            DPS_2000
        };
        enum ACCEL_FS_SEL {
            G_2, 
            G_4, 
            G_8, 
            G_16
        };

        // Constructor
        explicit PIM448(const ADDRESS address, GYRO_FS_SEL gfs_sel, ACCEL_FS_SEL afs_sel);

        // Public methods
        void     setConfig();

        bool     getConfirmation();

        float    getTemperature();

        float    getGyrXDPS();
        float    getGyrYDPS();
        float    getGyrZDPS();

        float    getAccXMG();
        float    getAccYMG();
        float    getAccZMG();

    private:
        // Registers
        #define REG_BANK_SEL_REG     0x7F // R/W

            // Bank 0
        #define WHO_AM_I_REG         0x00 // R
        #define USER_CTRL_REG        0x03 // R/W
        #define PWR_MGMT_REG         0x06 // R/W

        #define ACCEL_X_OUT_REG      0x2D // R
        #define ACCEL_Y_OUT_REG      0x2F // R
        #define ACCEL_Z_OUT_REG      0x31 // R

        #define GYRO_X_OUT_REG       0x33 // R
        #define GYRO_Y_OUT_REG       0x35 // R
        #define GYRO_Z_OUT_REG       0x37 // R

        #define TEMP_OUT_REG         0x39 // R

            // Bank 2
        #define GYRO_SMPLRT_DIV_REG  0x00 // R/W
        #define GYRO_CONFIG_REG      0x01 // R/W
        #define ACCEL_SMPLRT_DIV_REG 0x10 // R/W
        #define ACCEL_CONFIG_REG     0x14 // R/W
        #define TEMP_CONFIG_REG      0x53 // R/W

        // Private vars
        ADDRESS         _address;
        GYRO_FS_SEL     _gfs_sel;
        ACCEL_FS_SEL    _afs_sel;
        
        uint16_t _config = 0x00;
        float    _temperature;

        // Enum linked arrays
        // https://product.tdk.com/system/files/dam/doc/product/sensor/mortion-inertial/imu/data_sheet/ds-000189-icm-20948-v1.5.pdf#page=11
        float    _gyro_sens[4] = {
            131.0,
            65.5,
            32.8,
            16.4
        };
        // https://product.tdk.com/system/files/dam/doc/product/sensor/mortion-inertial/imu/data_sheet/ds-000189-icm-20948-v1.5.pdf#page=12
        float    _accel_sens[4] = {
            16384.0,
            8192.0,
            4096.0,
            2048.0
        };

        // I2C buffers
        uint8_t  _i2c_write_buffer[2];
        uint8_t  _i2c_read_buffer[2];

        // Private methods
        void     setUsrBank(uint8_t bank);
        void     setGyroConfig();
        void     setAccelConfig();
        void     setTempConfig();
};

#endif