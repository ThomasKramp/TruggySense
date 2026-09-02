/**
 * @file I2C.h
 * @author Thomas Kramp (creator)
 * @brief I2C initializer and interface.
 * @version 1.0
 * @date 2026-05-25
 * 
 * @copyright Copyright (c) 2026 Thomas Kramp, All rights reserved.
 * This project is released under the CC BY-NC-SA licence.
 * See licence.txt for more information.
 */
#ifndef I2C_H_
#define I2C_H_

#pragma once

#include <stdint.h>
#include <Wire.h>

#include "i2c_addresses.h"
#include "interrupt_utils.h"

extern Print &debugSerial;

class I2C {
    private:
        void scan();

    public:
        I2C();

        void write(uint8_t dev_addr, uint8_t reg_addr, uint8_t* buffer, size_t length);
        void read(uint8_t dev_addr, uint8_t reg_addr, uint8_t* buffer, size_t length);
};

#endif