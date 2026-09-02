#include "encoderv2.h"

// TeensyTimer ITimer(TEENSY_TIMER_1);

volatile long EncoderV2::wheel_counter[NUM_ENCODER_PINS] = {0};

EncoderV2::EncoderV2()
{
    memset(this->wheel_rpm, 0.0, sizeof(this->wheel_rpm)); // Initialize all elements to 0
    memset(this->wheel_timer, 0, sizeof(this->wheel_timer));   // Initialize all elements to 0
    // You can select Teensy Hardware Timer  from TEENSY_TIMER_1 or TEENSY_TIMER_3

    // Init Teensy timer TEENSY_TIMER_1
    // TeensyTimer ITimer0(TEENSY_TIMER_3);

    pinMode(WHEEL_FR, INPUT_PULLUP);
    pinMode(WHEEL_FL, INPUT_PULLUP);
    pinMode(WHEEL_RL, INPUT_PULLUP);
    pinMode(WHEEL_RR, INPUT_PULLUP);

    // ITimer0.attachInterruptInterval(TIMER0_INTERVAL_MS * 1000, EncoderV2::TimerHandler0);
    attachInterrupt(WHEEL_FR, EncoderV2::itWheelFR, FALLING);
    attachInterrupt(WHEEL_FL, EncoderV2::itWheelFL, FALLING);
    attachInterrupt(WHEEL_RR, EncoderV2::itWheelRR, FALLING);
    attachInterrupt(WHEEL_RL, EncoderV2::itWheelRL, FALLING);
}

void EncoderV2::itWheelFR()
{
    EncoderV2::wheel_counter[FR]++;
}

void EncoderV2::itWheelFL()
{
    EncoderV2::wheel_counter[FL]++;
}

void EncoderV2::itWheelRR()
{
    EncoderV2::wheel_counter[RR]++;
}

void EncoderV2::itWheelRL()
{
    EncoderV2::wheel_counter[RL]++;
}

float EncoderV2::getWheelSpeed(position pos)
{
    cli(); // noInterrupts ();
    long cp_count = wheel_counter[pos];
    wheel_counter[pos] = 0;
    sei(); // interrupts ();

    long t0 = this->wheel_timer[pos];
    long t1 = millis();

    this->wheel_timer[pos] = t1;

    double delta = (t1 - t0)/1000.0;   //To seconds
    Serial.println(cp_count);
    wheel_rpm[pos] = (1.0 / (N_MAGNET * (delta))) * (double)cp_count; //Hz
    //Serial.println(wheel_rpm[pos]);

    //wheel_rpm[pos] *= 60.0;   //RPM

    return wheel_rpm[pos];
}

float EncoderV2::getWheelCounter(position pos)
{
    cli(); // noInterrupts ();
    long cp_count = wheel_counter[pos];
    sei(); // interrupts ();
    return cp_count;
}