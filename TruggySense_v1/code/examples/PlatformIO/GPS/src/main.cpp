#include <TinyGPSPlus.h>
#include "gps.h"

GPSDriver gps;

void displayInfo();

#define GPS_RATE 1000
#define GPS_READ_RATE 1
long timer = 0;
long timer2 = 0;

long temp_timer = 0;

void setup()
{
  Serial.begin(115200);
  gps.init();
  timer = millis();
}

void loop()
{
  //gps.readGps();
  // This sketch displays information every time a new sentence is correctly encoded.
  if (millis()-timer > GPS_RATE)
  {
    temp_timer = micros();
    
    //gps.readGps();
    
    long diff = micros()- temp_timer;
    Serial.println(diff);

    timer = millis();
    displayInfo();
  }

  if (millis()-timer2 > GPS_READ_RATE)
  {
    timer2 = millis();

    gps.readGps();
  }
 
}

void displayInfo()
{
  Serial.print(F("Location: "));

  Serial.print(gps.getLatitude(), 6);
  Serial.print(F(","));
  Serial.print(gps.getLongitude(), 6);

  Serial.print(F("  Date/Time: "));

  Serial.print(gps.getDate());
  Serial.print(F(" "));

  Serial.print(gps.getTime());

  Serial.print(F(" "));
  Serial.print(F("SPEED: "));
  Serial.print(gps.getVelocity(), 2);

  Serial.print(F(" "));
  Serial.print(F("SATELITES: "));

  Serial.print(gps.getConnectedSatelites());

  Serial.print(F(" "));
  Serial.print(F("ALTI: "));

  Serial.print(gps.getAltitude());

  Serial.println();
}