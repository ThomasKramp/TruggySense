#ifndef ACS758_H_
#define ACS758_H_

#include "Arduino.h"
#include "Motor_Position.h"

class ACS758 {
    public:
        const uint8_t pins[4] = { PIN_FR, PIN_FL, PIN_RR, PIN_RL };

        // Constructor
        ACS758(float_t vcc);

        // Public methods
        float_t    getCurrent(MOTOR_POSITION pos);

    private:
        // Enums
        enum PINS {
            PIN_FR = 22, //28,
            PIN_FL = 23, //29,
            PIN_RR = 38, //30,
            PIN_RL = 39  //31
        };

        // Private vars
        float_t raw_voltage;
        float_t avr_current;

        uint8_t prim_samp_curr = 200;
        float_t sensitivity = 0.010f;
        bool bidirectional = true;

        float_t supply_voltage;
};

#endif