/**
 * @file PIM448.h
 * @author Thomas Kramp (creator)
 * @brief PIM448 module to measure the motion of the vehicle.
 * @version 1.0
 * @date 2026-05-25
 * 
 * @copyright Copyright (c) 2026 Thomas Kramp, All rights reserved.
 * This project is released under the CC BY-NC-SA licence.
 * See licence.txt for more information.
 */
#ifndef PIM448_H_
#define PIM448_H_

#include <ICM20948_WE.h>
#include <math.h>

#include "I2C.h"
#include "module.h"

/** @brief Mahony filter proportional gain constant. */
#define TWO_KP 2.0f

/** @brief Mahony filter integral gain constant. */
#define TWO_KI 0.005f

/**
 * @enum ACCEL_RANGE
 * @brief Accelerometer full-scale range settings for the PIM448 IMU.
 */
enum class ACCEL_RANGE {
    RANGE_2G  = ICM20948_ACC_RANGE_2G,      /** @brief 2 g full-scale range for highest sensitivity. */
    RANGE_4G  = ICM20948_ACC_RANGE_4G,      /** @brief 4 g full-scale range for moderate dynamics. */
    RANGE_8G  = ICM20948_ACC_RANGE_8G,      /** @brief 8 g full-scale range for high dynamics. */
    RANGE_16G = ICM20948_ACC_RANGE_16G      /** @brief 16 g full-scale range for very high dynamics. */
};

/**
 * @enum GYRO_RANGE
 * @brief Gyroscope full-scale range settings for the PIM448 IMU.
 */
enum class GYRO_RANGE {
    RANGE_250  = ICM20948_GYRO_RANGE_250,   /** @brief 250 °/s full-scale range for highest angular sensitivity. */
    RANGE_500  = ICM20948_GYRO_RANGE_500,   /** @brief 500 °/s full-scale range for moderate angular dynamics. */
    RANGE_1000 = ICM20948_GYRO_RANGE_1000,  /** @brief 1000 °/s full-scale range for high angular dynamics. */
    RANGE_2000 = ICM20948_GYRO_RANGE_2000   /** @brief 2000 °/s full-scale range for very high angular dynamics. */
};

/**
 * @enum DLPF
 * @brief Digital low-pass filter options for accelerometer and gyroscope.
 */
enum class DLPF {
    DLPF_0   = ICM20948_DLPF_0,    /** @brief Lowest cutoff frequency / strongest filtering. */
    DLPF_1   = ICM20948_DLPF_1,    /** @brief Low cutoff frequency filter. */
    DLPF_2   = ICM20948_DLPF_2,    /** @brief Moderate cutoff frequency filter. */
    DLPF_3   = ICM20948_DLPF_3,    /** @brief Higher cutoff frequency filter. */
    DLPF_4   = ICM20948_DLPF_4,    /** @brief Higher cutoff frequency filter with reduced delay. */
    DLPF_5   = ICM20948_DLPF_5,    /** @brief Near maximum cutoff frequency filter. */
    DLPF_6   = ICM20948_DLPF_6,    /** @brief Maximum cutoff frequency / minimal filtering. */
    DLPF_7   = ICM20948_DLPF_7,    /** @brief Alternative maximum cutoff frequency option. */
    DLPF_OFF = ICM20948_DLPF_OFF   /** @brief Digital low-pass filter disabled. */
};

/**
 * @enum MAG_REFRESH_RATE
 * @brief Magnetometer continuous measurement refresh rate settings.
 */
enum class MAG_REFRESH_RATE {
    RATE_10HZ  = AK09916_CONT_MODE_10HZ,   /** @brief Continuous magnetometer update at 10 Hz. */
    RATE_20HZ  = AK09916_CONT_MODE_20HZ,   /** @brief Continuous magnetometer update at 20 Hz. */
    RATE_50HZ  = AK09916_CONT_MODE_50HZ,   /** @brief Continuous magnetometer update at 50 Hz. */
    RATE_100HZ = AK09916_CONT_MODE_100HZ   /** @brief Continuous magnetometer update at 100 Hz. */
};

class PIM448 : public Module  {
    private:
        ICM20948_WE _imu;                   /** @brief ICM20948 IMU interface. */
        ACCEL_RANGE _accel_range;           /** @brief Accelerometer full-scale range. */
        GYRO_RANGE  _gyro_range;            /** @brief Gyroscope full-scale range. */
        DLPF        _accel_dlpf;            /** @brief Accelerometer digital low-pass filter configuration. */
        DLPF        _gyro_dlpf;             /** @brief Gyroscope digital low-pass filter configuration. */
        uint16_t    _accel_fs_devider;      /** @brief Accelerometer averaging divider for over-sampling. */
        uint8_t     _gyro_fs_devider;       /** @brief Gyroscope averaging divider for over-sampling. */
        MAG_REFRESH_RATE _mag_refresh_rate; /** @brief Magnetometer refresh rate. */

        xyzFloat _accel;    /** @brief Latest accelerometer measurements in g. */
        xyzFloat _gyro;     /** @brief Latest gyroscope measurements in radians per second. */
        xyzFloat _mag;      /** @brief Latest magnetometer measurements in microtesla. */

        uint32_t _imu_timer;    /** @brief Mahony filter state - last update timestamp in milliseconds. */
        double   _sampleFreq;   /** @brief Sample frequency in Hz. */

        float    _q0, _q1, _q2, _q3;    /** @brief Orientation quaternion components (W, I, J, K). */
        xyzFloat _integralFB;           /** @brief Integral feedback error components for the Mahony filter. */
        /** 
         * @brief Euler angles derived from quaternion (roll, pitch, yaw in radians).
         * @note Roll:  Rotation around the front-to-back axis.
         * @note Pitch: Rotation around the side-to-side axis.
         * @note Yaw:   Rotation around the vertical axis.
         */
        float    _roll, _pitch, _yaw;

        /**
         * @brief Inverse square-root (used by the Mahony filter).
         * @param x Input value.
         * @return Approximate inverse square-root of x.
         */
        float _invSqrt(float x);

        /**
         * @brief Apply the Mahony filter to update the orientation from sensor data.
         * @param ax Accelerometer X axis in g.
         * @param ay Accelerometer Y axis in g.
         * @param az Accelerometer Z axis in g.
         * @param gx Gyroscope X axis in degrees per second.
         * @param gy Gyroscope Y axis in degrees per second.
         * @param gz Gyroscope Z axis in degrees per second.
         * @param mx Magnetometer X axis in microtesla.
         * @param my Magnetometer Y axis in microtesla.
         * @param mz Magnetometer Z axis in microtesla.
         */
        void  _applyMahonyFilter(float ax, float ay, float az,
                                 float gx, float gy, float gz,
                                 float mx, float my, float mz);

    public:
        /**
         * @brief Construct a new PIM448 sensor.
         * @param address The I2C address.
         * @param accel_range Accelerometer full-scale range.
         * @param gyro_range Gyroscope full-scale range.
         * @param accel_dlpf Accelerometer digital low-pass filter setting.
         * @param gyro_dlpf Gyroscope digital low-pass filter setting.
         * @param accel_fs_devider Accelerometer amount of samples used.
         * @param gyro_fs_devider Gyroscope amount of samples used.
         * @param mag_refresh_rate Magnetometer continuous measurement refresh rate.
         */
        PIM448(const uint8_t address, const ACCEL_RANGE accel_range   = ACCEL_RANGE::RANGE_2G,
                                      const GYRO_RANGE gyro_range     = GYRO_RANGE::RANGE_250,
                                      const DLPF accel_dlpf           = DLPF::DLPF_OFF,
                                      const DLPF gyro_dlpf            = DLPF::DLPF_OFF,
                                      const uint16_t accel_fs_devider = 1,
                                      const uint8_t gyro_fs_devider   = 1,
                                      const MAG_REFRESH_RATE mag_refresh_rate = MAG_REFRESH_RATE::RATE_10HZ
        ) : Module("PIM448 - IMU Sensor"),
          _imu(ICM20948_WE(address)),
          _accel_range(accel_range),
          _gyro_range(gyro_range),
          _accel_dlpf(accel_dlpf),
          _gyro_dlpf(gyro_dlpf),
          _accel_fs_devider(accel_fs_devider),
          _gyro_fs_devider(gyro_fs_devider),
          _mag_refresh_rate(mag_refresh_rate),
          _imu_timer(0),
          _accel{0.0f, 0.0f, 0.0f},
          _gyro{0.0f, 0.0f, 0.0f},
          _mag{0.0f, 0.0f, 0.0f},
          _q0(1.0f),
          _q1(0.0f),
          _q2(0.0f),
          _q3(0.0f),
          _integralFB{0.0f, 0.0f, 0.0f} {}

        /**
         * @brief Initialize the PIM448 IMU.
         * This includes both the ICM20948 and the AK09916 magnetometer.
         * @return true if initialization is successful, false otherwise.
         */
        bool  init() override;

        /** @brief Read the latest raw sensor values, then compute the Euler angles. */
        void  read();

        /**
         * @brief Get accelerometer X axis measurement.
         * @return Accelerometer X axis in g.
         */
        float getAccelX();

        /**
         * @brief Get accelerometer Y axis measurement.
         * @return Accelerometer Y axis in g.
         */
        float getAccelY();

        /**
         * @brief Get accelerometer Z axis measurement.
         * @return Accelerometer Z axis in g.
         */
        float getAccelZ();

        /**
         * @brief Get gyroscope X axis measurement.
         * @return Gyroscope X axis in radians per second.
         */
        float getGyroX();

        /**
         * @brief Get gyroscope Y axis measurement.
         * @return Gyroscope Y axis in radians per second.
         */
        float getGyroY();

        /**
         * @brief Get gyroscope Z axis measurement.
         * @return Gyroscope Z axis in radians per second.
         */
        float getGyroZ();

        /**
         * @brief Get magnetometer X axis measurement.
         * @return Magnetometer X axis in microtesla.
         */
        float getMagX();

        /**
         * @brief Get magnetometer Y axis measurement.
         * @return Magnetometer Y axis in microtesla.
         */
        float getMagY();

        /**
         * @brief Get magnetometer Z axis measurement.
         * @return Magnetometer Z axis in microtesla.
         */
        float getMagZ();

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
        float getQuaternionW();

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