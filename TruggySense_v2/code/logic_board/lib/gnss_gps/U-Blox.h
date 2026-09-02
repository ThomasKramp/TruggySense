/**
 * @file U-Blox.h
 * @author Robbe Elsermans (creator)
 * @author Thomas Kramp (revisor)
 * @brief U-Blox module for fetching GNSS/GPS data.
 * @version 2.0
 * @date 2026-05-25
 * 
 * @copyright Copyright (c) 2025 Robbe Elsermans, All rights reserved.
 * This project is released under the CC BY-NC-SA licence.
 * See licence.txt for more information.
 */
#ifndef U_BLOX_H_
#define U_BLOX_H_

#pragma once

#include <TinyGPS++.h>

#include "module.h"

#define MINIMUM_SATELLITES 3   /** @brief Minimum number of satellites required for a valid GPS fix. */

class U_BLOX : public Module {
    private:
        HardwareSerial *_serial;  /** @brief Serial interface used to receive IBUS frames. */
        TinyGPSPlus     _decoder; /** @brief TinyGPS++ decoder for parsing NMEA sentences */
        
        int8_t _timeZoneOffset; /** @brief Time zone offset in hours; base time is set to UTC */

        /** @brief UBX CFG-PRT command: Switch baud rate to 115200. */
        const byte UBX_BAUD[28] = {
            0xB5, 0x62, 0x06, 0x00, 0x14, 0x00,
            0x01, 0x00, 0x00, 0x00,
            0xD0, 0x08, 0x00, 0x00,
            0x00, 0xC2, 0x01, 0x00,  // 115200 baud
            0x07, 0x00, 0x03, 0x00,
            0x00, 0x00, 0x00, 0x00,
            0xC0, 0x7E 
        };

        /** @brief UBX CFG-RATE command: Set update rate to 10Hz. */
        const byte UBX_10HZ[14] = {0xB5,0x62,0x06,0x08,0x06,0x00,0x64,0x00,0x01,0x00,0x01,0x00,0x7A,0x12};

        /** @brief UBX CFG-MSG command: Disable GLL sentence. */
        const byte DIS_GLL[16] = {0xB5,0x62,0x06,0x01,0x08,0x00,0xF0,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x2A};
        /** @brief UBX CFG-MSG command: Disable GSA sentence. */
        const byte DIS_GSA[16] = {0xB5,0x62,0x06,0x01,0x08,0x00,0xF0,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x31};
        /** @brief UBX CFG-MSG command: Disable GSV sentence. */
        const byte DIS_GSV[16] = {0xB5,0x62,0x06,0x01,0x08,0x00,0xF0,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x38};
        /** @brief UBX CFG-MSG command: Disable VTG sentence. */
        const byte DIS_VTG[16] = {0xB5,0x62,0x06,0x01,0x08,0x00,0xF0,0x05,0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x46};

        /** 
         * @brief Sends a UBX command to the GNSS module.
         * @param cmd Command byte array.
         * @param len Length of the command in bytes.
         */
        void sendUBX(const byte *cmd, const uint16_t len);

    public:
        /**
         * @brief Constructor for the U_BLOX class.
         * @param serial The serial interface.
         * @param _timeZoneOffset Time zone offset in hours (default: 0 UTC)
         * */
        U_BLOX(HardwareSerial &serial, const int8_t _timeZoneOffset = 0)
        : Module("NEO-6M - GNSS/GPS"),
          _serial(&serial),
          _timeZoneOffset(_timeZoneOffset) {}

        /**
         * @brief Initializes the U-Blox module.
         * @return true if initialization was successful, false otherwise.
         * @note For the GPS to collect enough data, its speed has to increase from 1Hz to 10Hz.
         * @note For the module to be able to communicate it needs to receive full messages between the fastest interrupt of the schedular.
         * Thus the baudrate is switched to 115200.
         * Besides that, the unused NMEA sentences are disabled to reduce the amount of data that needs to be send.
         */
        bool init() override;

        /** 
         * @brief Reads and decodes GNSS/GPS data. 
         * @note This method will get stuck if there is no GPS signal.
        */
        void read();

        /** 
         * @brief Retrieves the current date.
         * @return Date as DDMMYY format.
         */
        long    getDate();
        /** 
         * @brief Retrieves the current time.
         * @return Time as HHMMSS format.
         */
        long    getTime();
        /** 
         * @brief Retrieves the current latitude.
         * @return Latitude in decimal degrees.
         */
        float   getLatitude();
        /** 
         * @brief Retrieves the current longitude.
         * @return Longitude in decimal degrees.
         */
        float   getLongitude();
        /** 
         * @brief Retrieves the current velocity.
         * @return Velocity in km/h.
         */
        float   getVelocity();
        /** 
         * @brief Retrieves the current altitude.
         * @return Altitude in meters.
         */
        float   getAltitude();
        /** 
         * @brief Retrieves the number of satellites in view.
         * @return Number of satellites.
         */
        uint8_t getSatellites();
};

#endif