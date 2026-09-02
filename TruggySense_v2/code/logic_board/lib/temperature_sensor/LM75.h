/**
 * @file LM75.h
 * @author Thomas Kramp (creator)
 * @brief LM75 temperature sensor module.
 * @version 1.0
 * @date 2026-05-25
 * 
 * @copyright Copyright (c) 2026 Thomas Kramp, All rights reserved.
 * This project is released under the CC BY-NC-SA licence.
 * See licence.txt for more information.
 */
#ifndef LM75_H_
#define LM75_H_

#pragma once

#include "I2C.h"
#include "module.h"


#define LM75_TEMP_REG   0x00    /** @brief Temperature register. (read-only) */
#define LM75_CONFIG_REG 0x01    /** @brief Configuration register. (read/write) */
#define LM75_TOS_REG    0x02    /** @brief Over-temperature shutdown threshold register. (read/write) */
#define LM75_THYST_REG  0x03    /** @brief Hysteresis temperature register. (read/write) */

/**
 * @brief Fault queue settings for the LM75.
 *
 * The LM75 supports a fault queue that determines how many consecutive over-
 * temperature events are required before the alert output is asserted.
 */
enum class FAULTS : uint8_t {
    FAULTS_1,  /** @brief 1 fault required before alert. */
    FAULTS_2,  /** @brief 2 faults required before alert. */
    FAULTS_4,  /** @brief 4 faults required before alert. */
    FAULTS_6   /** @brief 6 faults required before alert. */
};

class LM75 : public Module {
    private:
        I2C    *_i2c;             /** @brief I2C bus instance. */
        uint8_t _write_buffer[2]; /** @brief Temporary local buffer for I2C write operations. */
        uint8_t _read_buffer[2];  /** @brief Temporary local buffer for I2C read operations. */

        const uint8_t     _address;   /** @brief I2C address. */
        const FAULTS      _faults;    /** @brief Fault queue setting. */
        const uint8_t     _max_temp;  /** @brief Over-temperature threshold. */
        const uint8_t     _fall_temp; /** @brief Hysteresis temperature threshold. */

    public:
        /**
         * @brief Construct for the LM75 class.
         * @param name      Name of the component to which the LM75 is mounted.
         * @param i2c       I2C bus instance.
         * @param address   I2C address.
         * @param faults    Fault queue.
         * @param max_temp  Over-temperature threshold value.
         * @param fall_temp Hysteresis temperature threshold value.
         */
        LM75(const String name, I2C &i2c, const uint8_t address, 
              const FAULTS faults = FAULTS::FAULTS_1, const uint8_t max_temp = 45, const uint8_t fall_temp = 35)
        : Module("LM75 - " + name + " Temperature Sensor"),
          _i2c(&i2c),
          _address(address),
          _faults(faults),
          _max_temp(max_temp),
          _fall_temp(fall_temp) {}

        /**
         * @brief Initialize the LM75 sensor.
         * @return true if initialization is successful, false otherwise.
         */
        bool    init() override;

        /**
         * @brief Read the current temperature.
         *
         * @return Temperature in degrees Celsius.
         */
        float   getTemperature();
};

#endif