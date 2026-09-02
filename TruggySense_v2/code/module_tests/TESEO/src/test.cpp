#include "Arduino.h"
#include "Wire.h"

#define _resetPin 32

void setup() {
    Serial.begin(115200);
    pinMode(_resetPin, OUTPUT);
    digitalWrite(_resetPin, LOW);
    
    // Wait for serial port to connect
    while (!Serial) { }
    digitalWrite(_resetPin, HIGH);

    Wire.begin();
    Wire.setClock(100000);

    Serial.println("(waiting 5 seconds for module to boot...)");
    delay(5000);
    Serial.println("Raw TESEO I2C dump — should see NMEA sentences:");
}

void loop() {
    uint8_t received = Wire.requestFrom((uint8_t)0x3A, (uint8_t)128);
    
    if (received == 0) {
        // Module stopped responding — reinitialise I2C
        Wire.end();
        delay(10);
        Wire.begin();
        Wire.setClock(100000);
        Serial.println("\n[I2C reinit]");
        delay(100);
        return;
    }

    for (uint8_t i = 0; i < received; i++) {
        uint8_t b = Wire.read();
        if (b != 0xFF) {          // skip dummy bytes
            Serial.write(b);      // print raw character
        }
    }
    delay(20);
}