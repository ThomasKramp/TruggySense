#include "encoder.h"

// Define the static member variables (allocate memory for them)
volatile long Encoder::enc_w_fr_c[4] = {0};
volatile long Encoder::t0[4] = {0};
volatile long Encoder::t1[4] = {0};
volatile bool Encoder::time_toggler[4] = {true};
volatile long Encoder::delta[4] = {0};
volatile float Encoder::f_wheel[4] = {1};
volatile float Encoder::f_pulse[4] = {0};
volatile float Encoder::rpm_wheel[4] = {0};
volatile float Encoder::v[4] = {0};

void Encoder::init()
{
    attachInterrupt(WHEEL_FR, Encoder::itWheelFr, FALLING);
}

void Encoder::deInit()
{
    detachInterrupt(WHEEL_FR);
}

void Encoder::itWheelFr()
{
    Encoder::calculateFrequency(FR);
}

void Encoder::itWheelFl()
{
    Encoder::calculateFrequency(FL);
}

void Encoder::itWheelRr()
{
    Encoder::calculateFrequency(RR);
}

void Encoder::itWheelRl()
{
    Encoder::calculateFrequency(RL);
}

void Encoder::calculateFrequency(position pos) 
{
        if (time_toggler[pos])
    {
        time_toggler[pos] = false;
        t0[pos] = micros();
        delta[pos] = t0[pos] - t1[pos];
    }
    else
    {
        time_toggler[pos] = true;
        t1[pos] = micros();
        delta[pos] = t1[pos] - t0[pos];
    }

    f_pulse[pos] = 1 / (delta[pos] / 1000000.0);

    f_wheel[pos] = f_pulse[pos] / N_MAGNET;

    rpm_wheel[pos] = f_wheel[pos] * 60;
}

float Encoder::getWheelFrequency(position position)
{
    switch (position)
    {
    case position::FR:
        return f_wheel[0];
        break;
    case position::FL:
        /* code */
        break;
    case position::RR:
        /* code */
        break;
    case position::RL:
        /* code */
        break;

    default:
        break;
    }
    return 0;
}