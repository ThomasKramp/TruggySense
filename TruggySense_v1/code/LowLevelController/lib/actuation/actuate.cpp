/**
 * @file actuate.cpp
 * @author Robbe Elsermans
 * @brief Implementation of actuate.h
 * @version 1.0
 * @date 2025-04-10
 * 
 * @copyright Copyright (c) 2025 Robbe Elsermans, All rights reserved.
 * This project is released under the CC BY-NC-SA licence.
 * See licence.txt for more information
 * 
 */
#include "actuate.h"
#include <Arduino.h>

PWMServo steer_servo;
PWMServo motor_RR;
PWMServo motor_RL;
PWMServo motor_FR;
PWMServo motor_FL;

uint8_t Actuate::init()
{
    disabled = true;
    // Initialize motor and servos
    if (!motor_RR.attach(PWM_PIN_MOTOR_RR)) // 33 motor RR
        return 0;

    // if (!motor_RL.attach(PWM_PIN_MOTOR_RL)) // 33 motor RR
    //     return 0;
    // if (!motor_FR.attach(PWM_PIN_MOTOR_FR)) // 33 motor RR
    //     return 0;
    // if (!motor_RL.attach(PWM_PIN_MOTOR_FL)) // 33 motor RR
    //     return 0;

    if (!steer_servo.attach(PWM_PIN_STEER_SERVO)) // 22 Servo
    {
        return 0;
    }

    disabled = false;
    return 1;
}

void Actuate::write(actuator act, uint16_t pwm_value_us)
{

    if (disabled)
    {
        return;
    }

    switch (act)
    {
    case Actuate::M_RR:
        motor_RR.write(map(pwm_value_us, PWM_US_LOW_VAL, PWM_US_HIGH_VAL, 0, 180));
        break;

    case Actuate::M_RL:
        motor_RL.write(map(pwm_value_us, PWM_US_LOW_VAL, PWM_US_HIGH_VAL, 0, 180));
        break;

    case Actuate::M_FR:
        motor_FR.write(map(pwm_value_us, PWM_US_LOW_VAL, PWM_US_HIGH_VAL, 0, 180));
        break;

    case Actuate::M_FL:
        motor_FL.write(map(pwm_value_us, PWM_US_LOW_VAL, PWM_US_HIGH_VAL, 0, 180));
        break;

    case Actuate::SS:
        steer_servo.write(map(pwm_value_us, PWM_US_LOW_VAL, PWM_US_HIGH_VAL, 0, 180));
        break;

    default:
        break;
    }
}

void Actuate::disableAll()
{
    disabled = true;

    // Serial.println("disabling");

    // detach is not implemented in PWMServo. So do it manually
    // https://forum.pjrc.com/index.php?threads/pwmservo-examples-compile-failure.27895/page-2
    motor_RR.detach();
    // motor_RL.detach();
    // motor_FR.detach();
    // motor_RL.detach();
    steer_servo.detach();
}

void Actuate::enableAll()
{
    init();
}

uint8_t Actuate::isDisabled()
{
    return disabled;
}