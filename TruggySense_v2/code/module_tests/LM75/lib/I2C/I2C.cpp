#include "I2C.h"

// Function to write a single byte to a register
void i2c_write(uint8_t dev_addr, uint8_t reg_addr, uint8_t* buffer, size_t length) {
    Wire.beginTransmission(dev_addr);      // Start communication with the device
    Wire.write(reg_addr);                     // Send the register address
    for (size_t i = 0; i < length; i++) {
        Wire.write(buffer[i] & 0xFF);
    }
    // Wire.write(settings >> 8);                // Write each byte in the data buffer
    // Wire.write(settings & 0xFF);              // Write each byte in the data buffer
    Wire.endTransmission();                   // End transmission and make the bus inactive
}

// Function to read multiple bytes starting from a register
void i2c_read(uint8_t dev_addr, uint8_t reg_addr, uint8_t* buffer, size_t length) {
    Wire.beginTransmission(dev_addr);
    Wire.write(reg_addr);
    Wire.endTransmission(false);
    Wire.requestFrom(dev_addr, length);

    for (size_t i = 0; i < length; i++) {
        if (Wire.available()) {
            buffer[i] = Wire.read();
        } else {
            buffer[i] = 0; // Fill with 0 if no data
        }
    }
}