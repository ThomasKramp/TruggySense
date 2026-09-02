/**
 * @file gps.h
 * @author Robbe Elsermans
 * @brief driver which contains gps class that utilizes TinyGPSPlus library
 * @version 1.0
 * @date 2025-04-10
 *
 * @copyright Copyright (c) 2025 Robbe Elsermans, All rights reserved.
 * This project is released under the CC BY-NC-SA licence.
 * See licence.txt for more information
 *
 */
#pragma once
#include <Arduino.h>
#include <TinyGPSPlus.h>

/**
 * @brief Depends on the used module.
 *
 * @note Check https://www.u-blox.com/en/u-center-2 where the baudrate can be discovered throuh the app.
 */
#define BAUDRATE 115200

class GPSDriver
{
public:
    /**
     * @brief initialize gps module by checking connection
     *
     * @return uint8_t 1 if success, 0 if failure
     */
    uint8_t init();

    /**
     * @brief read serial string if available and pass it through tinyGPS
     *
     * @note must be updated frequently such that a full char sequence can be decoded by the TinyGPS++ library
     *
     * @return uint8_t 1 if success, 0 if no char sequences received withing 5 seconds
     */
    uint8_t readGps();

    /**
     * @brief Get the Velocity object
     *
     * @note Can only be valid if there is a 3D fix (>=4 satelites).
     *
     * @return uint8_t the velocity in meters per second (m/s), 0 if not acquired
     */
    uint8_t getVelocity();

    /**
     * @brief Get the Latitude object
     *
     * @note Can only be valid if there is a 3D fix (>=4 satelites).
     *
     * @return float the latitude in degrees (°), 0 if not acquired
     */
    float getLatitude();

    /**
     * @brief Get the Longitude object
     *
     * @note Can only be valid if there is a 3D fix (>=4 satelites).
     *
     * @return float the longitude in degrees (°), 0 if not acquired
     */
    float getLongitude();

    /**
     * @brief Get the Date object
     *
     * @return uint32_t in the from of DDMMYY, 0 if not acquired
     */
    uint32_t getDate();

    /**
     * @brief Get the Time object in UTC format
     *
     * @return uint32_t in the form HHMMSSmm, 0 if not acquired
     *
     */
    uint32_t getTime();

    /**
     * @brief Get the altitude based on received satellite information.
     *
     * @note Can only be valid if there is a 3D fix (>=4 satelites).
     *
     * @return int16_t the altitude in meters (m), 0 if not acquired
     */
    int16_t getAltitude();

    /**
     * @brief Get the number of received satellites
     *
     * @return uint8_t count of found satellites, 0 if not acquired
     */
    uint8_t getConnectedSatelites();
};
