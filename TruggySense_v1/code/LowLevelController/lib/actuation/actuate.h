/**
 * @file actuate.h
 * @author Robbe Elsermans
 * @brief A driver to drive PWM controlled modules such as the ESC and Servo
 * 
 * The PWM modules will be actuated through the duration in microseconds which goes from 1000µs to 2000µs. 
 * In order to use PWM in conmination with interrupts, 
 * PWMServo.h is used instead of Servo.h. See <href src="https://www.pjrc.com/teensy/td_libs_Servo.html">here</href> why.
 * 
 * @note I've added a detach() function to the PWMServo.cpp which does the job for me because it was not implemented by default.
 * 
 * @version 1.0
 * @date 2025-04-10
 *
 * @copyright Copyright (c) 2025 Robbe Elsermans, All rights reserved.
 * This project is released under the CC BY-NC-SA licence.
 * See licence.txt for more information
 *
 */
#pragma once

#include "stdint.h"
#include <PWMServo.h>
#include "wiring.h"

#define PWM_PIN_MOTOR_RR 33     /**< @brief Pin definition of motor Rear Right */
#define PWM_PIN_MOTOR_RL 9      /**< @brief Pin definition of motor Rear Left */
#define PWM_PIN_MOTOR_FR 23     /**< @brief Pin definition of motor Front Right */
#define PWM_PIN_MOTOR_FL 37     /**< @brief Pin definition of motor Front Left */
#define PWM_PIN_STEER_SERVO 22  /**< @brief Pin definition of Steer Servo */

/**
 * @brief Minimum us value that can be received.
 */
#define PWM_US_LOW_VAL 1000

/**
 * @brief Maximum us value that can be received.
 *
 */
#define PWM_US_HIGH_VAL 2000

/**
 * @brief Actuate class will actuate the received microseconds value to the selected pin defined by enum actuator.
 */
class Actuate
{
private:
    uint8_t disabled; /**< @brief flag to disable or enable all servo pins. */

public:
    /**
     * @brief All possible options to actuate
     *
     */
    enum actuator
    {
        M_FR, /**< @brief Motor Front Right */
        M_FL, /**< @brief Motor Front Left */
        M_RR, /**< @brief Motor Rear Right */
        M_RL, /**< @brief Motor Rear Left */
        SS    /**< @brief Steer Servo */
    };

    /**
     * @brief Initialization of the PWM pins
     *
     * Will attach the pins to the PWMServo library and will disable the pins such that no unwanted movement is caused.
     * 
     * @return uint8_t returns 1 for success and 0 for failure
     */
    uint8_t init();

    /**
     * @brief Write a certain µs value on the specified pin
     *
     * @param pin the pin to actuate
     * @param pwm_value_us the value to actuate
     *
     * @note, internally, it will be converted between 0-180°
     */
    void write(actuator act, uint16_t pwm_value_us);

    /**
     * @brief Disable all PWM pins
     *
     */
    void disableAll();

    /**
     * @brief Enable all PWM pins
     *
     */
    void enableAll();

    /**
     * @brief check if the PWM pins are disabled
     *
     * @return uint8_t 1 means disabled and 0 means not disabled
     */
    uint8_t isDisabled();
};