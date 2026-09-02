#pragma once

#include <Arduino.h>
#include "stdint.h"
#include "TimerInterrupt_Generic.h"

#define WHEEL_FR 5
#define WHEEL_FL 4
#define WHEEL_RR 3
#define WHEEL_RL 2

#define MOTOR_FR 30
#define MOTOR_FL 31
#define MOTOR_RR 32
#define MOTOR_RL 36

// For Teensy 4.0/4.1, F_BUS_ACTUAL = 150 MHz => max period is only 55922 us (~17.9 Hz)
#define N_MAGNET 6.0F

#define TIMER0_INTERVAL_MS  1000L
#define INTERVAL_FACTOR     1000.0/TIMER0_INTERVAL_MS

#define NUM_ENCODER_PINS    4

enum position
{
    FR,
    FL,
    RR,
    RL
};
typedef enum position position_t;

class EncoderV2{
    static volatile long wheel_counter[NUM_ENCODER_PINS];
    double wheel_rpm[NUM_ENCODER_PINS];
    long wheel_timer[NUM_ENCODER_PINS];

    static void itWheelFL();
    static void itWheelFR();
    static void itWheelRL();
    static void itWheelRR();

    public:
        EncoderV2();
        void deInit();
        float getWheelSpeed(position_t pos);
        float getWheelCounter(position_t pos);
};