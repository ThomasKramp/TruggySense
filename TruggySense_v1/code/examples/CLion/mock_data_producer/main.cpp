#include <iostream>
#include <fstream>
using namespace std;

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
    uint8_t n_failsafe; // when failsafe is active or not. 1 if not active, 0 if active.
    uint8_t dead_switch;// A deadswitch on the transmitter side if something goes wrong
    uint8_t i_esc_1;    // Current usage of esc 1
    uint8_t i_esc_2;    // Current usage of esc 2
    uint8_t i_esc_3;    // Current usage of esc 3
    uint8_t i_esc_4;    // Current usage of esc 4
};

int main() {
    logging_data data = {
        1, // timestamp
        2,   // date
        12345,      // time (in seconds from start of day)
        35.5f,      // t_llc
        45.3f,      // t_hlc
        30.2f,      // t_bp
        50.1f,      // t_ss
        60.4f,      // t_bm_1
        55.3f,      // t_bm_2
        58.2f,      // t_bm_3
        57.8f,      // t_bm_4
        42.0f,      // t_esc_1
        41.5f,      // t_esc_2
        43.1f,      // t_esc_3
        40.9f,      // t_esc_4
        2.0f,       // i_ss
        1.8f,       // i_llc
        2.2f,       // i_hlc
        10.0f,      // roll
        15.5f,      // pitch
        20.5f,      // yaw
        1.0f,       // w
        0.5f,       // i
        0.5f,       // j
        0.5f,       // k
        300.0f,     // rpm_wheel_1
        310.0f,     // rpm_wheel_2
        320.0f,     // rpm_wheel_3
        330.0f,     // rpm_wheel_4
        1500.0f,    // rpm_bm_1
        1400.0f,    // rpm_bm_2
        1450.0f,    // rpm_bm_3
        1600.0f,    // rpm_bm_4
        45.12345f,  // latitude
        -93.12345f, // longitude
        75.0f,      // velocity
        300.0f,     // altitude
        3.7f,       // v_cell_1
        3.7f,       // v_cell_2
        3.7f,       // v_cell_3
        3.7f,       // v_cell_4
        14.8f,      // v_batt
        5.0f,       // v_llc
        5.0f,       // v_hlc
        3.3f,       // v_phrp_3v3
        1500,       // ch_1
        1500,       // ch_2
        1500,       // ch_3
        1500,       // ch_4
        1500,       // ch_5
        1500,       // ch_6
        1500,       // ch_7
        1500,       // ch_8
        1500,       // ch_9
        1500,       // ch_10
        1500,       // ch_11
        1500,       // ch_12
        1500,       // ch_13
        1500,       // ch_14
        1500,       // pwm_bm_1
        1500,       // pwm_bm_2
        1500,       // pwm_bm_3
        1500,       // pwm_bm_4
        1501,       // pwm_ss
        10,         // c_s (connected satellites)
        1,          // n_failsafe (failsafe inactive)
        0,          // dead_switch (inactive)
        5,          // i_esc_1
        5,          // i_esc_2
        5,          // i_esc_3
        5           // i_esc_4
    };
    ofstream MyFile("sample_1.bin");
    MyFile.write((char*)&data, sizeof(data));
    // for (uint8_t i = 1; i < 10; i++) {
    //
    //     MyFile.write((char*)&data, sizeof(data));
    // }
    cout << sizeof(data) << endl;
    MyFile.close();

    return 0;
}
