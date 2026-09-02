/**
 * @file encoder.cpp
 * @author Robbe Elsermans
 * @brief Implementation of encoder.h
 * @version 1.0
 * @date 2025-04-10
 * 
 * @copyright Copyright (c) 2025 Robbe Elsermans, All rights reserved.
 * This project is released under the CC BY-NC-SA licence.
 * See licence.txt for more information
 * 
 */
#include "Encoder.h"

// TeensyTimer ITimer(TEENSY_TIMER_1);

volatile long Encoder::wheel_counter[NUM_ENCODER_PINS] = {0};
volatile uint8_t Encoder::wheel_magnets[NUM_ENCODER_PINS] = {FL_N_MAGNET, RL_N_MAGNET, RR_N_MAGNET, FR_N_MAGNET};

Encoder::Encoder() {
    memset(this->wheel_f, 0.0, sizeof(this->wheel_f));       // Initialize all elements to 0
    memset(this->wheel_rpm, 0.0, sizeof(this->wheel_rpm));   // Initialize all elements to 0
    memset(this->wheel_timer, 0, sizeof(this->wheel_timer)); // Initialize all elements to 0

    pinMode(WHEEL_FR, INPUT_PULLUP);
    pinMode(WHEEL_FL, INPUT_PULLUP);
    pinMode(WHEEL_RL, INPUT_PULLUP);
    pinMode(WHEEL_RR, INPUT_PULLUP);

    attachInterrupt(WHEEL_FR, Encoder::itWheelFR, FALLING);
    attachInterrupt(WHEEL_FL, Encoder::itWheelFL, FALLING);
    attachInterrupt(WHEEL_RR, Encoder::itWheelRR, FALLING);
    attachInterrupt(WHEEL_RL, Encoder::itWheelRL, FALLING);
}

void Encoder::itWheelFR() {
    Encoder::wheel_counter[FR]++;
}
void Encoder::itWheelFL() {
    Encoder::wheel_counter[FL]++;
}
void Encoder::itWheelRL() {
    Encoder::wheel_counter[RL]++;
}
void Encoder::itWheelRR() {
    Encoder::wheel_counter[RR]++;
}

float Encoder::getWheelSpeed(MOTOR_POSITION pos) {   
    // Serial.print("Requested position: ");
    // Serial.print(pos);
    // Serial.print(" N_Mag: ");
    // Serial.println(wheel_magnets[pos]);
    cli(); // noInterrupts ();
    long cp_count = wheel_counter[pos];
    wheel_counter[pos] = 0;
    sei(); // interrupts ();

    long t0 = this->wheel_timer[pos];
    long t1 = millis();

    double delta = (t1 - t0) / (1000.0); // To seconds
    this->wheel_timer[pos] = t1;         // Save for next run

    // 1/delta * counter/N_MAG
    wheel_f[pos] = (1.0 / delta) * (double)cp_count / (double)wheel_magnets[pos]; // Hz(rounds per second)
    /**
     * Example
     *
     * Given: N_MAG=40, DELTA=0.1, COUNTER=2,
     *
     * if we have 2 counts in the period of 0.1s, this means that for 1 second, we must divide this counter with 1/0.1=10 which yields 20 counts
     * -> 20/40 = 0.5 Hz so we have a rotation speed of 0.5 Hz.
     * -> F_WHEEL=(1/DELTA)*(COUNT/N_MAG)
     *  */

    wheel_rpm[pos] = wheel_f[pos] * 60.0; // RPM (Rounds per minute)

    return wheel_rpm[pos];
}

float Encoder::getWheelCounter(MOTOR_POSITION pos) {
    cli(); // noInterrupts ();
    long cp_count = wheel_counter[pos];
    sei(); // interrupts ();
    return cp_count;
}