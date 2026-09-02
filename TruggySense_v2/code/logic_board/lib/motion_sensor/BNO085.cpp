/**
 * @file BNO085.cpp
 * @author Robbe Elsermans (creator)
 * @author Thomas Kramp (revisor)
 * @brief Implementation of BNO085.h
 * @version 2.0
 * @date 2026-05-25
 * 
 * @copyright Copyright (c) 2025 Robbe Elsermans, All rights reserved.
 * This project is released under the CC BY-NC-SA licence.
 * See licence.txt for more information.
 */
#include "BNO085.h"

// #define PRINTDEBUG

bool BNO085::init() {
    if (!_bno085.begin_I2C()) {
    //if (!_bno085.begin_UART(&Serial1)) {  // Requires a device with > 300 byte UART buffer!
    //if (!_bno085.begin_SPI(BNO08X_CS, BNO08X_INT)) {
        setError(ModuleError::INIT_FAILED);
        return false;
    }
    if (!setReports()) {
        setError(ModuleError::REPORT_FAILED);
        return false;
    }

    return true;
}

bool BNO085::setReports() {
    #ifdef PRINTDEBUG
        debugSerial.println("\tSetting desired reports");
    #endif
    if (!_bno085.enableReport(_reportType, _reportInterval)) {
        #ifdef PRINTDEBUG
            debugSerial.println("\tCould not enable stabilized remote vector");
        #endif
        
        return false;
    }
    return true;
}

void BNO085::quaternionToEuler(const float qr, const float qi, const float qj, const float qk, const bool degrees) {
    // Store raw quaternion
    _q0 = qr;
    _q1 = qi;
    _q2 = qj;
    _q3 = qk;

    // Local squared terms for Euler calculation
    float sqr = sq(qr);
    float sqi = sq(qi);
    float sqj = sq(qj);
    float sqk = sq(qk);

    // Gimbal Lock counter:
    // float sinp = constrain(-2.0f * (qi * qk - qj * qr) / (sqi + sqj + sqk + sqr), -1.0f, 1.0f);
    // _pitch = asin(sinp);

    _yaw   = atan2(2.0 * (qi * qj + qk * qr),  (sqi - sqj - sqk + sqr));
    _pitch = asin(-2.0 * (qi * qk - qj * qr) / (sqi + sqj + sqk + sqr));
    _roll  = atan2(2.0 * (qj * qk + qi * qr), (-sqi - sqj + sqk + sqr));

    if (degrees) {
        _yaw   *= RAD_TO_DEG;
        _pitch *= RAD_TO_DEG;
        _roll  *= RAD_TO_DEG;
    }
}

void BNO085::read() {
    if (_bno085.wasReset()) {
        #ifdef PRINTDEBUG
            debugSerial.print("\tSensor was reset ");
        #endif
        setReports();
    }

    if (_bno085.getSensorEvent(&_sensorValue)) {
        // in this demo only one report type will be received depending on FAST_MODE define (above)
        _q0 = _sensorValue.un.arvrStabilizedRV.real;
        _q1 = _sensorValue.un.arvrStabilizedRV.i;
        _q2 = _sensorValue.un.arvrStabilizedRV.j;
        _q3 = _sensorValue.un.arvrStabilizedRV.k;
        quaternionToEuler(_sensorValue.un.arvrStabilizedRV.real, _sensorValue.un.arvrStabilizedRV.i, _sensorValue.un.arvrStabilizedRV.j, _sensorValue.un.arvrStabilizedRV.k, true);
    
        #ifdef PRINTDEBUG
            // debugSerial.println("\tQ0 (w):\t" + String(getQuaternionW()) + "\tQ1 (i):\t" + String(getQuaternionI()) 
            //                   + "\tQ2 (j):\t" + String(getQuaternionJ()) + "\tQ3 (k):\t" + String(getQuaternionK()));
            debugSerial.print("\tStatus:\t" + String(_sensorValue.status));
            debugSerial.println("\tRoll:\t" + String(getRoll()) + "°/s\tPitch:\t" + String(getPitch()) + "°/s\tYaw:\t" + String(getYaw()) + "°/s");
        #endif
    }
}

float BNO085::getRoll()  { return _roll; };
float BNO085::getPitch() { return _pitch; };
float BNO085::getYaw()   { return _yaw; };

float BNO085::getQuaternionR() { return _q0; };
float BNO085::getQuaternionI() { return _q1; };
float BNO085::getQuaternionJ() { return _q2; };
float BNO085::getQuaternionK() { return _q3; };