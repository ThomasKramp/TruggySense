/**
 * @file PIM448.cpp
 * @author Thomas Kramp (creator)
 * @brief Implementation of PIM448.h
 * @version 1.0
 * @date 2026-05-25
 * 
 * @copyright Copyright (c) 2026 Thomas Kramp, All rights reserved.
 * This project is released under the CC BY-NC-SA licence.
 * See licence.txt for more information.
 */
#include "PIM448.h"

#define PRINTDEBUG

bool PIM448::init() {
delay(50);
    if (!_imu.init()) {
        debugSerial.println("\tModule failed");
        setError(ModuleError::INIT_FAILED);
        return false;
    }
    delay(1000); // Let the IMU and its internal I²C bus settle
    if (_imu.whoAmI() != 0xEA) {
        debugSerial.println("\tWho Am I failed " + String(_imu.whoAmI(), HEX));
        setError(ModuleError::INIT_FAILED);
        return false;
    }
    bool magOk = false;
    for (int attempt = 0; attempt < 3; attempt++) {
        if (_imu.initMagnetometer()) {
            magOk = true;
            break;
        }
        debugSerial.println("\tMag retry...");
        delay(50);
    }
    if (!magOk) {
        debugSerial.println("\tMagnetometer failed");
        setError(ModuleError::INIT_FAILED);
        return false;
    }

  
    _imu.setAccRange(static_cast<ICM20948_accRange>(_accel_range));
    _imu.setGyrRange(static_cast<ICM20948_gyroRange>(_gyro_range));

    /*  DLPF    Accel 3dB Bandwidth [Hz]      Accel Output Rate [Hz]     Gyro 3dB Bandwidth [Hz]      Gyro Output Rate [Hz]
     *    0              246.0                     1125/(1+ASRD)                  196.6                    1125/(1+GSRD) 
     *    1              246.0                     1125/(1+ASRD)                  151.8                    1125/(1+GSRD)
     *    2              111.4                     1125/(1+ASRD)                  119.5                    1125/(1+GSRD)
     *    3               50.4                     1125/(1+ASRD)                   51.2                    1125/(1+GSRD)
     *    4               23.9                     1125/(1+ASRD)                   23.9                    1125/(1+GSRD)
     *    5               11.5                     1125/(1+ASRD)                   11.6                    1125/(1+GSRD)
     *    6                5.7                     1125/(1+ASRD)                    5.7                    1125/(1+GSRD)
     *    7              473.0                     1125/(1+ASRD)                  361.4                    1125/(1+GSRD)
     *    OFF           1209.0                     4500                         12106.0                    9000
     *    
     *    You achieve lowest noise using level 6  
     */
    _imu.setAccDLPF(static_cast<ICM20948_dlpf>(_accel_dlpf));
    _imu.setGyrDLPF(static_cast<ICM20948_dlpf>(_gyro_dlpf));

    /*  The sample rate divider divides the output rate of the accelerometer and gyroscope.
     *  Sample rate = Basic sample rate / (1 + divider) 
     *  It can only be applied if the corresponding DLPF is not off!
     *  Divider is a number: 
     *  - Accelerometer = 0...4095
     *  - Gyroscope     = 0...255
     *  If sample rates are set for the accelerometer and the gyroscope, the gyroscope
     *  sample rate has priority.
     */
    _imu.setAccSampleRateDivider(_accel_fs_devider);
    _imu.setGyrSampleRateDivider(_gyro_fs_devider);

    // add a delay of 1000/magRate to avoid first mag value being zero
    _imu.setMagOpMode(static_cast<AK09916_opMode>(_mag_refresh_rate));
    delay(50);
    return true;
}

void PIM448::read() {
    if (getLastError() == ModuleError::INIT_FAILED) {
        #ifdef PRINTDEBUG
            debugSerial.println("\t" + getErrorReport());
        #endif
        return;
    }

    _imu.readSensor();
    _imu.getGValues(&_accel);
    _imu.getGyrValues(&_gyro);
    _imu.getMagValues(&_mag);

    #ifdef PRINTDEBUG
        debugSerial.println("\tAcceleration (x,y,z):\t" + String(getAccelX()) + "g\t\t" + String(getAccelY()) + "g\t\t" + String(getAccelZ()) + "g");
        debugSerial.println("\tRotation (x,y,z):\t" + String(getGyroX()) + "rad/s\t" + String(getGyroY()) + "rad/s\t" + String(getGyroZ()) + "rad/s");
        debugSerial.println("\tMagnetic field (x,y,z):\t" + String(getMagX()) + "µT\t\t" + String(getMagY()) + "µT\t\t" + String(getMagZ()) + "µT");
    #endif

    _applyMahonyFilter(getAccelX(), getAccelY(), getAccelZ(),
                       getGyroX(),  getGyroY(),  getGyroZ(),
                       getMagX(),   getMagY(),   getMagZ());
}

float PIM448::getAccelX() { return _accel.x; }
float PIM448::getAccelY() { return _accel.y; }
float PIM448::getAccelZ() { return _accel.z; }

float PIM448::getGyroX() { return _gyro.x * 2 * M_PI / 360.0f; }
float PIM448::getGyroY() { return _gyro.y * 2 * M_PI / 360.0f; }
float PIM448::getGyroZ() { return _gyro.z * 2 * M_PI / 360.0f; }

float PIM448::getMagX() { return _mag.x; }
float PIM448::getMagY() { return _mag.y; }
float PIM448::getMagZ() { return _mag.z; }

float PIM448::getRoll() { return _roll; };
float PIM448::getPitch() { return _pitch; };
float PIM448::getYaw() { return _yaw; };

float PIM448::getQuaternionW() { return _q0; };
float PIM448::getQuaternionI() { return _q1; };
float PIM448::getQuaternionJ() { return _q2; };
float PIM448::getQuaternionK() { return _q3; };

float PIM448::_invSqrt(float x) { return 1.0f / sqrtf(x); }
void PIM448::_applyMahonyFilter(float ax, float ay, float az,
                                float gx, float gy, float gz,
                                float mx, float my, float mz) {
    uint32_t t0 = _imu_timer;
    uint32_t t1 = micros();
    double delta = ((t1 - t0) * 1.0f) / 1e6; // To seconds
    double _sampleFreq = 1.0f / delta;       // To Herz
    _imu_timer = t1;                         // Save for next run
    
    float recipNorm;
    float hx, hy, bx, bz;
    float halfvx, halfvy, halfvz, halfwx, halfwy, halfwz;
    float halfex, halfey, halfez;

    // Normalise accelerometer
    recipNorm = _invSqrt(ax*ax + ay*ay + az*az);
    ax *= recipNorm; ay *= recipNorm; az *= recipNorm;

    // Normalise magnetometer
    recipNorm = _invSqrt(mx*mx + my*my + mz*mz);
    mx *= recipNorm; my *= recipNorm; mz *= recipNorm;

    // Reference direction of Earth's magnetic field
    hx = 2.0f*(mx*(0.5f - _q2*_q2 - _q3*_q3) + my*(_q1*_q2 - _q0*_q3) + mz*(_q1*_q3 + _q0*_q2));
    hy = 2.0f*(mx*(_q1*_q2 + _q0*_q3) + my*(0.5f - _q1*_q1 - _q3*_q3) + mz*(_q2*_q3 - _q0*_q1));
    bx = sqrtf(hx*hx + hy*hy);
    bz = 2.0f*(mx*(_q1*_q3 - _q0*_q2) + my*(_q2*_q3 + _q0*_q1) + mz*(0.5f - _q1*_q1 - _q2*_q2));

    // Estimated direction of gravity and magnetic field
    halfvx = _q1*_q3 - _q0*_q2;
    halfvy = _q0*_q1 + _q2*_q3;
    halfvz = _q0*_q0 - 0.5f + _q3*_q3;
    halfwx = bx*(0.5f - _q2*_q2 - _q3*_q3) + bz*(_q1*_q3 - _q0*_q2);
    halfwy = bx*(_q1*_q2 - _q0*_q3)         + bz*(_q0*_q1 + _q2*_q3);
    halfwz = bx*(_q1*_q3 + _q0*_q2)         + bz*(0.5f - _q1*_q1 - _q2*_q2);

    // Error is cross product of estimated and measured directions
    halfex = (ay*halfvz - az*halfvy) + (my*halfwz - mz*halfwy);
    halfey = (az*halfvx - ax*halfvz) + (mz*halfwx - mx*halfwz);
    halfez = (ax*halfvy - ay*halfvx) + (mx*halfwy - my*halfwx);

    // Apply integral feedback
    _integralFB.x += TWO_KI * halfex * (1.0f / _sampleFreq);
    _integralFB.y += TWO_KI * halfey * (1.0f / _sampleFreq);
    _integralFB.z += TWO_KI * halfez * (1.0f / _sampleFreq);
    gx += _integralFB.x;
    gy += _integralFB.y;
    gz += _integralFB.z;

    // Apply proportional feedback
    gx += TWO_KP * halfex;
    gy += TWO_KP * halfey;
    gz += TWO_KP * halfez;

    // Integrate rate of change of quaternion
    float dt = 1.0f / _sampleFreq;
    _q0 += (-_q1*gx - _q2*gy - _q3*gz) * (0.5f * dt);
    _q1 += ( _q0*gx + _q2*gz - _q3*gy) * (0.5f * dt);
    _q2 += ( _q0*gy - _q1*gz + _q3*gx) * (0.5f * dt);
    _q3 += ( _q0*gz + _q1*gy - _q2*gx) * (0.5f * dt);

    // Normalise quaternion
    recipNorm = _invSqrt(_q0*_q0 + _q1*_q1 + _q2*_q2 + _q3*_q3);
    _q0 *= recipNorm;
    _q1 *= recipNorm;
    _q2 *= recipNorm;
    _q3 *= recipNorm;

    // Euler angles from quaternion
    // Pitch and roll are swapped due to the orientation of the PIM448 module.
    _roll  = atan2f(2.0f*(_q0*_q1 + _q2*_q3), 1.0f - 2.0f*(_q1*_q1 + _q2*_q2)) * 360.f / (2 * M_PI);
    _pitch = asinf(2.0f*(_q0*_q2 - _q3*_q1)) * 360.f / (2 * M_PI);
    _yaw   = atan2f(2.0f*(_q0*_q3 + _q1*_q2), 1.0f - 2.0f*(_q2*_q2 + _q3*_q3)) * 360.f / (2 * M_PI);

    #ifdef PRINTDEBUG
        debugSerial.println("\tQ0 (w):\t" + String(getQuaternionW()) + "\tQ1 (i):\t" + String(getQuaternionI()) 
                          + "\tQ2 (j):\t" + String(getQuaternionJ()) + "\tQ3 (k):\t" + String(getQuaternionK()));
        debugSerial.println("\tRoll:\t" + String(getRoll()) + "°/s\tPitch:\t" + String(getPitch()) + "°/s\tYaw:\t" + String(getYaw()) + "°/s");
    #endif
}