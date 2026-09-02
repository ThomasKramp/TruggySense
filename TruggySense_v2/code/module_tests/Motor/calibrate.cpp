#include "Arduino.h"
#include "PWMServo.h"

#ifndef LED_OPTO
  #define LED_OPTO 4
#endif

PWMServo motor;

void setup() {
    motor.attach(LED_OPTO);
    motor.write(0);   // full throttle — needed at power-on for calibration
}
void loop() {}