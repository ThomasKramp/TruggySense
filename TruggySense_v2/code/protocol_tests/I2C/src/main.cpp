/**
 * Scanner
 * 
 * Scans the I2C line for devices and prints their addresses.
 */

#include "Arduino.h"
#include "Wire.h"

#ifndef LED_BUILTIN
    #define LED_BUILTIN 13
#endif

void setup() {
    Wire.begin();
    Serial.begin (115200);

    // Turn led on to indicate Ready state
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);

    // Wait for serial port to connect
    while (!Serial) { }

    // Turn led off
    digitalWrite(LED_BUILTIN, LOW);
}

void loop() {
    // Start scanning
    Serial.println ();
    Serial.println ("I2C scanner. Scanning ...");
    byte count = 0;
    
    for (byte i = 1; i < 120; i++)
    {
        Wire.beginTransmission (i);
        if (Wire.endTransmission () == 0)
        {
        Serial.print ("Found address: ");
        Serial.print (i, DEC);
        Serial.print (" (0x");
        Serial.print (i, HEX);
        Serial.println (")");
        count++;
        delay (1);  // maybe unneeded?
        } // end of good response
    } // end of for loop
    Serial.println ("Done.");
    Serial.print ("Found ");
    Serial.print (count, DEC);
    Serial.println (" device(s).");

    delay(1000);
}