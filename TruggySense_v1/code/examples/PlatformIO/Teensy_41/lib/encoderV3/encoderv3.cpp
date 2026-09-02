#include "encoderv3.h"

volatile unsigned long EncoderV3::wheel_counter[4] = {0};

volatile unsigned long t0;
volatile unsigned long t1;
volatile unsigned long delta;
volatile uint8_t meas_done = false;

EncoderV3::EncoderV3()
{
    memset(this->wheel_rpm, 0.0, sizeof(this->wheel_rpm)); // Initialize all elements to 0
    // You can select Teensy Hardware Timer  from TEENSY_TIMER_1 or TEENSY_TIMER_3

    // Init Teensy timer TEENSY_TIMER_1
    // TeensyTimer ITimer0(TEENSY_TIMER_3);

    pinMode(WHEEL_FR, INPUT_PULLUP);
    pinMode(WHEEL_FL, INPUT_PULLUP);
    pinMode(WHEEL_RL, INPUT_PULLUP);
    pinMode(WHEEL_RR, INPUT_PULLUP);

    // ITimer0.attachInterruptInterval(TIMER0_INTERVAL_MS * 1000, EncoderV2::TimerHandler0);
    attachInterrupt(WHEEL_FR, EncoderV3::itWheelFR, FALLING);
    attachInterrupt(WHEEL_FL, EncoderV3::itWheelFL, FALLING);
    attachInterrupt(WHEEL_RR, EncoderV3::itWheelRR, FALLING);
    attachInterrupt(WHEEL_RL, EncoderV3::itWheelRL, FALLING);
}

void EncoderV3::itWheelFR()
{
    //When detected the second fall, we have a duration of 1/6 per relvolution
    if(meas_done)
    {
        meas_done = false;
        t1 = millis();
        delta = t1 - t0;
    }
    else
    {
        meas_done = true;
        t0 = millis();
    }

    EncoderV3::wheel_counter[FR]++;
}

void EncoderV3::itWheelFL()
{
    EncoderV3::wheel_counter[FL]++;
}

void EncoderV3::itWheelRR()
{
    EncoderV3::wheel_counter[RR]++;
}

void EncoderV3::itWheelRL()
{
    EncoderV3::wheel_counter[RL]++;
}

float EncoderV3::getWheelSpeed(position pos)
{
    cli();
    unsigned long cp_delta = delta;
    sei();

    wheel_rpm[pos] = (1.0/((float)cp_delta/1000.0))*(6)*60;

    return wheel_rpm[pos];
}

float EncoderV3::getWheelCounter(position pos)
{
    cli(); // noInterrupts ();
    long cp_count = wheel_counter[pos];
    sei(); // interrupts ();
    return cp_count;
}