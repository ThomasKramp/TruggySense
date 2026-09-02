/**
 * @file imu.cpp
 * @author Robbe Elsermans
 * @brief Implementation of imu.h
 * @version 1
 * @date 2025-04-10
 * 
 * @copyright Copyright (c) 2025 Robbe Elsermans, All rights reserved.
 * This project is released under the CC BY-NC-SA licence.
 * See licence.txt for more information
 * 
 */
#include "imu.h"

Adafruit_BNO08x bno08x(BNO08X_RESET);

// https://quaternions.online/
// https://eater.net/quaternions
void IMU::quaternionToEuler(float qr, float qi, float qj, float qk, euler_t *ypr, bool degrees = false)
{
    float sqr = sq(qr);
    float sqi = sq(qi);
    float sqj = sq(qj);
    float sqk = sq(qk);

    ypr->yaw = atan2(2.0 * (qi * qj + qk * qr), (sqi - sqj - sqk + sqr));
    ypr->pitch = asin(-2.0 * (qi * qk - qj * qr) / (sqi + sqj + sqk + sqr));
    ypr->roll = atan2(2.0 * (qj * qk + qi * qr), (-sqi - sqj + sqk + sqr));

    if (degrees)
    {
        ypr->yaw *= RAD_TO_DEG;
        ypr->pitch *= RAD_TO_DEG;
        ypr->roll *= RAD_TO_DEG;
    }
}

void IMU::setReports(sh2_SensorId_t report_type, long report_interval)
{
    bno08x.enableReport(report_type, report_interval);
}

float IMU::getDataBySource(imu_data data_source)
{
    switch (data_source)
    {
    case yaw:
        return this->position_euler.yaw;
        break;

    case pitch:
        return this->position_euler.pitch;
        break;

    case roll:
        return this->position_euler.roll;
        break;

    case w:
        return this->position_quaternion.w;
        break;

    case i:
        return this->position_quaternion.i;
        break;

    case j:
        return this->position_quaternion.j;
        break;

    case k:
        return this->position_quaternion.k;
        break;

    default:
        return 0;
        break;
    }
}

void IMU::computeData()
{
    if (bno08x.wasReset())
    {
        setReports(this->report_type, this->report_interval_us);
    }

    if (bno08x.getSensorEvent(&sensor_value))
    {
        this->position_quaternion.w = sensor_value.un.arvrStabilizedRV.real;
        this->position_quaternion.i = sensor_value.un.arvrStabilizedRV.i;
        this->position_quaternion.j = sensor_value.un.arvrStabilizedRV.j;
        this->position_quaternion.k = sensor_value.un.arvrStabilizedRV.k;
        quaternionToEuler(sensor_value.un.arvrStabilizedRV.real, sensor_value.un.arvrStabilizedRV.i, sensor_value.un.arvrStabilizedRV.j, sensor_value.un.arvrStabilizedRV.k, &this->position_euler, true);
    }
}

uint8_t IMU::init()
{
    if (!bno08x.begin_I2C())
    {
        return 0;
    }

    setReports(this->report_type, this->report_interval_us);
    return 1;
}
