/**
 * @file current.cpp
 * @author Robbe Elsermans
 * @brief Implementation of current.h
 * @version 1
 * @date 2025-04-10
 * 
 * @copyright Copyright (c) 2025 Robbe Elsermans, All rights reserved.
 * This project is released under the CC BY-NC-SA licence.
 * See licence.txt for more information
 * 
 */
#include "current.h"

uint8_t CurrentMeasure::init()
{
    pinMode(ADC_I_SS, INPUT);
    pinMode(ADC_I_HLC, INPUT);
    pinMode(ADC_I_LLC, INPUT);
    pinMode(ADC_I_ESC_1, INPUT);
    // pinMode(ADC_I_ESC_2, INPUT);
    // pinMode(ADC_I_ESC_3, INPUT);
    // pinMode(ADC_I_ESC_4, INPUT);
    return 1;
}

void CurrentMeasure::ReadCurrent()
{
    memset(average_data, 0.0, sizeof(average_data));
    for (uint8_t i = 0; i < AVERAGE_COUNT; i++)
    {
        raw_data[CurrentMeasure::hlc][i] = analogRead(ADC_I_HLC);
        raw_data[CurrentMeasure::llc][i] = analogRead(ADC_I_LLC);
        raw_data[CurrentMeasure::ss][i] = analogRead(ADC_I_SS);
        raw_data[CurrentMeasure::esc_1][i] = analogRead(ADC_I_ESC_1);
        // raw_data[CurrentMeasure::esc_2][i] = analogRead(ADC_I_ESC_2);
        // raw_data[CurrentMeasure::esc_3][i] = analogRead(ADC_I_ESC_3);
        // raw_data[CurrentMeasure::esc_4][i] = analogRead(ADC_I_ESC_4);

        average_data[CurrentMeasure::hlc] += raw_data[CurrentMeasure::hlc][i];
        average_data[CurrentMeasure::llc] += raw_data[CurrentMeasure::llc][i];
        average_data[CurrentMeasure::ss] += raw_data[CurrentMeasure::ss][i];
        average_data[CurrentMeasure::esc_1] += raw_data[CurrentMeasure::esc_1][i];
        // average_data[CurrentMeasure::esc_2] += raw_data[CurrentMeasure::esc_2][i];
        // average_data[CurrentMeasure::esc_3] += raw_data[CurrentMeasure::esc_3][i];
        // average_data[CurrentMeasure::esc_4] += raw_data[CurrentMeasure::esc_4][i];
    }

    average_data[CurrentMeasure::hlc] /= AVERAGE_COUNT;
    average_data[CurrentMeasure::llc] /= AVERAGE_COUNT;
    average_data[CurrentMeasure::ss] /= AVERAGE_COUNT;
    average_data[CurrentMeasure::esc_1] /= AVERAGE_COUNT;
}

float CurrentMeasure::getCurrentBySource(current_source source)
{
    switch (source)
    {
    case CurrentMeasure::hlc:
        return ((average_data[CurrentMeasure::hlc] * delta_v) / fac_curr_hlc);
        break;

    case CurrentMeasure::llc:
        return ((average_data[CurrentMeasure::llc] * delta_v) / fac_curr_llc);
        break;

    case CurrentMeasure::ss:
        return ((average_data[CurrentMeasure::ss] * delta_v) / fac_curr_ss);
        break;

    case CurrentMeasure::esc_1:
        return ((average_data[CurrentMeasure::esc_1] * delta_v) - zero_curr_esc_1 / fac_curr_esc_1);
        break;

    // case CurrentMeasure::esc_2:
    //     return ((average_data[CurrentMeasure::esc_2] * delta_v) - zero_curr_esc_2 / fac_curr_esc_2);
    //     break;

    // case CurrentMeasure::esc_3:
    //     return ((average_data[CurrentMeasure::esc_3] * delta_v) - zero_curr_esc_3 / fac_curr_esc_3);
    //     break;

    // case CurrentMeasure::esc_4:
    //     return ((average_data[CurrentMeasure::esc_4] * delta_v) - zero_curr_esc_4 / fac_curr_esc_4);
    //     break;

    default:
        break;
    }
    return 0;
}