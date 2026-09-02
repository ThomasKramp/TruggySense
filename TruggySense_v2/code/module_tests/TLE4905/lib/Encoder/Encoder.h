/**
 * @file encoder.h
 * @author Robbe Elsermans
 * @brief Enables encoding of the wheels and motors utilized by the RCV
 * @version 0.1
 * @date 2025-04-10
 * 
 * @note only wheel part is implemented
 * 
 * @copyright Copyright (c) 2025 Robbe Elsermans, All rights reserved.
 * This project is released under the CC BY-NC-SA licence.
 * See licence.txt for more information
 * 
 */
#ifndef ENCODER_H_
#define ENCODER_H_

#pragma once

#include "Arduino.h"
#include "Motor_Position.h"
#include "stdint.h"
#include "TimerInterrupt_Generic.h"

#define WHEEL_FR 24 /** @brief Pin definition of Front Right wheel encoder */
#define WHEEL_FL 25 /** @brief Pin definition of Front Left wheel encoder */
#define WHEEL_RR 26 /** @brief Pin definition of Rear Right wheel encoder */
#define WHEEL_RL 27 /** @brief Pin definition of Rear Left wheel encoder */

#define FR_N_MAGNET 14 /** @brief The number of magnets used on the Front Right wheel */
#define FL_N_MAGNET 14 /** @brief The number of magnets used on the Front Left wheel */
#define RR_N_MAGNET 14 /** @brief The number of magnets used on the Rear Right wheel */
#define RL_N_MAGNET 14 /** @brief The number of magnets used on the Rear Left wheel */

#define NUM_ENCODER_PINS 4 /** @brief The amount of encoder pins utilized per group. */

/**
 * @brief Encoder class that will manage the interrupts triggered by the encoders 
 * and calculate the RPM within the trigger interval of getWheelSpeed().
 */
class Encoder {
    static volatile long wheel_counter[NUM_ENCODER_PINS];
    static volatile uint8_t wheel_magnets[NUM_ENCODER_PINS];
    double wheel_f[NUM_ENCODER_PINS];
    double wheel_rpm[NUM_ENCODER_PINS];
    long wheel_timer[NUM_ENCODER_PINS];

    static void itWheelFL();
    static void itWheelFR();
    static void itWheelRL();
    static void itWheelRR();

public:
    /**
     * @brief Construct a new Encoder object and assign the encoder pins as interrupts
     * @note a pullup is added to the encoder pins internally where the interrupts will be triggered for a falling edge.
     */
    Encoder();

    /**
     * @brief Get the Wheel Speed in RPM
     * 
     * Will calculate the RPM of a given wheel position defined by position. It will use the pulse counter from the wheel pin
     * in combination with the duration between triggers of this method.
     * 
     * @param pos which position 
     * @return float the RPM
     */
    float getWheelSpeed(MOTOR_POSITION pos);

    /**
     * @brief Get the Wheel Counter state based on a wheel position defined by position.
     * 
     * @param pos which position 
     * @return float the counter value
     */
    float getWheelCounter(MOTOR_POSITION pos);
};
#endif