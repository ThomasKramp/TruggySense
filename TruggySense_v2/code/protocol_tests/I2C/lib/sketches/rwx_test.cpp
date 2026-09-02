/**
 * Read & Write test
 *
 * Testing the I2C read and write functions on the INA219
 */

#include "Arduino.h"
#include "Wire.h"

#ifndef LED_BUILTIN
    #define LED_BUILTIN 13
#endif

#ifndef DEV_ADDRESS
    #define DEV_ADDRESS 0x40
#endif

#ifndef REG_ADDRESS
    #define REG_ADDRESS 0x00
#endif

void i2c_write(uint8_t reg_addr, uint8_t* buffer, size_t length);
void i2c_read(uint8_t reg_addr, uint8_t* buffer, size_t length);

uint8_t write_buffer[2] = {0x00,0x01};
uint8_t read_buffer[2];

void setup() {
    Wire.begin();
    Serial.begin (115200);
    
    // Turn led on to indicate Ready state
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);

    // Wait for serial port to connect
    while (!Serial) { }

    // Turn led off
    digitalWrite(LED_BUILTIN, LOW);
}

void loop() {
    uint16_t value = (write_buffer[0] << 8) | write_buffer[1];
    value = (value << 1) | (value >> 15);   // rotate left
    write_buffer[0] = value >> 8;           // MSB
    write_buffer[1] = value & 0xFF;         // LSB
    
    i2c_write(REG_ADDRESS, write_buffer, 2);
    delay(100);
    Serial.print("Send:\t\t"); Serial.println((write_buffer[0] << 8) | write_buffer[1]);

    i2c_read(REG_ADDRESS, read_buffer, 2);
    Serial.print("Receive:\t"); Serial.println((read_buffer[0] << 8) | read_buffer[1]);
    Serial.println();
    delay(1900);
}

// Function to write a single byte to a register
void i2c_write(uint8_t reg_addr, uint8_t* buffer, size_t length) {
    Wire.beginTransmission(DEV_ADDRESS);      // Start communication with the device
    Wire.write(reg_addr);                     // Send the register address
    for (size_t i = 0; i < length; i++) {
        Wire.write(buffer[i] & 0xFF);
    }
    // Wire.write(settings >> 8);                // Write each byte in the data buffer
    // Wire.write(settings & 0xFF);              // Write each byte in the data buffer
    Wire.endTransmission();                   // End transmission and make the bus inactive
}

// Function to read multiple bytes starting from a register
void i2c_read(uint8_t reg_addr, uint8_t* buffer, size_t length) {
    Wire.beginTransmission(DEV_ADDRESS);
    Wire.write(reg_addr);
    Wire.endTransmission(false);
    Wire.requestFrom(DEV_ADDRESS, length);

    for (size_t i = 0; i < length; i++) {
        if (Wire.available()) {
            buffer[i] = Wire.read();
        } else {
            buffer[i] = 0; // Fill with 0 if no data
        }
    }
}