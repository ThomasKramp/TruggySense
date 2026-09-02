/**
 * @file voltage.cpp
 * @author Robbe Elsermans
 * @brief 
 * @version 0.1
 * @date 2025-04-10
 * 
 * @copyright Copyright (c) 2025 Robbe Elsermans, All rights reserved.
 * This project is released under the CC BY-NC-SA licence.
 * See licence.txt for more information
 * 
 */
#include "voltage.h"

uint8_t VoltageMeasure::init()
{
    pinMode(ADC_V_HLC, INPUT);
    pinMode(ADC_V_LLC, INPUT);
    pinMode(ADC_V_PER, INPUT);
    pinMode(ADC_V_B_1, INPUT);
    pinMode(ADC_V_B_2, INPUT);
    pinMode(ADC_V_B_3, INPUT);
    pinMode(ADC_V_B_4, INPUT);

    return 1;
}

void VoltageMeasure::ReadVoltage()
{
    memset(average_data, 0.0, sizeof(average_data));
    for (uint8_t i = 0; i < AVERAGE_COUNT; i++)
    {
        raw_data[VoltageMeasure::hlc][i] = analogRead(ADC_V_HLC);
        raw_data[VoltageMeasure::llc][i] = analogRead(ADC_V_LLC);
        raw_data[VoltageMeasure::per][i] = analogRead(ADC_V_PER);

        raw_data[VoltageMeasure::b_1][i] = analogRead(ADC_V_B_1);
        raw_data[VoltageMeasure::b_2][i] = analogRead(ADC_V_B_2);
        raw_data[VoltageMeasure::b_3][i] = analogRead(ADC_V_B_3);
        raw_data[VoltageMeasure::b_4][i] = analogRead(ADC_V_B_4);

        average_data[VoltageMeasure::hlc] += raw_data[VoltageMeasure::hlc][i];
        average_data[VoltageMeasure::llc] += raw_data[VoltageMeasure::llc][i];
        average_data[VoltageMeasure::per] += raw_data[VoltageMeasure::per][i];
        average_data[VoltageMeasure::b_1] += raw_data[VoltageMeasure::b_1][i];
        average_data[VoltageMeasure::b_2] += raw_data[VoltageMeasure::b_2][i];
        average_data[VoltageMeasure::b_3] += raw_data[VoltageMeasure::b_3][i];
        average_data[VoltageMeasure::b_4] += raw_data[VoltageMeasure::b_4][i];
    }

    average_data[VoltageMeasure::hlc] /= AVERAGE_COUNT;
    average_data[VoltageMeasure::llc] /= AVERAGE_COUNT;
    average_data[VoltageMeasure::per] /= AVERAGE_COUNT;
    average_data[VoltageMeasure::b_1] /= AVERAGE_COUNT;
    average_data[VoltageMeasure::b_2] /= AVERAGE_COUNT;
    average_data[VoltageMeasure::b_3] /= AVERAGE_COUNT;
    average_data[VoltageMeasure::b_4] /= AVERAGE_COUNT;
}

float VoltageMeasure::getVoltageBySource(volatge_sources source)
{
    switch (source)
    {
    case VoltageMeasure::hlc:
        return ((average_data[VoltageMeasure::hlc] * delta_v) / fac_volt_5v);
        break;

    case VoltageMeasure::llc:
        return ((average_data[VoltageMeasure::llc] * delta_v) / fac_volt_3V3);
        break;

    case VoltageMeasure::per:
        return ((average_data[VoltageMeasure::per] * delta_v) / fac_volt_3V3_peri);
        break;

    case VoltageMeasure::b_1:
        return ((average_data[VoltageMeasure::b_1] * delta_v) / fac_volt_c1);
        break;

    case VoltageMeasure::b_2:
        return ((average_data[VoltageMeasure::b_2] * delta_v) / fac_volt_c2) / 2.0;
        break;

    case VoltageMeasure::b_3:
        return ((average_data[VoltageMeasure::b_3] * delta_v) / fac_volt_c3) / 3.0;
        break;

    case VoltageMeasure::b_4:
        return ((average_data[VoltageMeasure::b_4] * delta_v) / fac_volt_c4) / 4.0;
        break;

    case VoltageMeasure::bat:
        return (getVoltageBySource(VoltageMeasure::b_1) + getVoltageBySource(VoltageMeasure::b_2) + getVoltageBySource(VoltageMeasure::b_3) + getVoltageBySource(VoltageMeasure::b_4));
        break;

    default:
        break;
    }
    return 0;
}