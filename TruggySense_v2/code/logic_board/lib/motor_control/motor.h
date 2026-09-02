/**
 * @file motor.h
 * @author Robbe Elsermans (creator)
 * @author Thomas Kramp (revisor)
 * @brief Control class for both the ESC of the motors and the steering servo.
 * @version 2.0
 * @date 2026-05-25
 * 
 * @copyright Copyright (c) 2025 Robbe Elsermans, All rights reserved.
 * This project is released under the CC BY-NC-SA licence.
 * See licence.txt for more information.
 */
#ifndef MOTOR_H_
#define MOTOR_H_

#pragma once

#include "PWMServo.h"
#include "module.h"

class Motor : public Module {
    private:
        PWMServo      _motor;      /** @brief PWM servo object for controlling the motor/steering. */
        const uint8_t _pin;        /** @brief PWM pin number. */
        const uint8_t _neutral;    /** @brief The PWM value for which the actuator isn't moving. */
        const float   _range;      /** @brief The range to which the PWM signal can deviate from the neutral value in either direction. */
        bool          _disabled;   /** @brief Flag indicating if the motor is disabled. */
    
    public:
        /**
         * @brief Constructs a the Motor class.
         * @param name The information of the module.
         * @param pin The pin to write from.
         * @param neutral The PWM value for which the actuator isn't moving.
         * @param range The range to which the PWM signal can deviate from the neutral value in either direction.
         */
        Motor(const String name, const uint8_t pin, const uint8_t neutral = 90, const float range = 45)
        : Module(name),
          _pin(pin),
          _neutral(neutral),
          _range(range),
          _disabled(true) {}

        /**
         * @brief Initializes the motor control module.
         * @return true if initialization is successful, false otherwise.
         */
        bool init() override;

        /**
         * @brief Writes a PWM value to the motor/servo.
         * @param value The PWM value to write.
         * @param invert_logic If true, inverts the rotation of the actuator.
         */
        void write(const uint16_t value, const bool invert_logic);

        /**
         * @brief Writes throttle and steering values.
         * @param throttle The throttle value for the motor.
         * @param value The steering value for the servo.
         */
        void write(const uint16_t throttle, const uint16_t value);

        /**
         * @brief Disables the motor if the kill switch is activated.
         * @param value The value ranging between 1000 and 2000, where any value below 1500 indicates the kill switch is activated.
         */
        void disable(const uint16_t value);

        /**
         * @brief Disables the motor if the battery cell voltages are too low. 
         * If any of the cell voltages drop below 3.3V, the motor will be disabled to prevent damage to the battery.
         * @param v_cell_1 Voltage of battery cell 1.
         * @param v_cell_2 Voltage of battery cell 2.
         * @param v_cell_3 Voltage of battery cell 3.
         * @param v_cell_4 Voltage of battery cell 4.
         */
        void disable(const float v_cell_1, const float v_cell_2, const float v_cell_3, const float v_cell_4);

        /**
         * @brief Checks if the motor is currently disabled.
         * @return true if disabled, false otherwise.
         */
        bool isDisabled();
};

#endif