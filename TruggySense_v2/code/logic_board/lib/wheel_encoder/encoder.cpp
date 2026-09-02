/**
 * @file encoder.cpp
 * @author Robbe Elsermans (creator)
 * @author Thomas Kramp (revisor)
 * @brief Implementation of encoder.h
 * @version 2.0
 * @date 2026-05-25
 * 
 * @copyright Copyright (c) 2025 Robbe Elsermans, All rights reserved.
 * This project is released under the CC BY-NC-SA licence.
 * See licence.txt for more information.
 */
#include "encoder.h"

// #define PRINTDEBUG
// TeensyTimer ITimer(TEENSY_TIMER_1);

volatile long Encoder::_wheel_counter[NUM_ENCODER_PINS] = {0};

static void (*_isrTable[NUM_ENCODER_PINS])() = {
    Encoder::iterateWheelFR,
    Encoder::iterateWheelFL,
    Encoder::iterateWheelRR,
    Encoder::iterateWheelRL
};

bool Encoder::init() {
    pinMode(_pin, INPUT_PULLUP);
    attachInterrupt(_pin, _isrTable[(uint8_t)(_pos)], FALLING);
    return true;
}

void Encoder::iterateWheelFR() { Encoder::_wheel_counter[(uint8_t)(POSITION::FR)]++; }
void Encoder::iterateWheelFL() { Encoder::_wheel_counter[(uint8_t)(POSITION::FL)]++; }
void Encoder::iterateWheelRR() { Encoder::_wheel_counter[(uint8_t)(POSITION::RR)]++; }
void Encoder::iterateWheelRL() { Encoder::_wheel_counter[(uint8_t)(POSITION::RL)]++; }

float Encoder::getWheelSpeed() {
    interrupts_disable();
        long count = _wheel_counter[(uint8_t)(_pos)];
        _wheel_counter[(uint8_t)(_pos)] = 0;
    interrupts_enable();

    long t0 = _wheel_timer;
    long t1 = millis();

    double delta = (t1 - t0) / (1000.0); // To seconds
    _wheel_timer = t1;         // Save for next run

    // 1/delta * counter/N_MAG
    double wheel_f = (1.0 / delta) * (double)count / (double)_magnets; // Hz(rounds per second)
    /**
     * Example
     *
     * Given: N_MAG=40, DELTA=0.1, COUNTER=2,
     *
     * if we have 2 counts in the period of 0.1s, this means that for 1 second, we must divide this counter with 1/0.1=10 which yields 20 counts
     * -> 20/40 = 0.5 Hz so we have a rotation speed of 0.5 Hz.
     * -> F_WHEEL=(1/DELTA)*(COUNT/N_MAG)
     *  */

    double wheel_rpm = wheel_f * 60.0; // RPM (Rounds per minute)

    #ifdef PRINTDEBUG
        debugSerial.print("\tMagnet passes: \t" + String(count) + "\t");
        debugSerial.print("\tMagnets: \t" + String(_magnets) + "\t");
        debugSerial.println("\tRPM: \t" + String(wheel_rpm));
    #endif

    return wheel_rpm;
}

long Encoder::getWheelCounter() {
    interrupts_disable();
        long count = _wheel_counter[(uint8_t)(_pos)];
    interrupts_enable();
    return count;
}