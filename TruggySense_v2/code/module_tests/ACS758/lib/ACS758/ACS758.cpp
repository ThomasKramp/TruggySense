#include "ACS758.h"

// Constructor
ACS758::ACS758(float_t vcc) {
    this->supply_voltage = vcc;

    analogReadResolution(12); // Can go to 12-bits
    analogReference(EXTERNAL);
}

//#define PRINTDEBUG 1

// Get functions
float_t ACS758::getCurrent(MOTOR_POSITION pos) {
    this->raw_voltage = (analogRead(this->pins[pos]) * this->supply_voltage) / 4095.0f;

    if (this->bidirectional) {
        this->avr_current = (this->raw_voltage - (supply_voltage / 2.0f)) / this->sensitivity;
    } else {
        this->avr_current = (this->raw_voltage - 0.6f) / this->sensitivity;
    }

    return this->avr_current;
}