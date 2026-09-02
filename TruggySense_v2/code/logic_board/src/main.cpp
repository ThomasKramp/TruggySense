/**
 * @file main.cpp
 * @author Robbe Elsermans (creator)
 * @author Thomas Kramp (revisor)
 * @brief Main sketch of the project.
 * @version 2.0
 * @date 2026-05-25
 * 
 * @copyright Copyright (c) 2025 Robbe Elsermans, All rights reserved.
 * This project is released under the CC BY-NC-SA licence.
 * See licence.txt for more information.
 */
#include "tasks.h"

uint8_t adcResolution = 10;
uint8_t sampleSize = 12;

Print &debugSerial = Serial;
HardwareSerial &ibusSerial = Serial8;
HardwareSerial &gpsSerial = Serial5;

I2C &i2c;

uint8_t tasks_initialized = 0;

LED *indicator_led;

Motor *steering_servo;
Motor *motor_fr;
Motor *motor_fl;
Motor *motor_rr;
Motor *motor_rl;
Fan   *cooling_fan;

FS_X6B *remote_control_receiver;
radio_t radio_data;

U_BLOX *gnss_gps;
localization_t gps_data;

Encoder *encoder_fr;
Encoder *encoder_fl;
Encoder *encoder_rr;
Encoder *encoder_rl;
encoder_t encoder_data;

Supply *voltage_sensor_bc_1;
Supply *voltage_sensor_bc_2;
Supply *voltage_sensor_bc_3;
Supply *voltage_sensor_bc_4;
Supply *voltage_sensor_3V3;
Supply *voltage_sensor_5V0;
voltage_t voltage_data;

ACS758 *high_current_sensor_fr;
ACS758 *high_current_sensor_fl;
ACS758 *high_current_sensor_rr;
ACS758 *high_current_sensor_rl;
INA219 *low_current_sensor_hlc;
INA219 *low_current_sensor_llc;
INA219 *low_current_sensor_etc;
current_t current_data;

LM75 *temperature_sensor_llc;
LM75 *temperature_sensor_hlc;
LM75 *temperature_sensor_bp;
LM75 *temperature_sensor_ss;
LM75 *temperature_sensor_bm_1;
LM75 *temperature_sensor_bm_2;
LM75 *temperature_sensor_bm_3;
LM75 *temperature_sensor_bm_4;
LM75 *temperature_sensor_esc_1;
LM75 *temperature_sensor_esc_2;
LM75 *temperature_sensor_esc_3;
LM75 *temperature_sensor_esc_4;
temperature_t temperature_data;

// PIM448 *imu_sensor;
BNO085 *imu_sensor;
movement_t movement_data;

SDCard *sd_logger;
nRF24 *wireless_logger;
logging_data_t log_data;

//#define SHOW_STARTUP

void setup() {
    Serial.begin(115200);

    #ifdef SHOW_STARTUP
        //while (!Serial) {}
        debugSerial.println("Start initialisation");
    #endif
    
    // Initialize system
    if (!initializeTasks()) {
      debugSerial.println("An error occured during startup!");
      indicator_led->write(LOW);
    }
}

// main.tex line 1900

void loop() { }