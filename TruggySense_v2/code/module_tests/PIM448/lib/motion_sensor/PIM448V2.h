#ifndef PIM448V2_H_
#define PIM448V2_H_

#include <ICM20948_WE.h>
#include <Arduino.h>
#include <math.h>

#include "I2C.h"
#include "interrupt_utils.h"
#include "module.h"

extern Print &debugSerial;

class PIM448 : public Module  {
    public:
        enum ADDRESS {
            ADDRESS_1 = 0x68,
            ADDRESS_2 = 0x69
        };

        enum ACCEL_RANGE {
            RANGE_2G  = ICM20948_ACC_RANGE_2G,      //  2 g (default)
            RANGE_4G  = ICM20948_ACC_RANGE_4G,      //  4 g
            RANGE_8G  = ICM20948_ACC_RANGE_8G,      //  8 g
            RANGE_16G = ICM20948_ACC_RANGE_16G      // 16 g
        };

        enum GYRO_RANGE {
            RANGE_250  = ICM20948_GYRO_RANGE_250,   // 250 °/s (default)
            RANGE_500  = ICM20948_GYRO_RANGE_500,   // 500 °/s
            RANGE_1000 = ICM20948_GYRO_RANGE_1000,  // 1000 °/s
            RANGE_2000 = ICM20948_GYRO_RANGE_2000   // 2000 °/s
        };

        enum DLPF {
            DLPF_0   = ICM20948_DLPF_0,
            DLPF_1   = ICM20948_DLPF_1,
            DLPF_2   = ICM20948_DLPF_2,
            DLPF_3   = ICM20948_DLPF_3,
            DLPF_4   = ICM20948_DLPF_4,
            DLPF_5   = ICM20948_DLPF_5,
            DLPF_6   = ICM20948_DLPF_6,
            DLPF_7   = ICM20948_DLPF_7,
            DLPF_OFF = ICM20948_DLPF_OFF 
        };

        enum MAG_REFRESH_RATE {                     // Continuous measurements
            RATE_10HZ =  AK09916_CONT_MODE_10HZ,    // 10 Hz
            RATE_20HZ =  AK09916_CONT_MODE_20HZ,    // 20 Hz
            RATE_50HZ =  AK09916_CONT_MODE_50HZ,    // 50 Hz
            RATE_100HZ = AK09916_CONT_MODE_100HZ    // 100 Hz (default)
        };
  
        PIM448(const ADDRESS address, const ACCEL_RANGE accel_range,   const GYRO_RANGE gyro_range,
                                      const DLPF accel_dlpf,           const DLPF gyro_dlpf,
                                      const uint16_t accel_fs_devider, const uint8_t gyro_fs_devider,
                                      const MAG_REFRESH_RATE mag_refresh_rate)
        : Module("PIM448 - IMU Sensor"),
          _imu(ICM20948_WE(address)),
          _accel_range(accel_range),
          _gyro_range(gyro_range),
          _accel_dlpf(accel_dlpf),
          _gyro_dlpf(gyro_dlpf),
          _accel_fs_devider(accel_fs_devider),
          _gyro_fs_devider(gyro_fs_devider),
          _mag_refresh_rate(mag_refresh_rate),
          _imu_timer(0),
          _q0(1.0f),
          _q1(0.0f),
          _q2(0.0f),
          _q3(0.0f),
          _integralFBx(0.0f),
          _integralFBy(0.0f),
          _integralFBz(0.0f) {}

        bool  init() override;
        void  read();

        float getAccelX();
        float getAccelY();
        float getAccelZ();

        float getGyroX();
        float getGyroY();
        float getGyroZ();

        float getMagX();
        float getMagY();
        float getMagZ();

        float getRoll();
        float getPitch();
        float getYaw();

        float getQuaternionW();
        float getQuaternionI();
        float getQuaternionJ();
        float getQuaternionK();

    private:
        ICM20948_WE _imu;
        ACCEL_RANGE _accel_range;
        GYRO_RANGE  _gyro_range;
        DLPF        _accel_dlpf;
        DLPF        _gyro_dlpf;
        uint16_t    _accel_fs_devider;
        uint8_t     _gyro_fs_devider;
        MAG_REFRESH_RATE _mag_refresh_rate;

        xyzFloat _accel;
        xyzFloat _gyro;
        xyzFloat _mag;

        // Mahony filter state
        uint32_t _imu_timer;
        double   _sampleFreq; // Hz — match your actual update rate
        float    _q0, _q1, _q2, _q3;
        float    _integralFBx, _integralFBy, _integralFBz;
        float    _roll, _pitch, _yaw;
        // Roll:  Rotation around the front-to-back axis.
        // Pitch: Rotation around the side-to-side axis.
        // Yaw:   Rotation around the vertical axis.

        // Since the module is rotated by 90 degrees, the pitch and angle are swapped.


        #define TWO_KP 2.0f     // Proportional gain
        #define TWO_KI 0.005f   // Integral gain

        float _invSqrt(float x);
        void  _applyMahonyFilter(float ax, float ay, float az,
                                 float gx, float gy, float gz,
                                 float mx, float my, float mz);
};

#endif