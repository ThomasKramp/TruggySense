/**
 * @file current.h
 * @author Robbe Elsermans
 * @brief Contains class and pins to enable current measurements with the INA169 and ACS758ECB-200B current sensors.
 * @version 1.0
 * @date 2025-04-10
 * 
 * @note the ACS758ECB-200B current sensor doesn't work at the moment. Only ESC_1 is implemented.
 * 
 * @copyright Copyright (c) 2025 Robbe Elsermans, All rights reserved.
 * This project is released under the CC BY-NC-SA licence.
 * See licence.txt for more information
 * 
 */
#pragma once

#include <Arduino.h>

#define ADC_I_SS 41 /**< @brief Pin definition of steerig servo current */
#define ADC_I_HLC 39 /**< @brief Pin definition of high level controller current */
#define ADC_I_LLC 40 /**< @brief Pin definition of low level controller current */
#define ADC_I_ESC_1 27 /**< @brief Pin definition of ESC 1 current corresponding to Rear Left*/
// #define ADC_I_ESC_2 38 /**< @brief Pin definition of ESC 2 current */ // RR
// #define ADC_I_ESC_3 26 /**< @brief Pin definition of ESC 3 current */ // FL
// #define ADC_I_ESC_4 25 /**< @brief Pin definition of ESC 4 current */ // FR

/**
 * @brief The average number represents how many measurements we conduct to calculate an average.
 * 
 */
#define AVERAGE_COUNT 12.0F

/**
 * @brief CurrentMeasure class is responible to initialize the pins, calculate and return the calculated current
 * 
 */
class CurrentMeasure
{
private:
    const float delta_v = 0.00322; /**< @brief Precision of the local ADC (10-bit) in volts */
    const float fac_curr_hlc = 0.75;
    const float fac_curr_llc = 1;
    const float fac_curr_ss = 0.75;
    const float fac_curr_esc_1 = 0.01; //10mV/A
    const float zero_curr_esc_1 = 2.5; //5V/2=2.5V
    // const float fac_curr_esc_2 = 0.02; //20mV/A
    // const float zero_curr_esc_2 = 2.5; //5V/2=2.5V
    // const float fac_curr_esc_3 = 0.02; //20mV/A
    // const float zero_curr_esc_3 = 2.5; //5V/2=2.5V
    // const float fac_curr_esc_4 = 0.02; //20mV/A
    // const float zero_curr_esc_4 = 2.5; //5V/2=2.5V

    float raw_data[8][(int)AVERAGE_COUNT] = {0};
    float average_data[8] = {0};

public:

    /**
     * @brief the different current sources 
     * 
     */
    enum current_source
    {
        hlc,   // High level controller
        llc,   // Low level controller
        ss,    // Steering Servo
        esc_1, // ESC 1
        esc_2, // ESC 2
        esc_3, // ESC 3
        esc_4, // ESC 4
    };

    /**
     * @brief initializes the utilized pins
     * 
     * @return uint8_t 1 if success and 0 if failure
     */
    uint8_t init();

    /**
     * @brief reads the analog value of the pins and repeats this process based on AVERAGE_COUNT
     * 
     */
    void ReadCurrent();

    /**
     * @brief Get the current by source current_source.
     * 
     * @param source the requested source 
     * @return float that represents the current value from the requested source
     */
    float getCurrentBySource(current_source source);
};