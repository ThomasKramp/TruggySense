/**
 * @file sd_logger.h
 * @author Robbe Elsermans
 * @brief Class that uses SdFat.h to log logging_data_t struct in binary format to a local SD-card
 * @version 0.1
 * @date 2025-04-28
 * 
 * @copyright Copyright (c) 2025 Robbe Elsermans, All rights reserved.
 * This project is released under the CC BY-NC-SA licence.
 * See licence.txt for more information
 * 
 */
#pragma once

#include "SdFat.h"

struct logging_data
{
    uint32_t timestamp;     // The timestamp of the data
    uint32_t date;          // The date aquired from a satellite
    uint32_t time;          // The time aquired from a satellite
    float t_llc;        // Temperature of low level controller
    float t_hlc;        // Temperature of high level controller
    float t_bp;         // Temperature of battery pack
    float t_ss;         // Temperature of steering servo motor
    float t_bm_1;       // Temperature of brushless motor 1
    float t_bm_2;       // Temperature of brushless motor 2
    float t_bm_3;       // Temperature of brushless motor 3
    float t_bm_4;       // Temperature of brushless motor 4
    float t_esc_1;      // Temperature of Electric Speed Controller 1
    float t_esc_2;      // Temperature of Electric Speed Controller 2
    float t_esc_3;      // Temperature of Electric Speed Controller 3
    float t_esc_4;      // Temperature of Electric Speed Controller 4
    float i_ss;         // Current usgae of steering servo
    float i_llc;        // Current usage of low level controller
    float i_hlc;        // Current usage of high level controller
    float roll;         // roll defines rolling
    float pitch;        // pitch defines nose up or down
    float yaw;          // yaw defines left right swing
    float w;            // real part of quaternion
    float i;            // i part
    float j;            // j part
    float k;            // k part
    float rpm_wheel_1;  // The RPM of wheel 1
    float rpm_wheel_2;  // The RPM of wheel 2
    float rpm_wheel_3;  // The RPM of wheel 3
    float rpm_wheel_4;  // The RPM of wheel 4
    float rpm_bm_1;     // The RPM of motor 1
    float rpm_bm_2;     // The RPM of motor 2
    float rpm_bm_3;     // The RPM of motor 3
    float rpm_bm_4;     // The RPM of motor 4
    float latitude;     // The latitude
    float longitude;    // the longitide
    float velocity;     // the relative velocity
    float altitude;     // the altitude based on received satellites distances
    float v_cell_1;     // Battery cell voltage 1.
    float v_cell_2;     // Battery cell voltage 2.
    float v_cell_3;     // Battery cell voltage 3.
    float v_cell_4;     // Battery cell voltage 4.
    float v_batt;       // Battery voltage.
    float v_llc;        // VOltage of low level controllers power supply.
    float v_hlc;        // Voltage of high level controllers power supply.
    float v_phrp_3v3;   // Voltage of pheripherals that use 3.3V
    uint16_t ch_1;      // Channel 1 that ranges from 1000 to 2000 (ms)
    uint16_t ch_2;      // Channel 2 that ranges from 1000 to 2000 (ms)
    uint16_t ch_3;      // Channel 3 that ranges from 1000 to 2000 (ms)
    uint16_t ch_4;      // Channel 4 that ranges from 1000 to 2000 (ms)
    uint16_t ch_5;      // Channel 5 that ranges from 1000 to 2000 (ms)
    uint16_t ch_6;      // Channel 6 that ranges from 1000 to 2000 (ms)
    uint16_t ch_7;      // Channel 7 that ranges from 1000 to 2000 (ms)
    uint16_t ch_8;      // Channel 8 that ranges from 1000 to 2000 (ms)
    uint16_t ch_9;      // Channel 9 that ranges from 1000 to 2000 (ms)
    uint16_t ch_10;     // Channel 10 that ranges from 1000 to 2000 (ms)
    uint16_t ch_11;     // Channel 11 that ranges from 1000 to 2000 (ms)
    uint16_t ch_12;     // Channel 12 that ranges from 1000 to 2000 (ms)
    uint16_t ch_13;     // Channel 13 that ranges from 1000 to 2000 (ms)
    uint16_t ch_14;     // Channel 14 that ranges from 1000 to 2000 (ms)
    uint16_t pwm_bm_1;  // PWM value from 1000 to 2000 (ms) for brushless motor 1
    uint16_t pwm_bm_2;  // PWM value from 1000 to 2000 (ms) for brushless motor 2
    uint16_t pwm_bm_3;  // PWM value from 1000 to 2000 (ms) for brushless motor 3
    uint16_t pwm_bm_4;  // PWM value from 1000 to 2000 (ms) for brushless motor 4
    uint16_t pwm_ss;    // PWM value from 1000 to 2000 (ms) for steering servo
    uint8_t c_s;        // Connected satellites
    uint8_t failsafe;   // when failsafe is active or not. 0 if not active, 1 if active.
    uint8_t dead_switch;// A deadswitch on the transmitter side if something goes wrong
    uint8_t i_esc_1;    // Current usage of esc 1
    uint8_t i_esc_2;    // Current usage of esc 2
    uint8_t i_esc_3;    // Current usage of esc 3
    uint8_t i_esc_4;    // Current usage of esc 4
};
typedef struct logging_data logging_data_t;

#define BASE_FILE_NAME "llc_data_0.bin"

// const uint8_t SD_CS_PIN = SS;
#define SD_CONFIG SdioConfig(FIFO_SDIO)

class SDLogger
{
private:
    SdFs sd;
    FsFile file;
    String filename;

    uint8_t writeBinData(logging_data_t bin_data);
    uint8_t writeBinData(uint8_t * bin_data);

public:
    //Find a correct file name without overwriting other existing files.
    //It uses a sequence number that keeps increasing.
    String generateUniqueFilenameAndUse();

    // Setup the connection and test if the SD-card is accessible
    // Creates a file for this session.
    // Return 1 if success and 0 if not.
    uint8_t init();

    // // This creates a file,
    // // if it exists, it creates a file with a number higher then the previous.
    // uint8_t createFile();

    /**
     * Get the currently used filename.
     */
    String getFileName();

    // Returns the current file size in bytes.
    uint64_t getFileSize();

    /**
     * Write data to the SD-card with a struct of logging_data_t.
     */
    // uint8_t writeDataToSdCard(logging_data_t data);

    /**
     * Write data to the internal file with a struct of logging_data_t.
     */
    uint8_t logData(logging_data_t data);

    /**
     * save data to SD-card
     */
    // uint8_t saveData();

    uint8_t closeFile();
};