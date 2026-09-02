/**
 * @file voltage.h
 * @author Robbe Elsermans
 * @brief Contains class and pins to enable voltage measurements with the build in ADC.
 * @version 1.0
 * @date 2025-04-10
 *
 * @copyright Copyright (c) 2025 Robbe Elsermans, All rights reserved.
 * This project is released under the CC BY-NC-SA licence.
 * See licence.txt for more information
 *
 */
#pragma once

#include <Arduino.h>

#define ADC_V_HLC 24
#define ADC_V_LLC 20
#define ADC_V_PER 21

#define ADC_V_B_1 14 // C1
#define ADC_V_B_2 15 // C2
#define ADC_V_B_3 16 // C3
#define ADC_V_B_4 17 // C4

/**
 * @brief The average number represents how many measurements we conduct to calculate an average.
 *
 */
#define AVERAGE_COUNT 12.0F

class VoltageMeasure
{
private:
    const float delta_v = 0.00322;
    const float fac_volt_3V3 = 0.8913;
    const float fac_volt_3V3_peri = 0.8913;
    const float fac_volt_5v = 0.6427;
    const float fac_volt_c1 = 0.7674;
    const float fac_volt_c2 = 0.3394;
    const float fac_volt_c3 = 0.2476;
    const float fac_volt_c4 = 0.1803;

    float raw_data[8][(int)AVERAGE_COUNT] = {0};
    float average_data[8] = {0};

public:
    enum volatge_sources
    {
        hlc, // High level controller
        llc, // Low level controller
        per, // pheripheral
        b_1, // C1
        b_2, // C2
        b_3, // C3
        b_4, // C4
        bat  // Summation of Cx
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
    void ReadVoltage();

    /**
     * @brief Get the Voltage By Source
     * 
     * @param source the requested source 
     * @return float that represents the voltage value from the requested source
     */
    float getVoltageBySource(volatge_sources source);
};