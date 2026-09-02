/**
 * @file supply.h
 * @author Robbe Elsermans (creator)
 * @author Thomas Kramp (revisor)
 * @brief Supply voltage monitoring using the LLC's ADCs.
 * @version 2.0
 * @date 2026-05-25
 * 
 * @copyright Copyright (c) 2025 Robbe Elsermans, All rights reserved.
 * This project is released under the CC BY-NC-SA licence.
 * See licence.txt for more information.
 */
#ifndef SUPPLY_H_
#define SUPPLY_H_

#pragma once

#include "module.h"

extern uint8_t adcResolution;  /** @brief ADC resolution in bits. */
extern uint8_t sampleSize;     /** @brief Number of samples to average for voltage readings. */

class Supply : public Module {
    private:
        const uint8_t _pin;      /** @brief ADC pin number. */
        const float   _max_volt; /** @brief Maximum voltage to scale the ADC readings. */

    public:
        /**
         * @brief Constructor for Supply class.
         * @param name Name of the source.
         * @param pin Pin to read from.
         * @param max_volt Maximum voltage value (used for scaling).
         */
        Supply(const String name, const uint8_t pin, const float max_volt) 
        : Module("ADC - " + name),
          _pin(pin),
          _max_volt(max_volt) {}

        /**
         * @brief Initializes the supply module.
         * @return True if initialization is successful, false otherwise.
         */
        bool init() override;

        /**
         * @brief Gets the current voltage reading.
         * @return The voltage in volts.
         */
        float getVoltage();
};

#endif