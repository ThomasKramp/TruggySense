/*
 * SD card datalogger
 *
 * This example shows how to log data to and delete data from an SD card using the SD library.
 * 
 * The circuit: (SD card attached to SPI bus as follows)
 * ** MOSI - pin 11
 * ** MISO - pin 12
 * ** CLK  - pin 13
 * ** CS   - pin 4
 * 
 * Created  24 Nov 2010     by Tom Igoe
 * Modified 06 Mar 2026     by Thomas K.
 * 
 * This example code is in the public domain.
 * 
 * Source:  https://forum.pjrc.com/index.php?threads/how-to-use-the-teensy-4-1-microsd-card-a-tutorial-for-beginners.62433/post-249341
 */

#include "Arduino.h"
#include "SD.h"
#include "SPI.h"

#ifndef LED_BUILTIN
  #define LED_BUILTIN 13
#endif

const uint8_t chipSelect = BUILTIN_SDCARD; 

uint8_t counter = 0;
String dataString = "";

void setup() {
    Serial.begin (115200);
    // initialize LED digital pin as an output.
    pinMode(LED_BUILTIN, OUTPUT);

    // Turn led on to indicate Ready state
    digitalWrite(LED_BUILTIN, HIGH);

    // Leonardo: wait for serial port to connect
    while (!Serial) { }

    // Turn led off
    digitalWrite(LED_BUILTIN, LOW);

    Serial.print("Initializing SD card...");
  
    // see if the card is present and can be initialized:
    if (!SD.begin(chipSelect)) {
        Serial.println("Card failed, or not present");
        // don't do anything more:
        return;
    }
    Serial.println("card initialized.");
}

void loop() {
    // make a string for assembling the data to log:
    counter += 1;
    dataString = String(counter);

    String filename = "datalog.txt";
    if (SD.exists(filename.c_str())) {
        Serial.println("File exists. Deleting...");
        SD.remove(filename.c_str());
    }

    for (uint8_t i = 0; i < 255; i++) {
        filename = "llc_data_" + String(i) + ".bin";
        if (SD.exists(filename.c_str())) {
            Serial.println("File exists. Deleting...");
            SD.remove(filename.c_str());
        }
        filename = "llc_data_" + String(i) + ".csv";
        if (SD.exists(filename.c_str())) {
            Serial.println("File exists. Deleting...");
            SD.remove(filename.c_str());
        }
        filename = "llc_data_" + String(i) + ".json";
        if (SD.exists(filename.c_str())) {
            Serial.println("File exists. Deleting...");
            SD.remove(filename.c_str());
        }
    }

    //------------------------------------------------------------------
    // open the file named datalog.txt on the sd card
    File dataFile = SD.open("datalog.txt", FILE_WRITE);

    // if the file is available, write the contents of datastring to it
    if (dataFile) {
        dataFile.println(dataString);
        dataFile.close();

    // if the file isn't open, pop up an error:
    } else {
        Serial.println("error opening datalog.txt");
    }

    //------------------------------------------------------------------
    //open up datalog.txt and then print all of its contents
    // File dataFile = SD.open("datalog.txt");
    // if(dataFile) {
    //     Serial.println("datalog:");
    //     while (dataFile.available()) {
    //         Serial.write(dataFile.read());
    //     }
    //     // close the file:
    //     dataFile.close();

    //     //Delete the file so it can be created again at the begining of the loop
    //     SD.remove("datalog.txt");
    // } else {
    //     // if the file didn't open, print an error:
    //     Serial.println("error opening datalog.txt");
    // }

    delay(1000);
}