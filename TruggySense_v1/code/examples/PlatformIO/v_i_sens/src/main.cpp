#include <Arduino.h>

//#define ADC_I_M_RR 27

#define ADC_I_SERVO 41

float delta_v = 0.00322;

void setup() {

  Serial.begin(9600);

  // pinMode(ADC_I_M_RR, INPUT);
  pinMode(ADC_I_SERVO, INPUT);

  pinMode(13, OUTPUT);
}

void loop() {
  int reading = analogRead(ADC_I_SERVO);
  Serial.println();
  Serial.println((reading*delta_v)/0.75);

  digitalWrite(13, !digitalRead(13));

  delay(200);
}