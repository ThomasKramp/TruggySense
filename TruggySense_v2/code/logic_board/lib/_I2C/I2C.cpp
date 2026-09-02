/**
 * @file I2C.cpp
 * @author Thomas Kramp (creator)
 * @brief Implementation of I2C.h
 * @version 1.0
 * @date 2026-05-25
 * 
 * @copyright Copyright (c) 2026 Thomas Kramp, All rights reserved.
 * This project is released under the CC BY-NC-SA licence.
 * See licence.txt for more information.
 */
#include "I2C.h"

#define PRINTDEBUG 1

I2C::I2C() {
    // will only call if it hasn't been called yet
    #ifndef WIRE_BEGIN_CALLED
        #define WIRE_BEGIN_CALLED
        Wire.begin();
        
        #ifdef PRINTDEBUG
            debugSerial.println("\tI2C initiated");
            I2C::scan();
        #endif
    #endif
}

void I2C::scan() {
    // Start scanning
    debugSerial.println("I2C scanner. Scanning ...");
    byte count = 0;
    
    for (byte i = 1; i < 120; i++) {
        Wire.beginTransmission(i);
        if (Wire.endTransmission () == 0) {
            debugSerial.println("Found address: " + String(i, DEC) + " (0x" + String(i, HEX) + ")");
            count++;
            delay (1);
        }
    }
    debugSerial.println("Found " + String(count, DEC) + " device(s).\n");
}

// Function to write a single byte to a register
void I2C::write(uint8_t dev_addr, uint8_t reg_addr, uint8_t* buffer, size_t length) {
    Wire.beginTransmission(dev_addr);         // Start communication with the device
    Wire.write(reg_addr);                     // Send the register address
    for (size_t i = 0; i < length; i++) {
        Wire.write(buffer[i] & 0xFF);
    }
    // Wire.write(settings >> 8);                // Write each byte in the data buffer
    // Wire.write(settings & 0xFF);              // Write each byte in the data buffer
    Wire.endTransmission();                   // End transmission and make the bus inactive
}

// Function to read multiple bytes starting from a register
void I2C::read(uint8_t dev_addr, uint8_t reg_addr, uint8_t* buffer, size_t length) {
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