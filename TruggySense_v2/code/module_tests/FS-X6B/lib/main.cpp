/*
 * FlySky Receiver
 *
 * This example shows ...
 * 
 * The circuit:
 * ** RX (IBUS) - 
 * 
 * Created  21 MAy 2020     by Ricardo Paiva
 * Modified 06 Mar 2026     by Thomas K.
 * 
 * This example code is in the public domain.
 * 
 * Source:  https://medium.com/@werneckpaiva/how-to-read-rc-signal-with-arduino-using-flysky-ibus-73448bc924eb
 * Video:
 *  - Receiver module:  https://www.youtube.com/watch?v=NuVQz7FCAZk
 *  - IBUS conversion:  https://www.youtube.com/watch?v=q80QijmQzOI
 */

#include "HardwareSerial.h"
#include "IBusBM.h"

HardwareSerial& ibusRcSerial = Serial8; // Check Teensy UART connections: https://www.pjrc.com/teensy/td_uart.html
IBusBM ibusRc;

void setup() {
  Serial.begin(115200);
  ibusRc.begin(ibusRcSerial);
}

// Read the number of a given channel and convert to the range provided.
// If the channel is off, return the default value
int readChannel(byte channelInput, int minLimit, int maxLimit, int defaultValue){
    uint16_t ch = ibusRc.readChannel(channelInput);
    if (ch < 100) return defaultValue;
    return map(ch, 1000, 2000, minLimit, maxLimit);
}

// Red the channel and return a boolean value
bool redSwitch(byte channelInput, bool defaultValue){
    int intDefaultValue = (defaultValue)? 100: 0;
    int ch = readChannel(channelInput, 0, 100, intDefaultValue);
    return (ch > 50);
}

void loop() {
    for (byte i = 0; i < 18; i++){
        int value = readChannel(i, -100, 100, 0);
        Serial.print("Ch");
        Serial.print(i + 1);
        Serial.print(": ");
        Serial.print(value);
        Serial.print(" ");
    }
    Serial.println();
    delay(10);
}