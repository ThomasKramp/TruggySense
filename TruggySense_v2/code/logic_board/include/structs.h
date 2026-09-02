/**
 * @file structs.h
 * @author Robbe Elsermans (creator)
 * @author Thomas Kramp (revisor)
 * @brief Data structures for the TruggySense project.
 * @version 2.0
 * @date 2026-05-25
 * 
 * @copyright Copyright (c) 2025 Robbe Elsermans, All rights reserved.
 * This project is released under the CC BY-NC-SA licence.
 * See licence.txt for more information.
 */
#ifndef STRUCTS_H_
#define STRUCTS_H_

#pragma once

#include <stdint.h>

/** @brief datastruct of localization data. */
struct localization {
    long date;                    /** @brief The date acquired from a satellite DDMMYY. */
    long time;                    /** @brief The time acquired from a satellite in HHMMSSmm. */
    float latitude;               /** @brief The latitude. */
    float longitude;              /** @brief The longitude. */
    float velocity;               /** @brief The relative velocity. */
    float altitude;               /** @brief The altitude based on received satellites distances. */
    uint8_t connected_satellites; /** @brief The number of connected satellites. */
};
typedef struct localization localization_t;

/** @brief datastruct of radio channels. */
struct radio {
    uint16_t ch_1;  /** @brief Channel 1 that ranges from 1000 to 2000 (ms). */
    uint16_t ch_2;  /** @brief Channel 2 that ranges from 1000 to 2000 (ms). */
    uint16_t ch_3;  /** @brief Channel 3 that ranges from 1000 to 2000 (ms). */
    uint16_t ch_4;  /** @brief Channel 4 that ranges from 1000 to 2000 (ms). */
    uint16_t ch_5;  /** @brief Channel 5 that ranges from 1000 to 2000 (ms). */
    uint16_t ch_6;  /** @brief Channel 6 that ranges from 1000 to 2000 (ms). */
    uint16_t ch_7;  /** @brief Channel 7 that ranges from 1000 to 2000 (ms). */
    uint16_t ch_8;  /** @brief Channel 8 that ranges from 1000 to 2000 (ms). */
    uint16_t ch_9;  /** @brief Channel 9 that ranges from 1000 to 2000 (ms). */
    uint16_t ch_10; /** @brief Channel 10 that ranges from 1000 to 2000 (ms). */
    bool failsafe;  /** @brief When failsafe is active or not. */
};
typedef struct radio radio_t;

/** @brief datastruct of encoder measurement data. */
struct encoder {
    float rpm_wheel_1; /** @brief The RPM of wheel 1. */
    float rpm_wheel_2; /** @brief The RPM of wheel 2. */
    float rpm_wheel_3; /** @brief The RPM of wheel 3. */
    float rpm_wheel_4; /** @brief The RPM of wheel 4. */
};
typedef struct encoder encoder_t;

/** @brief datastruct of voltage measurement data. */
struct voltage {
    float v_cell_1;   /** @brief Battery cell voltage 1. */
    float v_cell_2;   /** @brief Battery cell voltage 2. */
    float v_cell_3;   /** @brief Battery cell voltage 3. */
    float v_cell_4;   /** @brief Battery cell voltage 4. */
    float v_rail_3v3; /** @brief Voltage of pheripherals that use 3.3V */
    float v_rail_5v0; /** @brief Voltage of pheripherals that use 5.0V */
};
typedef struct voltage voltage_t;

/** @brief datastruct of current measurement data. */
struct current {
    float i_esc_1; /** @brief Current usage of esc. 1*/
    float i_esc_2; /** @brief Current usage of esc. 2*/
    float i_esc_3; /** @brief Current usage of esc. 3*/
    float i_esc_4; /** @brief Current usage of esc. 4*/
    float i_etc;   /** @brief Current usage of steering servo, FS-X6B and other 5V modules. */
    float i_llc;   /** @brief Current usage of low level controller and the 3.3V modules. */
    float i_hlc;   /** @brief Current usage of high level controller. */
};
typedef struct current current_t;

/** @brief datastruct of sensed temperatures. */
struct temperature {
    float t_llc;   /** @brief Temperature of low level controller. */
    float t_hlc;   /** @brief Temperature of high level controller. */
    float t_bp;    /** @brief Temperature of battery pack. */
    float t_ss;    /** @brief Temperature of steering servo motor. */
    float t_bm_1;  /** @brief Temperature of brushless motor 1. */
    float t_bm_2;  /** @brief Temperature of brushless motor 2. */
    float t_bm_3;  /** @brief Temperature of brushless motor 3. */
    float t_bm_4;  /** @brief Temperature of brushless motor 4. */
    float t_esc_1; /** @brief Temperature of Electric Speed Controller 1. */
    float t_esc_2; /** @brief Temperature of Electric Speed Controller 2. */
    float t_esc_3; /** @brief Temperature of Electric Speed Controller 3. */
    float t_esc_4; /** @brief Temperature of Electric Speed Controller 4. */
};
typedef struct temperature temperature_t;

/** @brief datastruct of IMU measurement data. */
struct movement {
    float roll;  /** @brief Roll defines rolling. */
    float pitch; /** @brief Pitch defines nose up or down. */
    float yaw;   /** @brief Yaw defines left right swing. */
    float w;     /** @brief Real part of quaternion. */
    float i;     /** @brief i part of quaternion. */
    float j;     /** @brief j part of quaternion. */
    float k;     /** @brief k part of quaternion. */
};
typedef struct movement movement_t;

struct logging_data {
    uint32_t timestamp  = 0; /** @brief The timestamp of the data. */
    // GPS data:
    uint32_t date       = 0; /** @brief The date acquired from a satellite. */
    uint32_t time       = 0; /** @brief The time acquired from a satellite. */
    float latitude      = 0; /** @brief The latitude. */
    float longitude     = 0; /** @brief The longitude. */
    float velocity      = 0; /** @brief The relative velocity. */
    float altitude      = 0; /** @brief The altitude based on received satellites distance. */
    uint8_t c_s         = 0; /** @brief Connected satellites. */
    // Remote control data:
    uint16_t ch_1       = 0; /** @brief Channel 1 that ranges from 1000 to 2000 (ms). */
    uint16_t ch_2       = 0; /** @brief Channel 2 that ranges from 1000 to 2000 (ms). */
    uint16_t ch_3       = 0; /** @brief Channel 3 that ranges from 1000 to 2000 (ms). */
    uint16_t ch_4       = 0; /** @brief Channel 4 that ranges from 1000 to 2000 (ms). */
    uint16_t ch_5       = 0; /** @brief Channel 5 that ranges from 1000 to 2000 (ms). */
    uint16_t ch_6       = 0; /** @brief Channel 6 that ranges from 1000 to 2000 (ms). */
    uint16_t ch_7       = 0; /** @brief Channel 7 that ranges from 1000 to 2000 (ms). */
    uint16_t ch_8       = 0; /** @brief Channel 8 that ranges from 1000 to 2000 (ms). */
    uint16_t ch_9       = 0; /** @brief Channel 9 that ranges from 1000 to 2000 (ms). */
    uint16_t ch_10      = 0; /** @brief Channel 10 that ranges from 1000 to 2000 (ms). */
    bool failsafe       = 0; /** @brief When failsafe is active or not. */
    // Encoder data:
    float rpm_wheel_1   = 0; /** @brief The RPM of wheel 1. */
    float rpm_wheel_2   = 0; /** @brief The RPM of wheel 2. */
    float rpm_wheel_3   = 0; /** @brief The RPM of wheel 3. */
    float rpm_wheel_4   = 0; /** @brief The RPM of wheel 4. */
    // Voltage data:
    float v_cell_1      = 0; /** @brief Battery cell voltage 1. */
    float v_cell_2      = 0; /** @brief Battery cell voltage 2. */
    float v_cell_3      = 0; /** @brief Battery cell voltage 3. */
    float v_cell_4      = 0; /** @brief Battery cell voltage 4. */
    float v_rail_3V3    = 0; /** @brief Voltage of low level controllers power supply. */
    float v_rail_5v0    = 0; /** @brief Voltage of high level controllers power supply. */
    // Current data:
    float i_esc_1       = 0; /** @brief Current usage of esc 1. */
    float i_esc_2       = 0; /** @brief Current usage of esc 2. */
    float i_esc_3       = 0; /** @brief Current usage of esc 3. */
    float i_esc_4       = 0; /** @brief Current usage of esc 4. */
    float i_etc         = 0; /** @brief Current usage of steering servo. */
    float i_llc         = 0; /** @brief Current usage of low level controller. */
    float i_hlc         = 0; /** @brief Current usage of high level controller. */
    // Temperature data:
    float t_llc         = 0; /** @brief Temperature of low level controller. */
    float t_hlc         = 0; /** @brief Temperature of high level controller. */
    float t_bp          = 0; /** @brief Temperature of battery pack. */
    float t_ss          = 0; /** @brief Temperature of steering servo motor. */
    float t_bm_1        = 0; /** @brief Temperature of brushless motor 1. */
    float t_bm_2        = 0; /** @brief Temperature of brushless motor 2. */
    float t_bm_3        = 0; /** @brief Temperature of brushless motor 3. */
    float t_bm_4        = 0; /** @brief Temperature of brushless motor 4. */
    float t_esc_1       = 0; /** @brief Temperature of Electric Speed Controller 1. */
    float t_esc_2       = 0; /** @brief Temperature of Electric Speed Controller 2. */
    float t_esc_3       = 0; /** @brief Temperature of Electric Speed Controller 3. */
    float t_esc_4       = 0; /** @brief Temperature of Electric Speed Controller 4. */
    // Movement data:
    float roll          = 0; /** @brief Roll defines rolling. */
    float pitch         = 0; /** @brief Pitch defines nose up or down. */
    float yaw           = 0; /** @brief Yaw defines left right swing. */
    float w             = 0; /** @brief Real part of quaternion. */
    float i             = 0; /** @brief i part. */
    float j             = 0; /** @brief j part. */
    float k             = 0; /** @brief k part*/
};
typedef struct logging_data logging_data_t;

#endif