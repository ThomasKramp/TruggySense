/**
 * @file fan.h
 * @author Thomas Kramp (creator)
 * @brief Control class for the cooling fan.
 * @version 1.0
 * @date 2026-05-25
 * 
 * @copyright Copyright (c) 2025 Robbe Elsermans, All rights reserved.
 * This project is released under the CC BY-NC-SA licence.
 * See licence.txt for more information.
 */
#ifndef FAN_H_
#define FAN_H_

#pragma once

#include "module.h"

class Fan : public Module {
    private:
        const uint8_t _pin; /** @brief GPIO pin number. */
        
    public:
        /**
         * @brief Constructor for the Fan class.
         * @param pin Digital output pin used to control the fan.
         */
        Fan(const uint8_t pin)
        : Module("800-19322-01B0 - Cooling Fan"),
          _pin(pin) {}

        /**
         * @brief Initialize the fan module.
         * @return true if successful, false otherwise.
         */
        bool init() override;

        /**
         * @brief Write a raw PWM value to the fan.
         * @param value Value between 1000 and 2000.
         */
        void write(const uint16_t value);

        /**
         * @brief Adjust the fan output based on temperature.
         * @param temperature Measured temperature used to determine fan speed.
         */
        void write(const float temperature);
};

#endif