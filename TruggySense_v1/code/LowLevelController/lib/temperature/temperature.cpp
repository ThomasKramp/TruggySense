/**
 * @file temperature.cpp
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
#include "temperature.h"
#include <Arduino.h>

OneWire oneWire(ONE_WIRE_BUS);

// // Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

// float internal_temp = 0.0;
// uint8_t sensor_availability_1 = 0;
// uint8_t sensor_availability_2 = 0;

uint8_t Temperature::init()
{
    sensor_availability_1 = 0;
    sensor_availability_2 = 0;

    sensors.begin(); // implicit reseTemperature::search()

    // Loop though the availabe sensors and see if there are matches with known addresses. Only taking the first 4 bytes.
    DeviceAddress temp;
    while (oneWire.search(temp))
    {
        if (temp[0] == hlc_thermometer[0] && temp[1] == hlc_thermometer[1] && temp[2] == hlc_thermometer[2] && temp[3] == hlc_thermometer[3])
        {
            sensor_availability_1 |= HLC_AVAILABILITY;
        }
        else if (temp[0] == bp_thermometer[0] && temp[1] == bp_thermometer[1] && temp[2] == bp_thermometer[2] && temp[3] == bp_thermometer[3])
        {
            sensor_availability_1 |= BP_AVAILABILITY;
        }
        else if (temp[0] == esc_1_thermometer[0] && temp[1] == esc_1_thermometer[1] && temp[2] == esc_1_thermometer[2] && temp[3] == esc_1_thermometer[3])
        {
            sensor_availability_2 |= ESC_1_AVAILABILITY;
        }
        else if (temp[0] == esc_2_thermometer[0] && temp[1] == esc_2_thermometer[1] && temp[2] == esc_2_thermometer[2] && temp[3] == esc_2_thermometer[3])
        {
            sensor_availability_2 |= ESC_2_AVAILABILITY;
        }
        else if (temp[0] == esc_3_thermometer[0] && temp[1] == esc_3_thermometer[1] && temp[2] == esc_3_thermometer[2] && temp[3] == esc_3_thermometer[3])
        {
            sensor_availability_2 |= ESC_3_AVAILABILITY;
        }
        else if (temp[0] == esc_4_thermometer[0] && temp[1] == esc_4_thermometer[1] && temp[2] == esc_4_thermometer[2] && temp[3] == esc_4_thermometer[3])
        {
            sensor_availability_2 |= ESC_4_AVAILABILITY;
        }
        else if (temp[0] == ss_thermometer[0] && temp[1] == ss_thermometer[1] && temp[2] == ss_thermometer[2] && temp[3] == ss_thermometer[3])
        {
            sensor_availability_1 |= SS_AVAILABILITY;
        }
        else if (temp[0] == bm_1_thermometer[0] && temp[1] == bm_1_thermometer[1] && temp[2] == bm_1_thermometer[2] && temp[3] == bm_1_thermometer[3])
        {
            sensor_availability_2 |= BM_1_AVAILABILITY;
        }
        else if (temp[0] == bm_2_thermometer[0] && temp[1] == bm_2_thermometer[1] && temp[2] == bm_2_thermometer[2] && temp[3] == bm_2_thermometer[3])
        {
            sensor_availability_2 |= BM_2_AVAILABILITY;
        }
        else if (temp[0] == bm_3_thermometer[0] && temp[1] == bm_3_thermometer[1] && temp[2] == bm_3_thermometer[2] && temp[3] == bm_3_thermometer[3])
        {
            sensor_availability_2 |= BM_3_AVAILABILITY;
        }
        else if (temp[0] == bm_4_thermometer[0] && temp[1] == bm_4_thermometer[1] && temp[2] == bm_4_thermometer[2] && temp[3] == bm_4_thermometer[3])
        {
            sensor_availability_2 |= BM_4_AVAILABILITY;
        }
        else
        {
            Serial.println("Unknown sensor found!");
            return 0;
        }
    }

    //Is always available as it is the buildin sensor
    sensor_availability_1 |= LLC_AVAILABILITY;

    if (sensor_availability_1 & HLC_AVAILABILITY)
        sensors.setResolution(hlc_thermometer, TEMPERATURE_PRECISION);

    if (sensor_availability_1 & SS_AVAILABILITY)
        sensors.setResolution(ss_thermometer, TEMPERATURE_PRECISION);

    if (sensor_availability_1 & BP_AVAILABILITY)
        sensors.setResolution(bp_thermometer, TEMPERATURE_PRECISION);

    if (sensor_availability_2 & ESC_1_AVAILABILITY)
        sensors.setResolution(esc_1_thermometer, TEMPERATURE_PRECISION);

    if (sensor_availability_2 & ESC_2_AVAILABILITY)
        sensors.setResolution(esc_2_thermometer, TEMPERATURE_PRECISION);

    if (sensor_availability_2 & ESC_3_AVAILABILITY)
        sensors.setResolution(esc_3_thermometer, TEMPERATURE_PRECISION);

    if (sensor_availability_2 & ESC_4_AVAILABILITY)
        sensors.setResolution(esc_4_thermometer, TEMPERATURE_PRECISION);

    if (sensor_availability_2 & BM_1_AVAILABILITY)
        sensors.setResolution(bm_1_thermometer, TEMPERATURE_PRECISION);

    if (sensor_availability_2 & BM_2_AVAILABILITY)
        sensors.setResolution(bm_2_thermometer, TEMPERATURE_PRECISION);

    if (sensor_availability_2 & BM_3_AVAILABILITY)
        sensors.setResolution(bm_3_thermometer, TEMPERATURE_PRECISION);

    if (sensor_availability_2 & BM_4_AVAILABILITY)
        sensors.setResolution(bm_4_thermometer, TEMPERATURE_PRECISION);

    sensors.setWaitForConversion(false); // No delays

    internal_temp = 0.0;
    tempmon_init();

    return 1;
}

void Temperature::read()
{
    sensors.setWaitForConversion(false);
    sensors.requestTemperatures();
    internal_temp = tempmonGetTemp();
}

float Temperature::getBySource(temperature_source source)
{
    switch (source)
    {
    case Temperature::hlc:
        if (sensor_availability_1 & HLC_AVAILABILITY)
            return sensors.getTempC(hlc_thermometer);

        return -127;
        break;

    case Temperature::llc:
        if (sensor_availability_1 & LLC_AVAILABILITY)
            return internal_temp;
        break;

    case Temperature::ss:
        if (sensor_availability_1 & SS_AVAILABILITY)
            return sensors.getTempC(ss_thermometer);
        return -127; //-127 means not available
        break;

    case Temperature::bp:
        if (sensor_availability_1 & BP_AVAILABILITY)
            return sensors.getTempC(bp_thermometer);
        return -127; //-127 means not available
        break;

    case Temperature::esc_1:
        if (sensor_availability_2 & ESC_1_AVAILABILITY)
            return sensors.getTempC(esc_1_thermometer);
        return -127; //-127 means not available
        break;

    case Temperature::esc_2:
        if (sensor_availability_2 & ESC_2_AVAILABILITY)
            return sensors.getTempC(esc_2_thermometer);
        return -127; //-127 means not available
        break;

    case Temperature::esc_3:
        if (sensor_availability_2 & ESC_3_AVAILABILITY)
            return sensors.getTempC(esc_3_thermometer);
        return -127; //-127 means not available
        break;

    case Temperature::esc_4:
        if (sensor_availability_2 & ESC_4_AVAILABILITY)
            return sensors.getTempC(esc_4_thermometer);
        return -127; //-127 means not available
        break;
    case Temperature::bm_1:
        if (sensor_availability_2 & BM_1_AVAILABILITY)
            return sensors.getTempC(bm_1_thermometer);
        return -127; //-127 means not available
        break;
    case Temperature::bm_2:
        if (sensor_availability_2 & BM_2_AVAILABILITY)
            return sensors.getTempC(bm_2_thermometer);
        return -127; //-127 means not available
        break;

    case Temperature::bm_3:
        if (sensor_availability_2 & BM_3_AVAILABILITY)
            return sensors.getTempC(bm_3_thermometer);
        return -127; //-127 means not available
        break;

    case Temperature::bm_4:
        if (sensor_availability_2 & BM_4_AVAILABILITY)
            return sensors.getTempC(bm_4_thermometer);
        return -127; //-127 means not available
        break;

    default:
        break;
    }
    return -127;
}