/**
 * @file BNO085.h
 * @author Robbe Elsermans (creator)
 * @author Thomas Kramp (revisor)
 * @brief BNO085 module to measure the motion of the vehicle.
 * @version 2.0
 * @date 2026-05-25
 *
 * @copyright Copyright (c) 2025 Robbe Elsermans, All rights reserved.
 * This project is released under the CC BY-NC-SA licence.
 * See licence.txt for more information.
 */
#ifndef BNO085_H_
#define BNO085_H_

#include <Adafruit_BNO08x.h>
#include <Wire.h>

#include "module.h"

/** @brief Reset pin for BNO085 sensor (active low). */
#define BNO08X_RESET -1

class BNO085 : public Module  {
    private:
        Adafruit_BNO08x   _bno085;          /** @brief BNO085 IMU interface. */
        sh2_SensorValue_t _sensorValue;     /** @brief Raw sensor values. */
        sh2_SensorId_t    _reportType;      /** @brief Sensor report type. */
        const long        _reportInterval;  /** @brief Report interval in microseconds for the selected sensor report. */

        float    _q0, _q1, _q2, _q3;    /** @brief Orientation quaternion components (W, I, J, K). */
        /** 
         * @brief Euler angles derived from quaternion (roll, pitch, yaw in radians).
         * @note Roll:  Rotation around the front-to-back axis.
         * @note Pitch: Rotation around the side-to-side axis.
         * @note Yaw:   Rotation around the vertical axis.
         */
        float    _roll, _pitch, _yaw;

        /**
         * @brief Configure the BNO085 to emit the desired report type.
         * @return true when report configuration succeeds.
         */
        bool setReports();
        /**
         * @brief Convert quaternion components to Euler angles.
         * @param qr Quaternion real component.
         * @param qi Quaternion I component.
         * @param qj Quaternion J component.
         * @param qk Quaternion K component.
         * @param degrees If true, convert output to degrees instead of radians.
         */
        void quaternionToEuler(const float qr, const float qi, const float qj, const float qk, const bool degrees = false);
        
    public:
        /**
         * @brief Construct a new BNO085 sensor.
         * @param address The I2C address.
         * @param fast_mode Select a higher-frequency report mode when true.
         */
        BNO085(const uint8_t address, const bool fast_mode = false)
        : Module("BNO085 - IMU Sensor"),
         _bno085(Adafruit_BNO08x(BNO08X_RESET)),
         _reportType(fast_mode ? SH2_GYRO_INTEGRATED_RV : SH2_ARVR_STABILIZED_RV),
         _reportInterval(fast_mode ? 2000 : 5000),
          _q0(1.0f),
          _q1(0.0f),
          _q2(0.0f),
          _q3(0.0f) {}

        /**
         * @brief Initialize the BNO085 IMU.
         * @return true if initialization is successful, false otherwise.
         */
        bool  init() override;

        /** @brief Compute the Euler angles. */
        void  read();

        /**
         * @brief Get the current roll angle.
         * @return Roll angle in degrees.
         */
        float getRoll();

        /**
         * @brief Get the current pitch angle.
         * @return Pitch angle in degrees.
         */
        float getPitch();

        /**
         * @brief Get the current yaw angle.
         * @return Yaw angle in degrees.
         */
        float getYaw();

        /**
         * @brief Get the real component of the current orientation quaternion.
         * @return Quaternion real component.
         */
        float getQuaternionR();

        /**
         * @brief Get the I component of the current orientation quaternion.
         * @return Quaternion I component.
         */
        float getQuaternionI();

        /**
         * @brief Get the J component of the current orientation quaternion.
         * @return Quaternion J component.
         */
        float getQuaternionJ();

        /**
         * @brief Get the K component of the current orientation quaternion.
         * @return Quaternion K component.
         */
        float getQuaternionK();
};

#endif