#ifndef _ENCODER_
#define _ENCODER_

#include "Arduino.h"

#define WHEEL_FR 5
#define WHEEL_FL 4
#define WHEEL_RR 3
#define WHEEL_RL 2

#define MOTOR_FR 30
#define MOTOR_FL 31
#define MOTOR_RR 32
#define MOTOR_RL 36

#define N_MAGNET 6.0

enum position
{
    FR,
    FL,
    RR,
    RL
};

class Encoder
{

    static volatile long enc_w_fr_c[4];
    static volatile long t0[4];
    static volatile long t1[4];
    static volatile bool time_toggler[4];
    static volatile long delta[4];
    static volatile float f_wheel[4];
    static volatile float f_pulse[4];
    static volatile float rpm_wheel[4];
    static volatile float v[4];

    static void itWheelFr();
    static void itWheelFl();
    static void itWheelRr();
    static void itWheelRl();

    static void itMotorFr();
    static void itMotorFl();
    static void itMotorRr();
    static void itMotorRl();

    static void calculateFrequency(position pos);

public:
    static float getWheelFrequency(position position);
    static float getMotorFrequency(position position);
    static void init();
    static void deInit();
};

#endif