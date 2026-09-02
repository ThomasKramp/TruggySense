/**
 * @file encoder.h
 * @author Robbe Elsermans (creator)
 * @author Thomas Kramp (revisor)
 * @brief Enables encoding of the wheels and motors utilized by the RCV.
 * @version 2.0
 * @date 2026-05-25
 * 
 * @copyright Copyright (c) 2025 Robbe Elsermans, All rights reserved.
 * This project is released under the CC BY-NC-SA licence.
 * See licence.txt for more information.
 */
#ifndef ENCODER_H_
#define ENCODER_H_

#pragma once

// #include "TimerInterrupt_Generic.h"
#include "module.h"

#define NUM_ENCODER_PINS      4  /** @brief The amount of encoder pins utilized per group. */ 
#define ENCODER_MAGNET_COUNT 14  /** @brief The amount of mangets utilized per sensor. */ 

/** @brief All possible positions of the encoders. */
enum class POSITION : uint8_t {
    FR, /** @brief Front Right position */
    FL, /** @brief Front Left position */
    RR, /** @brief Rear Right position */
    RL  /** @brief Rear Left position */
};

class Encoder : public Module {
    private:
        const uint8_t  _pin;     /** @brief The pin to which the encoder is connected. */
        const POSITION _pos;     /** @brief The position of the wheel to which the encoder is attached. */
        const uint8_t  _magnets; /** @brief The number of magnets used by this wheel. */
        long _wheel_timer;       /** @brief A timer to calculate the duration between triggers of getWheelSpeed(). */

        static volatile long _wheel_counter[NUM_ENCODER_PINS];

    public:
        /**
         * @brief Constructor for the Encoder class.
         * @param name The position of the encoder.
         * @param pin The pin number connected to the encoder.
         * @param pos The position of the wheel to which the encoder is attached.
         * @param magnets The number of magnets used in the encoder setup.
         */
        Encoder(const String name, const uint8_t pin, const POSITION pos, const uint8_t  magnets = ENCODER_MAGNET_COUNT)
        : Module("AH3323A-P-B - " + name + " Wheel encoder"),
          _pin(pin),
          _pos(pos),
          _magnets(magnets),
          _wheel_timer(0) {}

        /**
         * @brief Initializes the encoder by setting up the necessary pin modes and attaching the interrupts.
         * @return true if initialization is successful, false otherwise.
         * @note A pullup is added to the encoder pins internally where the interrupts will be triggered for a falling edge.
         */
        bool init() override;

        /**
         * @brief Get the wheel speed in RPM. 
         * It will use the pulse counter from the wheel pin in combination with the duration between triggers of this method.
         * @return The RPM of the wheel.
         */
        float getWheelSpeed();

        /**
         * @brief Get the wheel counter state.
         * @return The wheel counter value.
         */
        long getWheelCounter();

        /** 
         * @brief Interrupt service routines for each encoder position.
         * @note These methods have to be defined individually, since attachInterrupt only accepts static functions.
         */
        static void iterateWheelFL();
        static void iterateWheelFR();
        static void iterateWheelRL();
        static void iterateWheelRR();
};
#endif