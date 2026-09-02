#include "gps.h"

TinyGPSPlus gps_object;

uint8_t GPSDriver::init()
{
    Serial2.begin(BAUDRATE);
    // while(true){
    //     Serial.print(Serial2.read());
    //     delay(100);
    // }

    return 1;
}

uint8_t GPSDriver::readGps()
{
    while (Serial2.available() > 0)
    {
        if (gps_object.encode(Serial2.read()))
        {

            if (millis() > 5000 && gps_object.charsProcessed() < 10)
            {
                Serial.println(F("No GPS detected: check wiring."));
                while (true)
                    ;
            }
        }
    }
    return 0;
}
uint8_t GPSDriver::getVelocity()
{
    return speed;
}
float GPSDriver::getLatitude()
{
    return this->lat;
}
float GPSDriver::getLongitude()
{
    if (gps_object.location.isValid())
        return gps_object.location.lng();
    return 0;
}
uint32_t GPSDriver::getDate()
{
    if (gps_object.date.isValid())
        return gps_object.date.value();
    return 0;
}

uint32_t GPSDriver::getTime()
{
    if (gps_object.time.isValid())
        return gps_object.time.value();
    return 0;
}
int16_t GPSDriver::getAltitude()
{
    if (gps_object.altitude.isValid())
        return gps_object.altitude.meters();
    return 0;
}
uint8_t GPSDriver::getConnectedSatelites()
{
    if (gps_object.satellites.isValid())
        return gps_object.satellites.value();
    return 0;
}