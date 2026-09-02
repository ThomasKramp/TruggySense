#include <Arduino.h>
#include "../../Teensy_41/.pio/libdeps/teensy41/TimerInterrupt_Generic/src/TeensyTimerInterrupt_Generic.h"
#include "../../Teensy_41/.pio/libdeps/teensy41/TimerInterrupt_Generic/src/ISR_Timer_Generic.h"

// You can select Teensy Hardware Timer  from TEENSY_TIMER_1 or TEENSY_TIMER_3

// Init Teensy timer TEENSY_TIMER_1
TeensyTimer ITimer(TEENSY_TIMER_1);

// Init Teensy_ISR_Timer
// Each Teensy_ISR_Timer can service 16 different ISR-based timers
ISR_Timer Teensy_ISR_Timer;
void TimerHandler()
{
  Teensy_ISR_Timer.run();
}

#define HW_TIMER_INTERVAL_MS 10L

#define TIMER_INTERVAL_2S 500L
#define TIMER_INTERVAL_5S 4000L
#define TIMER_INTERVAL_11S 6000L
#define TIMER_INTERVAL_101S 101000L

#define N_MAGNET 6.0F

#define TIMER0_INTERVAL_MS  500L
#define INTERVAL_FACTOR     1000.0/TIMER0_INTERVAL_MS

volatile uint16_t wheel_counter[4];
volatile float wheel_freq[4];

// In Teensy, avoid doing something fancy in ISR, for example complex Serial.print with String() argument
// The pure simple Serial.prints here are just for demonstration and testing. Must be eliminate in working environment
// Or you can get this run-time error / crash
void doingSomething2s()
{
  // Doing something here inside ISR every 2 seconds
  digitalWrite(2, !digitalRead(2));

  float _temp = wheel_counter[0];
  wheel_counter[0] = 0;

  _temp *= INTERVAL_FACTOR; // bring the measurement back up to 1 second
  _temp /= N_MAGNET;

  wheel_freq[0] = _temp;
}

void doingSomething5s()
{
  // Doing something here inside ISR every 5 seconds

  digitalWrite(3, !digitalRead(3));
}

void doingSomething11s()
{
  // Doing something here inside ISR  every 11 seconds

  digitalWrite(4, !digitalRead(4));
}

long lastMillis = 0;
volatile long counter = 0;

void itEncoder()
{
  cli();
  wheel_counter[0]++;
  sei();
}

void setup()
{
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);

  pinMode(5, INPUT);
  attachInterrupt(5, itEncoder, FALLING);

  Serial.begin(9600);

  // Interval in microsecs
  if (ITimer.attachInterruptInterval(HW_TIMER_INTERVAL_MS * 1000, TimerHandler))
  {
    lastMillis = millis();
    Serial.println("Starting  ITimer OK, millis() = " + String(lastMillis));
  }
  else
    Serial.println("Can't set ITimer correctly. Select another freq. or interval");

  // Just to demonstrate, don't use too many ISR Timers if not absolutely necessary
  // You can use up to 16 timer for each ISR_Timer
  Teensy_ISR_Timer.setInterval(TIMER_INTERVAL_2S, doingSomething2s);
  Teensy_ISR_Timer.setInterval(TIMER_INTERVAL_5S, doingSomething5s);
  Teensy_ISR_Timer.setInterval(TIMER_INTERVAL_11S, doingSomething11s);
}

void loop()
{
  // put your main code here, to run repeatedly:
  delay(100);
  Serial.println(wheel_freq[0]);
}