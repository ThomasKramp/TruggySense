/*
* Arduino Wireless Communication Tutorial
*       Example 1 - Receiver Code
*                
* by Dejan Nedelkovski, www.HowToMechatronics.com
* 
* Library: TMRh20/RF24, https://github.com/tmrh20/RF24/
*/

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9, 10, 4000000);  // CE, CSN, SPI speed
// Limit SPI to 4MHz for nRF24L01 compatibility

const byte address[6] = "00001";

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  
  // while (!Serial) { }
  Serial.println("Receiver on");
  digitalWrite(LED_BUILTIN, HIGH);
  
  radio.begin();
  radio.flush_rx();
  if (!radio.isChipConnected()) {
    Serial.println("nRF24L01 not detected! Check wiring/power.");
    // while (1);  // halt
  } else
    Serial.println("Chip connected OK");
  radio.openReadingPipe(1, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.setChannel(76);
  // radio.setAutoAck(false);
  // radio.setPayloadSize(64);
  // radio.setCRCLength(RF24_CRC_16);
  Serial.println("=== Radio Config ===");
  radio.printDetails();   // dumps all registers
  Serial.println("=== Listening ===");
  radio.startListening();
}

void loop() {
  // Serial.println("Listening...");  // temporary debug line
  while (radio.available()) {
    char text[64] = "";
    radio.read(&text, sizeof(text));
    if (strlen(text) == 0) {
      Serial.println("Empty string received");
      break;
    } else {
      Serial.println("Length: " + strlen(text));
      Serial.println("\tContents: " + String(text));
    }
  }
  if (!radio.available())
    Serial.println("No signal");
  delay(500);
}