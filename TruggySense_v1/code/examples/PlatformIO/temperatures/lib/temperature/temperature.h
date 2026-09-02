#pragma once

#include <stdint.h>

#define ONE_WIRE_BUS 6
#define TEMPERATURE_PRECISION 12

#include <OneWire.h>
#include <DallasTemperature.h>

#define HLC_AVAILABILITY (1 << 0)
#define LLC_AVAILABILITY (1 << 1)
#define SS_AVAILABILITY (1 << 2)
#define BP_AVAILABILITY (1 << 3)

#define BM_1_AVAILABILITY (1 << 0)
#define BM_2_AVAILABILITY (1 << 1)
#define BM_3_AVAILABILITY (1 << 2)
#define BM_4_AVAILABILITY (1 << 3)
#define ESC_1_AVAILABILITY (1 << 4)
#define ESC_2_AVAILABILITY (1 << 5)
#define ESC_3_AVAILABILITY (1 << 6)
#define ESC_4_AVAILABILITY (1 << 7)

enum temperature_source
{
    hlc,    //High level controller
    llc,    //Low level controller
    ss,     //Steering Servo
    bp,     //Battery pack
    esc_1,  //ESC 1
    esc_2,  //ESC 2
    esc_3,  //ESC 3
    esc_4,  //ESC 4
    bm_1,   //Brushless motor 1
    bm_2,   //Brushless motor 2
    bm_3,   //Brushless motor 3
    bm_4,   //Brushless motor 4
};
typedef enum temperature_source e_temperature_source_t;

void initTemperature();
void readTemperature();
float getTemperatureBySource(e_temperature_source_t source);