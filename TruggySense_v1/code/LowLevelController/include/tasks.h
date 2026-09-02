/**
 * @file tasks.h
 * @author Robbe Elsermans
 * @brief In this file, all tasks their function names will be defined and a brief description of what the tasks include.
 *
 * Here, all task files will be defined and their datastructure. A task will run off a certain moment in time which are defined by
 * TIMER_INTERVAL_{task name}. These timers are set to a value which is partly derived from the proposal document.
 * Nevertheless, some timer intervals have been lowerd due to the fact that the drivers required this.ADC_ETC_CTRL_DMA_MODE_SEL
 *
 * If you want to lower a certain task, thus increase the sample rate, you need to check if a tasks total duration is not
 * longer than the task interval rate. This is easly done by using millis() and Serial.print() at the beginning and ending of a task
 * to have insights in the duration of the task.
 *
 * Remember to also increase the logging rate TIMER_INTERVAL_LOG and save rate TIMER_INTERVAL_SAVE_LOG to actually save the sampled data.
 *
 * When one would expand the datastructures with more data, they must be aware of the concequences.
 *
 * <ol>
 * <li>Changing the tasks such that the datastruct is filled in</li>
 * <li>Changing the datastruct for logging in sd_logger.h. Make sure to follow the sorting here (internally, data is stored aligned)</li>
 * <li>Changing the taskLog() and taskSaveLog() tasks</li>
 * <li>Changing the base_station.h such that the correct chuncks and payload size is entered</li>
 * <li>Changing the node-red backend such that the binary file is decoded correctly</li>
 * <li>Changing the base_station receiver nrf_rx to match the datastructure in sd_logger.h</li>
 * </ol>
 *
 * I know, adding a byte is cumbersome. If you want, you can change the code such that it becomes easier to add or remove code.
 * -> base_station could be initialized with the total payload which then can be sliced to 30 byte chunks inside the driver for instance.
 *
 * @version 1.0
 * @date 2025-04-10
 *
 * @copyright Copyright (c) 2025 Robbe Elsermans, All rights reserved.
 * This project is released under the CC BY-NC-SA licence.
 * See licence.txt for more information
 *
 */

#pragma once

#include <stdint.h>
// #include "X6B.h"
#include "ibus.h"
#include "actuate.h"
#include "current.h"
#include "voltage.h"
#include "encoder.h"
#include "temperature.h"
#include "imu.h"
#include "sd_logger.h"
#include "gps.h"
#include "base_station.h"

#define SYS_TIMER_INTERVAL_US 500       /**< @brief The system tick interval described in micro seconds. Should always be lower then the lowest task interval.*/

#define TIMER_INTERVAL_TEMPERATURE 2000 /**< @brief the temperature task interval rate in miliseconds*/
#define TIMER_INTERVAL_IBUS 10          /**< @brief the ibus task interval rate in miliseconds*/
#define TIMER_INTERVAL_GPS 1            /**< @brief the gps task interval rate in miliseconds*/
#define TIMER_INTERVAL_IMU 10           /**< @brief the imu task interval rate in miliseconds*/
#define TIMER_INTERVAL_ADC 1000         /**< @brief the adc task interval rate in miliseconds containing current and voltage measurement*/
#define TIMER_INTERVAL_ENCODER 166      /**< @brief the encoder task interval rate in miliseconds*/
#define TIMER_INTERVAL_ACTUATE 10       /**< @brief the actuation/ PWM task interval rate in miliseconds*/
#define TIMER_INTERVAL_LEDS 100         /**< @brief the led task interval rate in miliseconds*/
// #define TIMER_INTERVAL_ROS_SEND 10       /**< @brief the ROS send task interval rate in miliseconds*/
// #define TIMER_INTERVAL_ROS_RECEIVE 10    /**< @brief the ROS receive task interval rate in miliseconds*/
#define TIMER_INTERVAL_BASE_STATION 500 /**< @brief the base station communication task interval rate in miliseconds*/
#define TIMER_INTERVAL_LOG 50           /**< @brief the logging task interval rate in miliseconds*/
#define TIMER_INTERVAL_SAVE_LOG 100     /**< @brief the saving loggings task interval rate in miliseconds*/
#define TIMER_INTERVAL_SAFETY 10        /**< @brief the logging task interval rate in miliseconds*/

/**
 * @brief Initialization of all tasks
 *
 * If a certain module doesn't despond such as the nRF, SD-card, IMU, and GPS, it will disable these tasks at teh very start.
 * Nevertheless, this initializer will then throw 0 because a failure has happend.
 *
 * @return uint8_t 1 if success or 0 if failure
 */
uint8_t initializeTasks();

/**
 * @brief Task that will read the temperatures
 *
 * Task that will read the temperatures of the pheripherals and environments.
 * This includes the DS18B20+ sensors (OneWire) as well as the Teensy internal temperature sensor.
 * It will store the sensed values to the struct temperature_t
 *
 */
void taskReadTemperature();

/**
 * @brief datastruct of sensed temperatures
 *
 */
struct temperature
{
    float t_llc;   /**< @brief Temperature of low level controller*/
    float t_hlc;   /**< @brief Temperature of high level controller*/
    float t_bp;    /**< @brief Temperature of battery pack*/
    float t_ss;    /**< @brief Temperature of steering servo motor*/
    float t_bm_1;  /**< @brief Temperature of brushless motor 1*/
    float t_bm_2;  /**< @brief Temperature of brushless motor 2*/
    float t_bm_3;  /**< @brief Temperature of brushless motor 3*/
    float t_bm_4;  /**< @brief Temperature of brushless motor 4*/
    float t_esc_1; /**< @brief Temperature of Electric Speed Controller 1*/
    float t_esc_2; /**< @brief Temperature of Electric Speed Controller 2*/
    float t_esc_3; /**< @brief Temperature of Electric Speed Controller 3*/
    float t_esc_4; /**< @brief Temperature of Electric Speed Controller 4*/
};
typedef struct temperature temperature_t;

// extern Temperature temperature;
extern temperature_t temperature_data;

/**
 * @brief Task that can read the radio receiver and stores it in radio_t struct.
 *
 */
void taskReadIbus();

/**
 * @brief datastruct of radio channels
 *
 */
struct radio
{
    uint16_t ch_1;    /**< @brief Channel 1 that ranges from 1000 to 2000 (ms) */
    uint16_t ch_2;    /**< @brief Channel 2 that ranges from 1000 to 2000 (ms)*/
    uint16_t ch_3;    /**< @brief Channel 3 that ranges from 1000 to 2000 (ms)*/
    uint16_t ch_4;    /**< @brief Channel 4 that ranges from 1000 to 2000 (ms)*/
    uint16_t ch_5;    /**< @brief Channel 5 that ranges from 1000 to 2000 (ms)*/
    uint16_t ch_6;    /**< @brief Channel 6 that ranges from 1000 to 2000 (ms)*/
    uint16_t ch_7;    /**< @brief Channel 7 that ranges from 1000 to 2000 (ms)*/
    uint16_t ch_8;    /**< @brief Channel 8 that ranges from 1000 to 2000 (ms)*/
    uint16_t ch_9;    /**< @brief Channel 9 that ranges from 1000 to 2000 (ms)*/
    uint16_t ch_10;   /**< @brief Channel 10 that ranges from 1000 to 2000 (ms)*/
    uint16_t ch_11;   /**< @brief Channel 11 that ranges from 1000 to 2000 (ms)*/
    uint16_t ch_12;   /**< @brief Channel 12 that ranges from 1000 to 2000 (ms)*/
    uint16_t ch_13;   /**< @brief Channel 13 that ranges from 1000 to 2000 (ms)*/
    uint16_t ch_14;   /**< @brief Channel 14 that ranges from 1000 to 2000 (ms)*/
    uint8_t failsafe; /**< @brief when failsafe is active or not. 0 if no failsafe, 1 if failsafe.*/
};
typedef struct radio radio_t;

// extern IBUS x6b_receiver;
extern radio_t radio_data;

/**
 * @brief Task that can read the GPS module and store its data in localization_t struct.
 *
 */
void taskReadGps();

/**
 * @brief datastruct of localization data
 *
 */
struct localization
{
    long date;                    /**< @brief The date aquired from a satellite DDMMYY*/
    long time;                    /**< @brief The time aquired from a satellite in HHMMSSmm*/
    float latitude;               /**< @brief The latitude*/
    float longitude;              /**< @brief the longitide*/
    float velocity;               /**< @brief the relative velocity*/
    float altitude;               /**< @brief the altitude based on received satellites distances*/
    uint8_t connected_satellites; /**< @brief the number of connected satellites*/
};
typedef struct localization localization_t;

// extern GPSDriver gps;
extern localization_t gps_data;

/**
 * @brief Task that will read the ADC of both voltage and currents that are currently present and stores it in voltage_t and current_t.
 *
 */
void taskReadVAndI();

/**
 * @brief datastruct of voltage measurement data
 *
 */
struct voltage
{
    float v_cell_1;   /**< @brief Battery cell voltage 1.*/
    float v_cell_2;   /**< @brief Battery cell voltage 2.*/
    float v_cell_3;   /**< @brief Battery cell voltage 3.*/
    float v_cell_4;   /**< @brief Battery cell voltage 4.*/
    float v_batt;     /**< @brief Battery voltage.*/
    float v_llc;      /**< @brief VOltage of low level controllers power supply.*/
    float v_hlc;      /**< @brief Voltage of high level controllers power supply.*/
    float v_phrp_3v3; /**< @brief Voltage of pheripherals that use 3.3V*/
    // float v_phrp_5v;  /**< @brief Voltage of pheripherals that use 5V*/
};
typedef struct voltage voltage_t;

/**
 * @brief datastruct of current measurement data
 *
 */
struct current
{
    uint8_t i_esc_1; /**< @brief Current usage of esc 1*/
    uint8_t i_esc_2; /**< @brief Current usage of esc 2*/
    uint8_t i_esc_3; /**< @brief Current usage of esc 3*/
    uint8_t i_esc_4; /**< @brief Current usage of esc 4*/
    float i_ss;      /**< @brief Current usgae of steering servo*/
    float i_llc;     /**< @brief Current usage of low level controller*/
    float i_hlc;     /**< @brief Current usage of high level controllerv*/
};
typedef struct current current_t;

// extern VoltageMeasure voltageMeasure;
extern voltage_t voltage_data;
// extern CurrentMeasure currentMeasure;
extern current_t current_data;

/**
 * @brief Task that reads the 9-axis IMU and store its data in movement_t struct.
 *
 */
void taskReadIMU();

// https://simple.wikipedia.org/wiki/Pitch,_yaw,_and_roll

/**
 * @brief datastruct of IMU measurement data
 *
 */
struct movement
{
    float roll;  /**< @brief roll defines rolling*/
    float pitch; /**< @brief pitch defines nose up or down*/
    float yaw;   /**< @brief yaw defines left right swing*/
    float w;     /**< @brief real part of quaternionv*/
    float i;     /**< @brief i part*/
    float j;     /**< @brief j part*/
    float k;     /**< @brief k part*/
};
typedef struct movement movement_t;

// extern IMU imu;
extern movement_t movement_data;

/**
 * @brief Task that calculates the vehicles wheel & motor speed in RPM and stores it in encoder_t.
 *
 */
void taskReadEncoder();

/**
 * @brief datastruct of encoder measurement data
 *
 */
struct encoder
{
    float rpm_wheel_1; /**< @brief The RPM of wheel 1*/
    float rpm_wheel_2; /**< @brief The RPM of wheel 2*/
    float rpm_wheel_3; /**< @brief The RPM of wheel 3*/
    float rpm_wheel_4; /**< @brief The RPM of wheel 4*/
    float rpm_bm_1;    /**< @brief The RPM of motor 1*/
    float rpm_bm_2;    /**< @brief The RPM of motor 2*/
    float rpm_bm_3;    /**< @brief The RPM of motor 3*/
    float rpm_bm_4;    /**< @brief The RPM of motor 4*/
};
typedef struct encoder encoder_t;

// extern Encoder wheel_encoder;
extern encoder_t encoder_data;

/**
 * @brief Task that actuates PWM actuators such as the ESCs and servo based on the vehicle_actuation_t struct data
 *
 */
void taskActuate();

/**
 * @brief datastruct of pwm data that needs to be set
 *
 */
struct vehicle_actuation
{
    uint16_t pwm_bm_1; /**< @brief PWM value from 1000 to 2000 (ms) for brushless motor 1*/
    uint16_t pwm_bm_2; /**< @brief PWM value from 1000 to 2000 (ms) for brushless motor 2*/
    uint16_t pwm_bm_3; /**< @brief PWM value from 1000 to 2000 (ms) for brushless motor 3*/
    uint16_t pwm_bm_4; /**< @brief PWM value from 1000 to 2000 (ms) for brushless motor 4*/
    uint16_t pwm_ss;   /**< @brief PWM value from 1000 to 2000 (ms) for steering servo*/
};
typedef struct vehicle_actuation vehicle_actuation_t;

// extern Actuate vehicle_actuation;
extern vehicle_actuation_t vehicle_actuation_data;

/**
 * @brief Task to control the onboard LEDs for notifying the user based on the led_control_t
 * @note is not used at the moment
 */
void taskLed();

struct led_control
{
    uint8_t led_1; /**< @brief The flag to write led 1*/
    uint8_t led_2; /**< @brief The flag to write led 2*/
    uint8_t led_3; /**< @brief The flag to write led 3*/
};
typedef led_control led_control_t;

/**
 * @brief Task to log loggings into the logging_data struct
 * @note Data structure used here is defined in sd_logger.h.
 *
 */
void taskLog();

/**
 * @brief Task to log loggings onto an SD-card. Will use logging_data struct ad datastructure
 * @note Data structure used here is defined in sd_logger.h.
 */
void taskSaveLog();

/**
 * @brief Task to write received data to the base station
 * @note Data structure used here is defined in sd_logger.h.
 *
 */
void taskWriteBaseStation();

/**
 * @brief Task that controls the safety of the vehicle
 * 
 * 
 */
void taskSafety();

extern volatile uint8_t dead_switch; /**< Determines if the vehicle can be driven or not. @note is used as start and stop signal to Save a Log onto the sd card in taskSaveLog(). */

/**
 * @brief Task that handles the tick counter used to enable the tasks scedules or trigger moments
 *
 */
void sysTick();

// void taskPublishRos();
// void taskSubscribeRos();
