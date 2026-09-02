/**
 * @file U-Blox.cpp
 * @author Robbe Elsermans (creator)
 * @author Thomas Kramp (revisor)
 * @brief Implementation of U-Blox.h
 * @version 2.0
 * @date 2026-05-25
 * 
 * @copyright Copyright (c) 2025 Robbe Elsermans, All rights reserved.
 * This project is released under the CC BY-NC-SA licence.
 * See licence.txt for more information.
 */#include "U-Blox.h"

#define PRINTDEBUG

void U_BLOX::sendUBX(const byte *cmd, const uint16_t len) {
    _serial->write(cmd, len);
    _serial->flush();
}

bool U_BLOX::init() {
    _serial->begin(9600);
    delay(500);

    #ifdef PRINTDEBUG
        debugSerial.println("Disabling unused NMEA sentences...");
    #endif
    sendUBX(DIS_GLL, sizeof(DIS_GLL));
    sendUBX(DIS_GSA, sizeof(DIS_GSA));
    sendUBX(DIS_GSV, sizeof(DIS_GSV));
    sendUBX(DIS_VTG, sizeof(DIS_VTG));

    #ifdef PRINTDEBUG
        debugSerial.println("Setting refresh rate");
    #endif
    sendUBX(UBX_10HZ, sizeof(UBX_10HZ));

    #ifdef PRINTDEBUG
        debugSerial.println("Setting baudrate");
    #endif
    sendUBX(UBX_BAUD, sizeof(UBX_BAUD));
    delayMicroseconds(2000);
    delay(5); // let module finish switching before we follow

    _serial->begin(115200); // Teensy follows the module

    #ifdef PRINTDEBUG
        debugSerial.println("=== Ready ===");
    #endif

    // Get a signal
    float latitude = 0, longitude = 0;

    while ((latitude == 0 || longitude == 0)) {// && millis() < 600000) {
        byte gpsData;
        while (_serial->available() > 0){
            gpsData =_serial->read();
            _decoder.encode(gpsData);
            #ifdef PRINTDEBUG
                if (_decoder.location.isUpdated()){
                    latitude = getLatitude();
                    longitude = getLongitude();
                    debugSerial.println();

                    debugSerial.print("\tPeriod: " + String(millis()) + "\t");
                    debugSerial.print("\tTime: " + String((_decoder.time.hour() + _timeZoneOffset + 24) % 24) 
                                    + ":" + String(_decoder.time.minute()) 
                                    + ":" + String(_decoder.time.second()));
                    debugSerial.println(" " + String(_decoder.date.day()) 
                                    + "/" + String(_decoder.date.month()) 
                                    + "/" + String(_decoder.date.year()));
                    debugSerial.print("\tSatellites: " + String(_decoder.satellites.value()) + "\t");
                    debugSerial.print("\tLatitude: " + String(getLatitude(), 6));
                    debugSerial.println("\tLongitude: " + String(getLongitude(), 6));
                } else {
                    debugSerial.write(gpsData);
                }
            #endif
        }
    }
    
    if (latitude == 0 || longitude == 0) {
        setError(ModuleError::INIT_FAILED);
        return false;
    }

    return true;
}

void U_BLOX::read() {
    if (getLastError() == ModuleError::INIT_FAILED) {
        #ifdef PRINTDEBUG
            //interrupts_enable();
            debugSerial.println("\tCouldn't connect");
            //interrupts_disable();
        #endif
        return;
    }

    // while (_serial->available() > 0) {
    //     if (_decoder.encode(_serial->read())) {
    //         if (millis() > 5000 && _decoder.charsProcessed() < 10) {
    //             return;
    //         }
    //     }
    // }

    byte gpsData;
    while (_serial->available() > 0){
        gpsData = _serial->read();
        _decoder.encode(gpsData);
        #ifdef PRINTDEBUG
            //interrupts_enable();
            if (_decoder.location.isUpdated()){
                debugSerial.println();

                // debugSerial.print("\tTime: " + String((_decoder.time.hour() + _timeZoneOffset + 24) % 24) 
                //                 + ":" + String(_decoder.time.minute()) 
                //                 + ":" + String(_decoder.time.second()));
                // debugSerial.println(" " + String(_decoder.date.day()) 
                //                   + "/" + String(_decoder.date.month()) 
                //                   + "/" + String(_decoder.date.year()));

                debugSerial.print("\tSatellites: " + String(_decoder.satellites.value()) + "\t");
                debugSerial.print("\tLatitude: " + String(getLatitude(), 6));
                debugSerial.println("\tLongitude: " + String(getLongitude(), 6));
            } else {
                debugSerial.write(gpsData);
            }
            //interrupts_disable();
        #endif
    }
}

long    U_BLOX::getDate() {
    if (_decoder.date.isValid())
        return _decoder.date.value();
    return 0;};
long    U_BLOX::getTime() {
    if (_decoder.time.isValid()) {
        // _decoder.time.value(); returns a uint32_t containing HH:MM:SS:cc
        // The extra +24 is in case of a negative offset
        // uint32_t localTime =
        //     ((_decoder.time.hour() + _timeZoneOffset + 24) % 24) * 1000000 +
        //       _decoder.time.minute() * 10000 +
        //       _decoder.time.second() * 100 +
        //       _decoder.time.centisecond();

        // return localTime;
        return _decoder.time.value();
    }
    return 0;
};
float   U_BLOX::getLatitude() {
    if (_decoder.location.isValid() && getSatellites() > MINIMUM_SATELLITES)
        return _decoder.location.lat();
    return 0;
};
float   U_BLOX::getLongitude() {
    if (_decoder.location.isValid() && getSatellites() > MINIMUM_SATELLITES)
        return _decoder.location.lng();
    return 0;
};
float   U_BLOX::getVelocity() {
    if (_decoder.speed.isValid() && getSatellites() > MINIMUM_SATELLITES)
        return _decoder.speed.kmph();
    return 0;
};
float   U_BLOX::getAltitude() {
    if (_decoder.altitude.isValid() && getSatellites() > MINIMUM_SATELLITES)
        return _decoder.altitude.meters();
    return 0;
};
uint8_t U_BLOX::getSatellites() {
    if (_decoder.satellites.isValid())
        return _decoder.satellites.value();
    return 0;
};