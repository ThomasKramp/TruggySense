#ifndef I2C_H_
#define I2C_H_

#include "Arduino.h"
#include "Wire.h"

void i2c_write(uint8_t dev_addr, uint8_t reg_addr, uint8_t* buffer, size_t length);
void i2c_read(uint8_t dev_addr, uint8_t reg_addr, uint8_t* buffer, size_t length);

#endif
