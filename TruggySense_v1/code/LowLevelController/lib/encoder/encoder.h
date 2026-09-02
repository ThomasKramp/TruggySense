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
#pragma once

#include <Arduino.h>
#include "stdint.h"
#include "TimerInterrupt_Generic.h"

#define WHEEL_FR 5 /**< @brief Pin definition of Front Right wheel encoder */
#define WHEEL_FL 3 /**< @brief Pin definition of Front Left wheel encoder */
#define WHEEL_RR 4 /**< @brief Pin definition of Rear Right wheel encoder */
#define WHEEL_RL 2 /**< @brief Pin definition of Rear Left wheel encoder */

#define MOTOR_FR 30 /**< @brief Pin definition of Front Right wheel motor */
#define MOTOR_FL 31 /**< @brief Pin definition of Front Right wheel motor */
#define MOTOR_RR 32 /**< @brief Pin definition of Front Right wheel motor */
#define MOTOR_RL 36 /**< @brief Pin definition of Front Right wheel motor */

/**
 * @brief The number of magnets used on the Front Right wheel
 * 
 */
#define FR_N_MAGNET 14

/**
 * @brief The number of magnets used on the Front Left wheel
 * 
 */
#define FL_N_MAGNET 14

/**
 * @brief The number of magnets used on the Rear Right wheel
 * 
 */
#define RR_N_MAGNET 14

/**
 * @brief The number of magnets used on the Read Left wheel
 * 
 */
#define RL_N_MAGNET 14

/**
 * @brief The amount of encoder pins utilized per group. 
 * 
 */
#define NUM_ENCODER_PINS 4

/**
 * @brief Encoder class that will manage the interrupts triggered by the encoders and calculate the RPM within the trigger interval of
 * getWheelSpeed().
 * 
 */
class Encoder
{
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
     * @brief All possible positions of the encoders
     * 
     */
    enum position
    {
        FL, /**< @brief Front Left position */
        RL, /**< @brief Rear Left position */
        RR, /**< @brief Rear Right position */
        FR  /**< @brief Front Right position */
    };
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
    float getWheelSpeed(position pos);

    /**
     * @brief Get the Wheel Counter state based on a wheel position defined by position.
     * 
     * @param pos which position 
     * @return float the counter value
     */
    float getWheelCounter(position pos);
};