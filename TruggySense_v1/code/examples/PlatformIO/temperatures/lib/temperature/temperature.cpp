#include "temperature.h"
#include <Arduino.h>

DeviceAddress hlc_thermometer = {0x28, 0x6E, 0x85, 0x71, 0x10, 0x0, 0x0, 0xBA};
DeviceAddress ss_termometer = {0x28, 0x7B, 0x47, 0x72, 0x10, 0x0, 0x0, 0x23};
DeviceAddress esc_1_thermometer = {0x28, 0xDB, 0xC3, 0x71, 0x10, 0x0, 0x0, 0x23};
DeviceAddress esc_2_thermometer = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
DeviceAddress esc_3_thermometer = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
DeviceAddress esc_4_thermometer = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
DeviceAddress bm_1_thermometer = {0x28, 0xE7, 0x39, 0x72, 0x10, 0x0, 0x0, 0x20};
DeviceAddress bm_2_thermometer = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
DeviceAddress bm_3_thermometer = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
DeviceAddress bm_4_thermometer = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
DeviceAddress bp_thermometer = {0x28, 0xE5, 0x49, 0x72, 0x10, 0x00, 0x00, 0x23};

// // Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// // Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

float internal_temp = 0.0;
uint8_t sensor_availability_1 = 0;
uint8_t sensor_availability_2 = 0;

void initTemperature()
{
    sensor_availability_1 = 0;
    sensor_availability_2 = 0;
    sensors.begin(); // implicit reset_search()

    // Search if addresses are present on the bus
    if (oneWire.search(hlc_thermometer, false))
        sensor_availability_1 |= HLC_AVAILABILITY;

    if (oneWire.search(bp_thermometer))
        sensor_availability_1 |= BP_AVAILABILITY;

    sensor_availability_1 |= LLC_AVAILABILITY;

    if (oneWire.search(ss_termometer))
        sensor_availability_1 |= SS_AVAILABILITY;

    if (oneWire.search(esc_1_thermometer))
        sensor_availability_2 |= ESC_1_AVAILABILITY;

    // if (oneWire.search(esc_2_thermometer))
    //     sensor_availability_2 |= ESC_2_AVAILABILITY;

    // if (oneWire.search(esc_3_thermometer))
    //     sensor_availability_2 |= ESC_3_AVAILABILITY;

    // if (oneWire.search(esc_4_thermometer))
    //     sensor_availability_2 |= ESC_4_AVAILABILITY;

    if (oneWire.search(bm_1_thermometer))
        sensor_availability_2 |= BM_1_AVAILABILITY;

    // if (oneWire.search(bm_2_thermometer))
    //     sensor_availability_2 |= BM_2_AVAILABILITY;

    // if (oneWire.search(bm_3_thermometer))
    //     sensor_availability_2 |= BM_3_AVAILABILITY;

    // if (oneWire.search(bm_4_thermometer))
    //     sensor_availability_2 |= BM_4_AVAILABILITY;

    // Set settings based on the availability
    if (sensor_availability_1 & HLC_AVAILABILITY)
        sensors.setResolution(hlc_thermometer, TEMPERATURE_PRECISION);

    if (sensor_availability_1 & SS_AVAILABILITY)
        sensors.setResolution(ss_termometer, TEMPERATURE_PRECISION);

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

    internal_temp = 0.0;
    tempmon_init();
}

void readTemperature()
{
    Serial.print("Requesting temperatures...");
    long timer = millis();
    sensors.requestTemperatures();
    long total_time = millis() - timer;
    Serial.print("Time:");
    Serial.print(total_time);
    Serial.println("  DONE");

    internal_temp = tempmonGetTemp();
}

float getTemperatureBySource(e_temperature_source_t source)
{
    switch (source)
    {
    case hlc:
        if (sensor_availability_1 & HLC_AVAILABILITY)
        {
            float tempC = sensors.getTempC(hlc_thermometer);
            if (tempC == DEVICE_DISCONNECTED_C)
            {
                Serial.println("Error: Could not read temperature data");
                return -100;
            }
            return tempC;
        }
        // return sensors.getTempC(hlc_thermometer);
        return -90; //-90 means not available
        break;

    case llc:
        if (sensor_availability_1 & LLC_AVAILABILITY)
            return internal_temp;
        break;

    case ss:
        if (sensor_availability_1 & SS_AVAILABILITY)
            return sensors.getTempC(ss_termometer);
        return -90; //-90 means not available
        break;

    case bp:
        if (sensor_availability_1 & BP_AVAILABILITY)
            return sensors.getTempC(bp_thermometer);
        return -90; //-90 means not available
        break;

    case esc_1:
        if (sensor_availability_2 & ESC_1_AVAILABILITY)
            return sensors.getTempC(esc_1_thermometer);
        return -90; //-90 means not available
        break;

    case esc_2:
        if (sensor_availability_2 & ESC_2_AVAILABILITY)
            return sensors.getTempC(esc_2_thermometer);
        return -90; //-90 means not available
        break;

    case esc_3:
        if (sensor_availability_2 & ESC_3_AVAILABILITY)
            return sensors.getTempC(esc_3_thermometer);
        return -90; //-90 means not available
        break;

    case esc_4:
        if (sensor_availability_2 & ESC_4_AVAILABILITY)
            return sensors.getTempC(esc_4_thermometer);
        return -90; //-90 means not available
        break;
    case bm_1:
        if (sensor_availability_2 & BM_1_AVAILABILITY)
            return sensors.getTempC(bm_1_thermometer);
        return -90; //-90 means not available
        break;
    case bm_2:
        if (sensor_availability_2 & BM_2_AVAILABILITY)
            return sensors.getTempC(bm_2_thermometer);
        return -90; //-90 means not available
        break;

    case bm_3:
        if (sensor_availability_2 & BM_3_AVAILABILITY)
            return sensors.getTempC(bm_3_thermometer);
        return -90; //-90 means not available
        break;

    case bm_4:
        if (sensor_availability_2 & BM_4_AVAILABILITY)
            return sensors.getTempC(bm_4_thermometer);
        return -90; //-90 means not available
        break;

    default:
        break;
    }
    return -100; //-100 means that the conversion is not done yet.
}