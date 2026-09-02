/**
 * PIM448 test sketch
 *
 * Tests the connection of the PIM448 module
 * by printing all the variables it senses.
 * 
 * Source: https://product.tdk.com/system/files/dam/doc/product/sensor/mortion-inertial/imu/data_sheet/ds-000189-icm-20948-v1.5.pdf
 */

#include "Arduino.h"
#include "PIM448.h"

#ifndef LED_BUILTIN
  #define LED_BUILTIN 13
#endif

PIM448 GyroAccel(PIM448::ADDRESS_1, PIM448::DPS_500, PIM448::G_4);

void setup() {
    Wire.begin();
    Serial.begin (115200);
    
    // Turn led on to indicate Ready state
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);

    // Wait for serial port to connect
    while (!Serial) { }

    // Config Register
    GyroAccel.setConfig();

    // Turn led off
    digitalWrite(LED_BUILTIN, LOW);
}

uint16_t counter = 1;
void loop() {
    Serial.print(counter);      Serial.print("\t");
    Serial.print("Receive:\t"); Serial.println(GyroAccel.getConfirmation() ? "true" : "false");

    Serial.print("\t\tTemperature:\t");
    Serial.print(GyroAccel.getTemperature());
    Serial.println("\t°C");

    Serial.print("\t\tGyro:");
    Serial.print("\tX: ");   Serial.print(GyroAccel.getGyrXDPS());
    Serial.print("\tY: ");   Serial.print(GyroAccel.getGyrYDPS());
    Serial.print("\tZ: ");   Serial.print(GyroAccel.getGyrZDPS());
    Serial.println("\tradians/s");

    Serial.print("\t\tAccel:");
    Serial.print("\tX: ");   Serial.print(GyroAccel.getAccXMG());
    Serial.print("\tY: ");   Serial.print(GyroAccel.getAccYMG());
    Serial.print("\tZ: ");   Serial.print(GyroAccel.getAccZMG());
    Serial.println("\tm/s²");
    counter += 1;
    delay(1000);
}