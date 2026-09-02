#define START_FRAME_SEQUENCE 0x2040


void setup() {
  // put your setup code here, to run once:
  pinMode(13, OUTPUT);
  Serial.begin(115200);
  Serial8.begin(115200, SERIAL_8N1);
  Serial.println("Testing");
}

byte input;
bool pre_start_frame = false;
bool start_frame_detected = true;

const byte frame_length = 30;
uint32_t buffer[frame_length];
uint8_t buffer_index = 0;

void loop() {
  while (Serial8.available() > 0) {
    input = Serial8.read();

    if(input == (START_FRAME_SEQUENCE & 0xFF00)>>8){
      pre_start_frame = true;
      //Serial.println("Header?");
    }

    if(pre_start_frame && input == (START_FRAME_SEQUENCE & 0xFF))
    {
      //Serial.println("Header!");
      pre_start_frame = false;
      start_frame_detected = true;
    }

    if(start_frame_detected && input != (START_FRAME_SEQUENCE & 0xFF))
    {
      buffer[buffer_index] = input;
      buffer_index++;

      if(buffer_index == frame_length){
        start_frame_detected = false;
        buffer_index = 0;
        Serial.print(buffer[0]);
        Serial.print(" ");
        Serial.print(buffer[1]);
        Serial.print(" ch1: ");
        Serial.println(buffer[0] | buffer[1]<<8);
      }
    }
  }
}
