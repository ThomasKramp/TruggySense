/**
 * @file DS18B20.h
 * @author Robbe Elsermans (creator)
 * @author Thomas Kramp (revisor)
 * @brief DS18B20 temperature sensor module.
 * @version 2.0
 * @date 2026-05-25
 *
 * @copyright Copyright (c) 2025 Robbe Elsermans, All rights reserved.
 * This project is released under the CC BY-NC-SA licence.
 * See licence.txt for more information.
 */
#ifndef DS18B20_H_
#define DS18B20_H_

#pragma once

#include <Arduino.h>

#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 6
#define TEMPERATURE_PRECISION 10

#define HLC_AVAILABILITY (1 << 0)
#define LLC_AVAILABILITY (1 << 1)
#define SS_AVAILABILITY (1 << 2)
#define BP_AVAILABILITY (1 << 3)

#define BM_1_AVAILABILITY (1 << 0)
#define BM_2_AVAILABILITY (1 << 1)
#define BM_3_AVAILABILITY (1 << 2)
#define BM_4_AVAILABILITY (1 << 3)
#define ESC_1_AVAILABILITY (1 << 4)
#define ESC_2_AVAILABILITY (1 << 5)
#define ESC_3_AVAILABILITY (1 << 6)
#define ESC_4_AVAILABILITY (1 << 7)

class DS18B20 {
    private:
        float internal_temp;
        uint8_t sensor_availability_1;
        uint8_t sensor_availability_2;

        // Addresses acquired from "playground -> platformio -> temperatures"
        DeviceAddress hlc_thermometer = {0x28, 0x6E, 0x85, 0x71, 0x10, 0x0, 0x0, 0xBA};
        DeviceAddress ss_thermometer = {0x28, 0x7B, 0x47, 0x72, 0x10, 0x0, 0x0, 0x23};
        DeviceAddress esc_1_thermometer = {0x28, 0xDB, 0xC3, 0x71, 0x10, 0x0, 0x0, 0x23};
        DeviceAddress esc_2_thermometer = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
        DeviceAddress esc_3_thermometer = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
        DeviceAddress esc_4_thermometer = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
        DeviceAddress bm_1_thermometer = {0x28, 0xE7, 0x39, 0x72, 0x10, 0x0, 0x0, 0x20};
        DeviceAddress bm_2_thermometer = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
        DeviceAddress bm_3_thermometer = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
        DeviceAddress bm_4_thermometer = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
        DeviceAddress bp_thermometer = {0x28, 0xE5, 0x49, 0x72, 0x10, 0x00, 0x00, 0x23};

    public:
        /**
         * @brief Defining the different sources a temperature can be measured from.
         * 
         */
        enum temperature_source {
            hlc,   // High level controller
            llc,   // Low level controller
            ss,    // Steering Servo
            bp,    // Battery pack
            esc_1, // ESC 1
            esc_2, // ESC 2
            esc_3, // ESC 3
            esc_4, // ESC 4
            bm_1,  // Brushless motor 1
            bm_2,  // Brushless motor 2
            bm_3,  // Brushless motor 3
            bm_4,  // Brushless motor 4
        };

        /**
         * @brief initialize the sensors
         * @return uint8_t 1 on success and 0 on failure
         */
        uint8_t init();

        /** @brief read the temperatures from all devices */
        void read();

        /**
         * @brief Get the temperature By Source
         * @param source the selected temperature module
         * @return float containing the temperature. -127 means not available.
         */
        float getBySource(temperature_source source);
};
#endif