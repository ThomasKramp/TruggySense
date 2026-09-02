#include "PIM448.h"

#define PRINTDEBUG 1

// Constructor
PIM448::PIM448(const ADDRESS address, GYRO_FS_SEL gfs_sel, ACCEL_FS_SEL afs_sel) {
    _address    = address;
    _gfs_sel    = gfs_sel;
    _afs_sel    = afs_sel;
}

void PIM448::setConfig() {
    setUsrBank(2);

    setGyroConfig();
    setAccelConfig();
    setTempConfig();

    setUsrBank(0);

    // Set USER controls:
    _i2c_write_buffer[0] = 0b00000000;
    i2c_write(_address, USER_CTRL_REG, _i2c_write_buffer, 0);

    // Power management:
    _i2c_write_buffer[0] = 0b00000000;
    _i2c_write_buffer[1] = 0b00000000;
    i2c_write(_address, PWR_MGMT_REG, _i2c_write_buffer, 2);
}

void PIM448::setUsrBank(uint8_t bank) {
    if (bank > 3) return;

    // Set USR BANK:
    _i2c_write_buffer[0] = (bank << 4) & 0xFF;
    i2c_write(_address, REG_BANK_SEL_REG, _i2c_write_buffer, 1);

    #ifdef PRINTDEBUG
        Serial.print("Current BANK:\t");
        i2c_read(_address, REG_BANK_SEL_REG, _i2c_read_buffer, 1);
        Serial.println((_i2c_read_buffer[0] >> 4));
    #endif
}

void PIM448::setGyroConfig() {
    // Gyro sample rate divider:
    _i2c_write_buffer[0] = 0b00000111;
    i2c_write(_address, GYRO_SMPLRT_DIV_REG, _i2c_write_buffer, 1);
    // Calcs:   f_s = 1.1kHz / (1 + reg)

    // Reset the full register:
    _config &= ~0x0000;

    // Gyro low-pass filter config:
    _config |= (0b10 << 11);
    
    // Gyro full scale select:
    _config |= (_gfs_sel << 9);

    // Enable gyro low-pass filter:
    _config |= (0b1 << 8);

    // Gyro self tests:
    _config |= (0b111 << 3);

    // Averaging filter (32 samples):
    _config |= (0b101 << 0);

    _i2c_write_buffer[0] = _config >> 8;
    _i2c_write_buffer[1] = _config & 0xFF;
    i2c_write(_address, GYRO_CONFIG_REG, _i2c_write_buffer, 2);
    
    #ifdef PRINTDEBUG
        Serial.print("Configuration:\t");
        i2c_read(_address, GYRO_CONFIG_REG, _i2c_read_buffer, 2);
        uint16_t config = (_i2c_read_buffer[0] << 8) | _i2c_read_buffer[1];
        Serial.println(config, BIN);
    #endif
}
void PIM448::setAccelConfig() {
    // Accel sample rate divider
    _i2c_write_buffer[0] = 0b0000;
    _i2c_write_buffer[1] = 0b00001111;
    i2c_write(_address, ACCEL_SMPLRT_DIV_REG, _i2c_write_buffer, 2);

    // Reset the full register:
    _config &= ~0x0000;

    // Accel low-pass filter config:
    _config |= (0b10 << 11);
    
    // Accel full scale select:
    _config |= (_afs_sel << 9);

    // Enable accel low-pass filter:
    _config |= (0b1 << 8);

    // Accel self tests:
    _config |= (0b111 << 3);

    // Averaging filter (32 samples):
    _config |= (0b101 << 0);

    _i2c_write_buffer[0] = _config >> 8;
    _i2c_write_buffer[1] = _config & 0xFF;
    i2c_write(_address, ACCEL_CONFIG_REG, _i2c_write_buffer, 2);

    #ifdef PRINTDEBUG
        Serial.print("Configuration:\t");
        i2c_read(_address, ACCEL_CONFIG_REG, _i2c_read_buffer, 2);
        uint16_t config = (_i2c_read_buffer[0] << 8) | _i2c_read_buffer[1];
        Serial.println(config, BIN);
    #endif
}
void PIM448::setTempConfig() {
    // Accel low-pass filter config:
    _config = 0b010;

    _i2c_write_buffer[0] = _config;
    i2c_write(_address, TEMP_CONFIG_REG, _i2c_write_buffer, 1);
}

// Get functions
bool PIM448::getConfirmation() {
    // https://product.tdk.com/system/files/dam/doc/product/sensor/mortion-inertial/imu/data_sheet/ds-000189-icm-20948-v1.5.pdf#page=36
    i2c_read(_address, WHO_AM_I_REG, _i2c_read_buffer, 1);
    return _i2c_read_buffer[0] == 0xEA;
}
float PIM448::getTemperature() {
    // https://product.tdk.com/system/files/dam/doc/product/sensor/mortion-inertial/imu/data_sheet/ds-000189-icm-20948-v1.5.pdf#page=45
    i2c_read(_address, TEMP_OUT_REG, _i2c_read_buffer, 2);
    int16_t raw_temp = (_i2c_read_buffer[0] << 8) | _i2c_read_buffer[1];
    // https://product.tdk.com/system/files/dam/doc/product/sensor/mortion-inertial/imu/data_sheet/ds-000189-icm-20948-v1.5.pdf#page=14
    return ((raw_temp - 0)/ 333.87) + 21;
}
float PIM448::getGyrXDPS(){
    // https://product.tdk.com/system/files/dam/doc/product/sensor/mortion-inertial/imu/data_sheet/ds-000189-icm-20948-v1.5.pdf#page=44
    i2c_read(_address, GYRO_X_OUT_REG, _i2c_read_buffer, 2);
    uint16_t raw_dps = (_i2c_read_buffer[0] << 8) | _i2c_read_buffer[1];
    return raw_dps / _gyro_sens[_gfs_sel] * (6.14 / 360);
}
float PIM448::getGyrYDPS(){
    // https://product.tdk.com/system/files/dam/doc/product/sensor/mortion-inertial/imu/data_sheet/ds-000189-icm-20948-v1.5.pdf#page=44
    i2c_read(_address, GYRO_Y_OUT_REG, _i2c_read_buffer, 2);
    uint16_t raw_dps = (_i2c_read_buffer[0] << 8) | _i2c_read_buffer[1];
    return raw_dps / _gyro_sens[_gfs_sel] * (6.14 / 360);
}
float PIM448::getGyrZDPS(){
    // https://product.tdk.com/system/files/dam/doc/product/sensor/mortion-inertial/imu/data_sheet/ds-000189-icm-20948-v1.5.pdf#page=45
    i2c_read(_address, GYRO_Z_OUT_REG, _i2c_read_buffer, 2);
    uint16_t raw_dps = (_i2c_read_buffer[0] << 8) | _i2c_read_buffer[1];
    return raw_dps / _gyro_sens[_gfs_sel] * (6.14 / 360);
}
float PIM448::getAccXMG(){
    // https://product.tdk.com/system/files/dam/doc/product/sensor/mortion-inertial/imu/data_sheet/ds-000189-icm-20948-v1.5.pdf#page=44
    i2c_read(_address, ACCEL_X_OUT_REG, _i2c_read_buffer, 2);
    int16_t raw_g = (_i2c_read_buffer[0] << 8) | _i2c_read_buffer[1];
    return raw_g / _accel_sens[_afs_sel] * 9.81;
}
float PIM448::getAccYMG(){
    // https://product.tdk.com/system/files/dam/doc/product/sensor/mortion-inertial/imu/data_sheet/ds-000189-icm-20948-v1.5.pdf#page=44
    i2c_read(_address, ACCEL_Y_OUT_REG, _i2c_read_buffer, 2);
    int16_t raw_g = (_i2c_read_buffer[0] << 8) | _i2c_read_buffer[1];
    return raw_g / _accel_sens[_afs_sel] * 9.81;
}
float PIM448::getAccZMG(){
    // https://product.tdk.com/system/files/dam/doc/product/sensor/mortion-inertial/imu/data_sheet/ds-000189-icm-20948-v1.5.pdf#page=45
    i2c_read(_address, ACCEL_Z_OUT_REG, _i2c_read_buffer, 2);
    int16_t raw_g = (_i2c_read_buffer[0] << 8) | _i2c_read_buffer[1];
    return raw_g / _accel_sens[_afs_sel] * 9.81;
}