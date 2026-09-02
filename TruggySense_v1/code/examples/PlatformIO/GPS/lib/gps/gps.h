#pragma once
#include <Arduino.h>
#include <TinyGPSPlus.h>

#define BAUDRATE 115200



class GPSDriver
{
private:
    long lat;
    long lon;
    uint8_t sat_count;
    long date;
    long time;
    float speed;
    float altitude;
    
public:
    uint8_t init();         //initialize gps
    uint8_t readGps();      //read serial string if available
    uint8_t getVelocity();  //The velocity or speed based on received satellite information in m/s
    float getLatitude();    // in degree
    float getLongitude();   // in degree
    uint32_t getDate();     //DDMMYY
    uint32_t getTime();     //HHMMSSmm
    int16_t getAltitude();  //The altitude based on received satellite information. Can only be valid if there is a 3D fix.
    uint8_t getConnectedSatelites();    //The number of received satellites
};
