/**
 * @file ACS758.h
 * @author Robbe Elsermans (creator)
 * @author Thomas Kramp (revisor)
 * @brief ACS758 module to measure the currents of the motors.
 * @version 2.0
 * @date 2026-05-25
 * 
 * @copyright Copyright (c) 2025 Robbe Elsermans, All rights reserved.
 * This project is released under the CC BY-NC-SA licence.
 * See licence.txt for more information.
 */
#ifndef ACS758_H_
#define ACS758_H_

#pragma once

#include "module.h"

extern uint8_t adcResolution;  /** @brief ADC resolution in bits. */
extern uint8_t sampleSize;     /** @brief Number of samples to average for voltage readings. */

class ACS758 : public Module {
    private:
        // Private vars
        const uint8_t _pin;             /** @brief ADC pin number. */
        const float   _supply_voltage;  /** @brief Supply voltage of the sensor, as a reference. */
        const uint8_t _prim_samp_curr;  /** @brief Maximum allowed current. */
        const float   _sensitivity;     /** @brief Sensitivity of the sensor, in mV/A. */
        const bool    _bidirectional;   /** @brief Indicates if the sensor works bidirectional. */

    public:
        /** 
         * @brief Constructor for the ACS758 class. 
         * @param name The position of the sensor.
         * @param pin Pin to read from.
         * @param vcc Supply voltage of the sensor, as a reference.
         * @param prim_samp_curr Maximum allowed current, used for scaling the output.
         * @param sensitivity Sensitivity of the sensor, in mV/A.
         * @param bidirectional Indicates if the sensor works bidirectional. 
         * If true, the output voltage is centered around half of the supply voltage. 
         * If false, the output voltage is 0V at 0A and increases with current.
         */
        ACS758(const String name, const uint8_t pin, const float vcc = 3.3f,
               const uint8_t prim_samp_curr = 200, const float sensitivity = 0.010f, const bool bidirectional = true)
        : Module("ACS758 - " + name + " High Current Sensor"),
          _pin(pin),
          _supply_voltage(vcc),
          _prim_samp_curr(prim_samp_curr),
          _sensitivity(sensitivity),
          _bidirectional(bidirectional) {}

        /**
         * @brief Initializes the ACS758 sensor.
         * @return True if initialization is successful, false otherwise.
         */
        bool init() override;

        /**
         * @brief Reads the motor current.
         * @return The measured current in Amperes.
         */
        float getCurrent();
};

#endif