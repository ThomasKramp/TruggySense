/**
 * @file gps.cpp
 * @author Robbe Elsermans
 * @brief Implementation of gps.h
 * @version 1
 * @date 2025-04-28
 * 
 * @copyright Copyright (c) 2025 Robbe Elsermans, All rights reserved.
 * This project is released under the CC BY-NC-SA licence.
 * See licence.txt for more information
 * 
 */
#include "gps.h"

TinyGPSPlus gps_object;

uint8_t GPSDriver::init()
{
    Serial2.begin(BAUDRATE);
    return 1;
}

uint8_t GPSDriver::readGps()
{
    while (Serial2.available() > 0)
    {
        if (gps_object.encode(Serial2.read()))
        {
            if (millis() > 5000 && gps_object.charsProcessed() < 10)
            {
                return 0;
            }
        }
    }
    return 1;
}

uint8_t GPSDriver::getVelocity()
{
    if (gps_object.speed.isValid() && GPSDriver::getConnectedSatelites() > 3)
    {
        return gps_object.speed.mps();
    }
    return 0;
}

float GPSDriver::getLatitude()
{
    if (gps_object.location.isValid() && GPSDriver::getConnectedSatelites() > 3)
    {
        return gps_object.location.lat();
    }
    return 0;
}

float GPSDriver::getLongitude()
{
    if (gps_object.location.isValid() && GPSDriver::getConnectedSatelites() > 3)
    {
        return gps_object.location.lng();
    }
    return 0;
}

uint32_t GPSDriver::getDate()
{
    if (gps_object.date.isValid())
    {
        return gps_object.date.value();
    }
    return 0;
}

uint32_t GPSDriver::getTime()
{
    if (gps_object.time.isValid())
    {
        return gps_object.time.value();
    }
    return 0;
}

int16_t GPSDriver::getAltitude()
{
    if (gps_object.altitude.isValid() && GPSDriver::getConnectedSatelites() > 3)
    {
        return gps_object.altitude.meters();
    }
    return 0;
}

uint8_t GPSDriver::getConnectedSatelites()
{
    if (gps_object.satellites.isValid())
    {
        return gps_object.satellites.value();
    }
    return 0;
}