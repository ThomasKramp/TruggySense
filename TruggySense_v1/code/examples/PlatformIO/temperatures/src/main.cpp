//TOGGLE BETWEEN SEARCH AND LIB
//#define SEARCH_SENSORS


#ifndef SEARCH_SENSORS
#include "temperature.h"

void setup(){
  initTemperature();
}

void loop(){
  readTemperature();
  delay(1000);

  Serial.print("battery pack: ");
  Serial.print(getTemperatureBySource(bp));
  Serial.print(" llc: ");
  Serial.print(getTemperatureBySource(llc));
  Serial.print(" esc-1: ");
  Serial.print(getTemperatureBySource(esc_1));
  Serial.print(" esc-2: ");
  Serial.print(getTemperatureBySource(esc_2));
  Serial.print(" esc-3: ");
  Serial.print(getTemperatureBySource(esc_3));
  Serial.print(" esc-4: ");
  Serial.print(getTemperatureBySource(esc_4));
  Serial.print(" bm-1: ");
  Serial.print(getTemperatureBySource(bm_1));
  Serial.print(" bm-2: ");
  Serial.print(getTemperatureBySource(bm_2));
  Serial.print(" bm-3: ");
  Serial.print(getTemperatureBySource(bm_3));
  Serial.print(" bm-4: ");
  Serial.print(getTemperatureBySource(bm_4));
  Serial.print(" hlc: ");
  Serial.print(getTemperatureBySource(hlc));
  Serial.print(" servo: ");
  Serial.println(getTemperatureBySource(ss));
}


#endif

#ifdef SEARCH_SENSORS

#include <OneWire.h>

uint8_t findDevices(int pin);

void setup()
{
  Serial.begin(9600);
  Serial.println("//\n// Start oneWireSearch \n//");

  for (uint8_t pin = 6; pin < 7; pin++)
  {
    findDevices(pin);
  }
  Serial.println("\n//\n// End oneWireSearch \n//");
}

void loop()
{
}

uint8_t findDevices(int pin)
{
  OneWire ow(pin);

  uint8_t address[8];
  uint8_t count = 0;


  if (ow.search(address))
  {
    Serial.print("\nuint8_t pin");
    Serial.print(pin, DEC);
    Serial.println("[][8] = {");
    do {
      count++;
      Serial.println("  {");
      for (uint8_t i = 0; i < 8; i++)
      {
        Serial.print("0x");
        if (address[i] < 0x10) Serial.print("0");
        Serial.print(address[i], HEX);
        if (i < 7) Serial.print(", ");
      }
      Serial.println("  },");
    } while (ow.search(address));

    Serial.println("};");
    Serial.print("// nr devices found: ");
    Serial.println(count);
  }

  return count;
}
#endif