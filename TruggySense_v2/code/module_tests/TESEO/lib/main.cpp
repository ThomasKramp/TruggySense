void setup() {
  Serial.begin(9600);   // USB, for output to PC
  Serial5.begin(9600);  // Hardware UART, pin 0 = RX
}

void loop() {
  if (Serial1.available()) {
    char c = Serial1.read();
    Serial.write(c);  // forward to USB serial monitor
  }
}