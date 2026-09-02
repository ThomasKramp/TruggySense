/**
 * @file led.h
 * @author Thomas Kramp (creator)
 * @brief Control class for the indicator LED.
 * @version 1.0
 * @date 2026-05-25
 * 
 * @copyright Copyright (c) 2025 Robbe Elsermans, All rights reserved.
 * This project is released under the CC BY-NC-SA licence.
 * See licence.txt for more information.
 */
#ifndef LED_H_
#define LED_H_

#pragma once

#include "module.h"

class LED : public Module {
    private:
        const uint8_t _pin; /** @brief GPIO pin number. */
        bool _ledState;     /** @brief The current state of the LED (true for on, false for off). */

    public:
        /**
         * @brief Constructor for the LED class.
         * @param pin The pin to write from.
        */
        LED(const uint8_t pin)
        : Module("LED - Indicator LED"),
          _pin(pin),
          _ledState(LOW) {}

        /** 
         * @brief Initializes the LED pin as output.
         * @return true if successful, false otherwise.
         */
        bool init() override;

        /** 
         * @brief Writes the specified value to the LED.
         * @param value true to turn on, false to turn off.
         */
        void write(const bool value);
        
        /** @brief Toggles the current state of the LED. */
        void toggle();
};

#endif