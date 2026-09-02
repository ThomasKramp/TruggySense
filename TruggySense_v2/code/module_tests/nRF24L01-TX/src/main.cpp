/*
* Arduino Wireless Communication Tutorial
*     Example 1 - Transmitter Code
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
  digitalWrite(LED_BUILTIN, HIGH);
  
  // while (!Serial) { }
  Serial.println("Transmitter on");
  digitalWrite(LED_BUILTIN, LOW);
  
  radio.begin();
  if (!radio.isChipConnected()) {
    Serial.println("nRF24L01 not detected! Check wiring/power.");
    // while (1);  // halt
  } else
    Serial.println("Chip connected OK");
  radio.openWritingPipe(address);
  //radio.setCRCLength(RF24_CRC_16);
  radio.setPALevel(RF24_PA_MIN);
  //radio.setPayloadSize(64);
  radio.setChannel(76);
  //radio.setAutoAck(false);
  Serial.println("=== Radio Config ===");
  radio.printDetails();   // dumps all registers
  Serial.println("=== Sending ===");
  radio.stopListening();
}

void loop() {
  char text[64] = "Ewa Broeder 2: Electric Boogaloo";
  radio.flush_tx();
  delayMicroseconds(200);
  bool result = radio.write(&text, sizeof(text));
  Serial.print("Write:\t");
  Serial.println(text);
  //Serial.println(result ? "Send OK" : "Send FAILED");
  delay(1000);
}