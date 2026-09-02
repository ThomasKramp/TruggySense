/**
 * @file motor.cpp
 * @author Robbe Elsermans (creator)
 * @author Thomas Kramp (revisor)
 * @brief Implementation of motor.h
 * @version 2.0
 * @date 2026-05-25
 * 
 * @copyright Copyright (c) 2025 Robbe Elsermans, All rights reserved.
 * This project is released under the CC BY-NC-SA licence.
 * See licence.txt for more information.
 */
#include "motor.h"

// #define PRINTDEBUG

bool Motor::init() {
    if (!_motor.attach(_pin)) {
        setError(ModuleError::INIT_FAILED);
        return false;
    }
    _motor.write(_neutral);
    _disabled = false;
    return true;
}

void Motor::write(const uint16_t value, const bool invert_logic) {
    if (getLastError() == ModuleError::INIT_FAILED) {
        #ifdef PRINTDEBUG
            debugSerial.println("\t" + getErrorReport());
        #endif
        return;
    }

    // Don't do anything if the motor is disabled
    if (_disabled)  return;

    float normalized = (value - 1500) / 500.0f; // Map value to -1.0 ... +1.0
    float output = _neutral + (normalized * _range * (invert_logic ? -1.0f : 1.0f));
    
    _motor.write(output);

    #ifdef PRINTDEBUG
        debugSerial.println("\tRaw value: " + String(value) + "\tNormalized value:\t" + String(normalized));
        debugSerial.println("\tOutput: " + String(output));
    #endif
}
void Motor::write(const uint16_t throttle, const uint16_t value) {
    if (getLastError() == ModuleError::INIT_FAILED) {
        #ifdef PRINTDEBUG
            debugSerial.println("\t" + getErrorReport());
        #endif
        return;
    }

    // Don't do anything if the motor is disabled
    if (_disabled)  return;

    float throttle_factor = (throttle - 1000) / 1000.0f;
    float normalized = (value - 1500) / 500.0f; // Map value to -1.0 ... +1.0
    float output = _neutral + (normalized * _range * throttle_factor);

    _motor.write(output); 

    #ifdef PRINTDEBUG
        debugSerial.println("\tRaw throttle: " + String(throttle) + "\tThrottle factor:\t" + String(throttle_factor));
        debugSerial.println("\tRaw value: " + String(value) + "\tNormalized value:\t" + String(normalized));
        debugSerial.println("\tOutput: " + String(output));
    #endif
}

void Motor::disable(const uint16_t value) {
    // Kill switches of the FS-X6B have the value 1000 if OFF and 2000 if ON.
    if (value < 1500 && !_disabled) {
        _motor.write(_neutral);
        _disabled = true;
        setError(ModuleError::DISABLED);
    } else if (value > 1500 && _disabled) {
        _disabled = false;
        setError(ModuleError::OK);
    }
}
void Motor::disable(const float v_cell_1, const float v_cell_2, const float v_cell_3, const float v_cell_4) {
    if (v_cell_1 <= 3.2f ||
        v_cell_2 - v_cell_1 <= 3.2f ||
        v_cell_3 - v_cell_2 <= 3.2f ||
        v_cell_4 - v_cell_3 <= 3.2f ) {
        _motor.write(_neutral);
        _disabled = true;
        setError(ModuleError::EMPTY_BATTERY);
    }
}
bool Motor::isDisabled() {
    return _disabled;
}