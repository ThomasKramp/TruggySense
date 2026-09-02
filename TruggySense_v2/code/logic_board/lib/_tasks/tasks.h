/**
 * @file tasks.h
 * @author Robbe Elsermans (creator)
 * @author Thomas Kramp (revisor)
 * @brief Scheduler of the project.
 * @version 2.0
 * @date 2026-05-25
 * 
 * @copyright Copyright (c) 2025 Robbe Elsermans, All rights reserved.
 * This project is released under the CC BY-NC-SA licence.
 * See licence.txt for more information.
 */
#ifndef TASKS_H_
#define TASKS_H_

#pragma once

#include <Arduino.h>

#include "i2c_addresses.h"
#include "interrupt_utils.h"
#include "pin_config.h"
#include "structs.h"

#include "I2C.h"

#include "led.h"
#include "motor.h"
#include "fan.h"
#include "FS-X6B.h"
#include "U-Blox.h"
#include "encoder.h"
#include "supply.h"
#include "ACS758.h"
#include "INA219.h"
#include "LM75.h"
// #include "PIM448.h"
#include "BNO085.h"
#include "SDCard.h"
#include "nRF24.h"

#define SYS_TIMER_INTERVAL_US       500     /** @brief The system tick interval described in micro seconds. Should always be lower then the lowest task interval.*/
#define TIMER_INTERVAL_LED          250     /** @brief Task  1) The indicator led task interval rate in miliseconds*/
#define TIMER_INTERVAL_SERVO        10      /** @brief Task  2) The steering servo task interval rate in miliseconds*/
#define TIMER_INTERVAL_MOTOR        10      /** @brief Task  3) the motor control task interval rate in miliseconds*/
#define TIMER_INTERVAL_FAN          100     /** @brief Task  4) The cooling fan task interval rate in miliseconds*/
#define TIMER_INTERVAL_IBUS         10      /** @brief Task  5) The remote control task interval rate in miliseconds*/
#define TIMER_INTERVAL_GPS          1       /** @brief Task  6) The gnss/gps task interval rate in miliseconds*/
#define TIMER_INTERVAL_ENCODER      166     /** @brief Task  7) The wheel encoder task interval rate in miliseconds*/
#define TIMER_INTERVAL_VOLT         1000    /** @brief Task  8) The adc task interval rate in miliseconds containing current and voltage measurement*/
#define TIMER_INTERVAL_CURRENT      1000    /** @brief Task  9) The adc task interval rate in miliseconds containing current and voltage measurement*/
#define TIMER_INTERVAL_TEMPERATURE  2000    /** @brief Task 10) The temperature task interval rate in miliseconds*/
#define TIMER_INTERVAL_MOTION       100     /** @brief Task 11) The imu task interval rate in miliseconds*/
#define TIMER_INTERVAL_ROS_SEND     100     /** @brief Task 12) The ROS send task interval rate in miliseconds*/
#define TIMER_INTERVAL_ROS_RECEIVE  100     /** @brief Task 13) The ROS receive task interval rate in miliseconds*/
#define TIMER_INTERVAL_LOG          100     /** @brief Task 14) The logging task interval rate in miliseconds*/
#define TIMER_INTERVAL_SD_LOG       100     /** @brief Task 15) The saving loggings task interval rate in miliseconds*/
#define TIMER_INTERVAL_WIRELESS_LOG 500     /** @brief Task 16) The base station communication task interval rate in miliseconds*/

extern Print &debugSerial;
extern HardwareSerial &gpsSerial;
extern HardwareSerial &ibusSerial;

extern I2C &i2c;

extern uint8_t tasks_initialized;
uint8_t initializeTasks();

extern LED *indicator_led;
void taskWriteLed();

extern Motor *steering_servo;
void taskWriteServo();

extern Motor *motor_fr;
extern Motor *motor_fl;
extern Motor *motor_rr;
extern Motor *motor_rl;
void taskWriteMotor();

extern Fan*cooling_fan;
void taskWriteFan();

extern radio_t radio_data;
extern FS_X6B *remote_control_receiver;
void taskReceiveRemoteControl();

extern localization_t gps_data;
extern U_BLOX *gnss_gps;
void taskReadGPS();

extern encoder_t encoder_data;
extern Encoder *encoder_fr;
extern Encoder *encoder_fl;
extern Encoder *encoder_rr;
extern Encoder *encoder_rl;
void taskReadEncoders();

extern voltage_t voltage_data;
extern Supply *voltage_sensor_bc_1;
extern Supply *voltage_sensor_bc_2;
extern Supply *voltage_sensor_bc_3;
extern Supply *voltage_sensor_bc_4;
extern Supply *voltage_sensor_3V3;
extern Supply *voltage_sensor_5V0;
void taskReadVoltages();

extern current_t current_data;
extern ACS758 *high_current_sensor_fr;
extern ACS758 *high_current_sensor_fl;
extern ACS758 *high_current_sensor_rr;
extern ACS758 *high_current_sensor_rl;
extern INA219 *low_current_sensor_hlc;
extern INA219 *low_current_sensor_llc;
extern INA219 *low_current_sensor_etc;
void taskReadCurrents();

extern temperature_t temperature_data;
extern LM75 *temperature_sensor_llc;
extern LM75 *temperature_sensor_hlc;
extern LM75 *temperature_sensor_bp;
extern LM75 *temperature_sensor_ss;
extern LM75 *temperature_sensor_bm_1;
extern LM75 *temperature_sensor_bm_2;
extern LM75 *temperature_sensor_bm_3;
extern LM75 *temperature_sensor_bm_4;
extern LM75 *temperature_sensor_esc_1;
extern LM75 *temperature_sensor_esc_2;
extern LM75 *temperature_sensor_esc_3;
extern LM75 *temperature_sensor_esc_4;
void taskReadTemperature();

extern movement_t movement_data;
// extern PIM448 *imu_sensor;
extern BNO085 *imu_sensor;
void taskReadIMU();

extern logging_data_t log_data;
void taskLogData();
extern SDCard *sd_logger;
void taskWriteSDLogs();
extern nRF24 *wireless_logger;
void taskSendLongRangeCommunication();

// void taskReadROS();
// void taskWriteROS();

void sysTick();

#endif