/**
 * @file INA219.h
 * @author Thomas Kramp (creator)
 * @brief INA219 module to measure the currents of all low voltage modules.
 * @version 1.0
 * @date 2026-05-25
 *
 * @copyright Copyright (c) 2026 Thomas Kramp, All rights reserved.
 * This project is released under the CC BY-NC-SA licence.
 * See licence.txt for more information.
 */
#ifndef INA219_H_
#define INA219_H_

#pragma once

#include "I2C.h"
#include "module.h"

/** INA219 register addresses. */
#define INA219_CONFIG_REG       0x00    /** @brief Configuration register (read/write). */
#define INA219_SHUNT_VOLT_REG   0x01    /** @brief Shunt voltage register (read-only). */
#define INA219_BUS_VOLT_REG     0x02    /** @brief Bus voltage register (read-only). */
#define INA219_POWER_REG        0x03    /** @brief Power register (read-only). */
#define INA219_CURRENT_REG      0x04    /** @brief Current register (read-only). */
#define INA219_CALIBRATE_REG    0x05    /** @brief Calibration register (read/write). */

class INA219 : public Module {
    private:
        I2C    *_i2c;             /** @brief I2C bus instance. */
        uint8_t _write_buffer[2]; /** @brief Temporary local buffer for I2C write operations. */
        uint8_t _read_buffer[2];  /** @brief Temporary local buffer for I2C read operations. */

        const uint8_t _address;         /** @brief I2C address. */
        const uint8_t _shunt_resistor;  /** @brief Shunt resistor value in milliohms. */
        const float   _max_current;     /** @brief Maximum expected current in amps. */
        const float   _current_LSB;     /** @brief The amps per bit. */
        const float   _rail_voltage;    /** @brief Supply voltage in volts. */

    public:
        /**
         * @brief Construct a new INA219 sensor.
         * @param name The module group which current is measured.
         * @param i2c The I2C interface.
         * @param address The I2C address.
         * @param shunt_resistor Shunt resistor value in milliohms.
         * @param max_current Maximum expected current range in amps.
         * @param rail_voltage Supply rail voltage in volts.
         */
        INA219(const String name, I2C &i2c, const uint8_t address,
               const uint8_t shunt_resistor, const float max_current, const float rail_voltage)
        : Module("INA219 - " + name + " Low Current Sensor"),
          _i2c(&i2c),
          _address(address),
          _shunt_resistor(shunt_resistor),
          _max_current(max_current),
          _current_LSB(max_current / 32768.0f),
          _rail_voltage(rail_voltage) {}

        /**
         * @brief Initialize the INA219 sensor.
         * @return true if initialization is successful, false otherwise.
         */
        bool init() override;

        /**
         * @brief Read the current flowing through the shunt resistor.
         * @return Current in amps.
         */
        float getCurrent();
};

#endif