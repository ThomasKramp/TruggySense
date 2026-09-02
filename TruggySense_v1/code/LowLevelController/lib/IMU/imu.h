/*!
 * @file imu.h
 * @author Robbe Elsermans
 * @brief A driver that utilize the Adafruit_BNO08x library to acquire Euler angle and Quaternions from a BNO085 IMU 9-axis sensor.
 * @version 1.0
 * @date 2025-04-10
 * 
 * @copyright Copyright (c) 2025 Robbe Elsermans, All rights reserved.
 * This project is released under the CC BY-NC-SA licence.
 * See licence.txt for more information
 * 
 */
#pragma once

#include <Adafruit_BNO08x.h>
#include <Wire.h>

/**
 * @brief An Euler struct containing all 3 Euler angles yaw, pitch and roll
 * 
 */
struct euler
{
    float yaw;
    float pitch;
    float roll;
};
typedef struct euler euler_t;

/**
 * @brief An Quaternion struct containing all 4 Quaternion values w, i, j, k
 * 
 */
struct quaternion // A 4D dimention
{
    float w;
    float i;
    float j;
    float k;
};
typedef struct quaternion quaternion_t;

#define BNO08X_RESET -1 //no reset is connected to it

/**
 * @brief Initialize, read and get the Euler or Quaternion specific angles given by e_imu_data from the BNO085 IMU 9-axis sensor.
 * 
 * @note This driver assumes the IMU is connected in the following way
 * ```
 *    MCU              IMU
 * VCC (3V3)    ->  VCC (3V3),
 * SDA (18)     ->  SDA,
 * SCL (19)     ->  SCL,
 * GND          ->  GND
 * ```
 */
class IMU
{
private:
    euler_t position_euler;
    quaternion_t position_quaternion;
    const sh2_SensorId_t report_type = SH2_ARVR_STABILIZED_RV;
    const long report_interval_us = 5000;
    sh2_SensorValue_t sensor_value;

    /**
     * @brief converts a Quaternion to Euler angles
     * 
     * @param qr real part of a Quaternion
     * @param qi imaginairy part 1
     * @param qj imaginairy part 2
     * @param qk imaginairy part 3
     * @param ypr The Euler euler_t struct pointer
     * @param degrees Set true if you want a degree Eulered angles. False if not.
     * 
     * See: https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles
     */
    void quaternionToEuler(float qr, float qi, float qj, float qk, euler_t *ypr, bool degrees = false);
    
    
    void setReports(sh2_SensorId_t report_type, long report_interval);

public:

    /**
     * @brief The data that can be acquired from this driver.
     * 
     */
    enum imu_data{
    roll, pitch, yaw, w, i, j, k
    };

    /**
     * @brief Initialize the IMU connection and test if the BNO085 sensor is found on the I2C bus.
     * 
     * @return uint8_t where 1 means correctly initialized and 0 means something went wrong.
     */
    uint8_t init();

    /**
     * @brief Let the BNO085 compute its angles and retrieve them in the local class properties.
     * 
     */
    void computeData();

    /**
     * @brief Get the Data By Source object
     * 
     * @param data_source containing the e_imu_data type definition
     * @return float the requested angle
     */
    float getDataBySource(imu_data data_source);
};