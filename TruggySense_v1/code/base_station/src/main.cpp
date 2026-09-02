#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

struct logging_data
{
  uint32_t timestamp;     // The timestamp of the data
  uint32_t date;          // The date aquired from a satellite
  uint32_t time;          // The time aquired from a satellite
  float t_llc;        // Temperature of low level controller
  float t_hlc;        // Temperature of high level controller
  float t_bp;         // Temperature of battery pack
  float t_ss;         // Temperature of steering servo motor
  float t_bm_1;       // Temperature of brushless motor 1
  float t_bm_2;       // Temperature of brushless motor 2
  float t_bm_3;       // Temperature of brushless motor 3
  float t_bm_4;       // Temperature of brushless motor 4
  float t_esc_1;      // Temperature of Electric Speed Controller 1
  float t_esc_2;      // Temperature of Electric Speed Controller 2
  float t_esc_3;      // Temperature of Electric Speed Controller 3
  float t_esc_4;      // Temperature of Electric Speed Controller 4
  float i_ss;         // Current usgae of steering servo
  float i_llc;        // Current usage of low level controller
  float i_hlc;        // Current usage of high level controller
  float roll;         // roll defines rolling
  float pitch;        // pitch defines nose up or down
  float yaw;          // yaw defines left right swing
  float w;            // real part of quaternion
  float i;            // i part
  float j;            // j part
  float k;            // k part
  float rpm_wheel_1;  // The RPM of wheel 1
  float rpm_wheel_2;  // The RPM of wheel 2
  float rpm_wheel_3;  // The RPM of wheel 3
  float rpm_wheel_4;  // The RPM of wheel 4
  float rpm_bm_1;     // The RPM of motor 1
  float rpm_bm_2;     // The RPM of motor 2
  float rpm_bm_3;     // The RPM of motor 3
  float rpm_bm_4;     // The RPM of motor 4
  float latitude;     // The latitude
  float longitude;    // the longitide
  float velocity;     // the relative velocity
  float altitude;     // the altitude based on received satellites distances
  float v_cell_1;     // Battery cell voltage 1.
  float v_cell_2;     // Battery cell voltage 2.
  float v_cell_3;     // Battery cell voltage 3.
  float v_cell_4;     // Battery cell voltage 4.
  float v_batt;       // Battery voltage.
  float v_llc;        // VOltage of low level controllers power supply.
  float v_hlc;        // Voltage of high level controllers power supply.
  float v_phrp_3v3;   // Voltage of pheripherals that use 3.3V
  uint16_t ch_1;      // Channel 1 that ranges from 1000 to 2000 (ms)
  uint16_t ch_2;      // Channel 2 that ranges from 1000 to 2000 (ms)
  uint16_t ch_3;      // Channel 3 that ranges from 1000 to 2000 (ms)
  uint16_t ch_4;      // Channel 4 that ranges from 1000 to 2000 (ms)
  uint16_t ch_5;      // Channel 5 that ranges from 1000 to 2000 (ms)
  uint16_t ch_6;      // Channel 6 that ranges from 1000 to 2000 (ms)
  uint16_t ch_7;      // Channel 7 that ranges from 1000 to 2000 (ms)
  uint16_t ch_8;      // Channel 8 that ranges from 1000 to 2000 (ms)
  uint16_t ch_9;      // Channel 9 that ranges from 1000 to 2000 (ms)
  uint16_t ch_10;     // Channel 10 that ranges from 1000 to 2000 (ms)
  uint16_t ch_11;     // Channel 11 that ranges from 1000 to 2000 (ms)
  uint16_t ch_12;     // Channel 12 that ranges from 1000 to 2000 (ms)
  uint16_t ch_13;     // Channel 13 that ranges from 1000 to 2000 (ms)
  uint16_t ch_14;     // Channel 14 that ranges from 1000 to 2000 (ms)
  uint16_t pwm_bm_1;  // PWM value from 1000 to 2000 (ms) for brushless motor 1
  uint16_t pwm_bm_2;  // PWM value from 1000 to 2000 (ms) for brushless motor 2
  uint16_t pwm_bm_3;  // PWM value from 1000 to 2000 (ms) for brushless motor 3
  uint16_t pwm_bm_4;  // PWM value from 1000 to 2000 (ms) for brushless motor 4
  uint16_t pwm_ss;    // PWM value from 1000 to 2000 (ms) for steering servo
  uint8_t c_s;        // Connected satellites
  uint8_t failsafe; // when failsafe is active or not. 1 if not active, 0 if active.
  uint8_t dead_switch;// A deadswitch on the transmitter side if something goes wrong
  uint8_t i_esc_1;    // Current usage of esc 1
  uint8_t i_esc_2;    // Current usage of esc 2
  uint8_t i_esc_3;    // Current usage of esc 3
  uint8_t i_esc_4;    // Current usage of esc 4
};
typedef struct logging_data logging_data_t;

RF24 radio(10, 28); // CE, CSN

const byte path[6] = "00001";

constexpr int payload_length = 228;
constexpr int chunks_count = 10;
constexpr int chunk_size = 30 + 1;

logging_data_t data;
uint8_t buffer[chunks_count][chunk_size] = {0};


void toPayload(uint8_t chunked_payload[chunks_count][chunk_size], uint8_t* payload);

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, path);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
}

void loop()
{
  uint8_t pack_num = 0;
  uint8_t reading = 1;
  long timeout_timer = millis();

  while (reading)
  {
    if (radio.available()) {
      char text[chunk_size] = {0};
      radio.read(&text, chunk_size);

      pack_num = (int)text[0];
      for (uint8_t i = 0; i < chunk_size; i++)
      {
        buffer[pack_num][i] = text[i];
      }
    }
    else {
      //timeout
      if (pack_num == 0)
      {
        reading = 0;
        break;
      }

      if (millis() - timeout_timer > 500)
      {
        Serial.println("Timeout occured!");
        reading = 0;
        break;
      }
    }

    //pack data again
    if (pack_num == (chunks_count - 1))
    {
      uint8_t rx_buffer[sizeof(logging_data)];
      toPayload(buffer, rx_buffer);

      logging_data* temp = (logging_data*)rx_buffer;

      data = *temp;
      reading = 0;

      String jsonString = "{";
      jsonString += "\"timestamp\": \"" + String(data.timestamp) + "\",";
      jsonString += "\"date\": \"" + String(data.date) + "\",";
      jsonString += "\"time\": \"" + String(data.time) + "\",";
      jsonString += "\"t_llc\": \"" + String(data.t_llc, 6) + "\",";
      jsonString += "\"t_hlc\": \"" + String(data.t_hlc, 6) + "\",";
      jsonString += "\"t_bp\": \"" + String(data.t_bp, 6) + "\",";
      jsonString += "\"t_ss\": \"" + String(data.t_ss, 6) + "\",";
      jsonString += "\"t_bm_1\": \"" + String(data.t_bm_1, 6) + "\",";
      jsonString += "\"t_bm_2\": \"" + String(data.t_bm_2, 6) + "\",";
      jsonString += "\"t_bm_3\": \"" + String(data.t_bm_3, 6) + "\",";
      jsonString += "\"t_bm_4\": \"" + String(data.t_bm_4, 6) + "\",";
      jsonString += "\"t_esc_1\": \"" + String(data.t_esc_1, 6) + "\",";
      jsonString += "\"t_esc_2\": \"" + String(data.t_esc_2, 6) + "\",";
      jsonString += "\"t_esc_3\": \"" + String(data.t_esc_3, 6) + "\",";
      jsonString += "\"t_esc_4\": \"" + String(data.t_esc_4, 6) + "\",";
      jsonString += "\"i_ss\": \"" + String(data.i_ss, 10) + "\",";
      jsonString += "\"i_llc\": \"" + String(data.i_llc, 10) + "\",";
      jsonString += "\"i_hlc\": \"" + String(data.i_hlc, 10) + "\",";
      jsonString += "\"roll\": \"" + String(data.roll, 6) + "\",";
      jsonString += "\"pitch\": \"" + String(data.pitch, 6) + "\",";
      jsonString += "\"yaw\": \"" + String(data.yaw, 6) + "\",";
      jsonString += "\"w\": \"" + String(data.w, 6) + "\",";
      jsonString += "\"i\": \"" + String(data.i, 6) + "\",";
      jsonString += "\"j\": \"" + String(data.j, 6) + "\",";
      jsonString += "\"k\": \"" + String(data.k, 6) + "\",";
      jsonString += "\"rpm_wheel_1\": \"" + String(data.rpm_wheel_1, 6) + "\",";
      jsonString += "\"rpm_wheel_2\": \"" + String(data.rpm_wheel_2, 6) + "\",";
      jsonString += "\"rpm_wheel_3\": \"" + String(data.rpm_wheel_3, 6) + "\",";
      jsonString += "\"rpm_wheel_4\": \"" + String(data.rpm_wheel_4, 6) + "\",";
      jsonString += "\"rpm_bm_1\": \"" + String(data.rpm_bm_1, 6) + "\",";
      jsonString += "\"rpm_bm_2\": \"" + String(data.rpm_bm_2, 6) + "\",";
      jsonString += "\"rpm_bm_3\": \"" + String(data.rpm_bm_3, 6) + "\",";
      jsonString += "\"rpm_bm_4\": \"" + String(data.rpm_bm_4, 6) + "\",";
      jsonString += "\"latitude\": \"" + String(data.latitude, 6) + "\",";
      jsonString += "\"longitude\": \"" + String(data.longitude, 6) + "\",";
      jsonString += "\"velocity\": \"" + String(data.velocity, 6) + "\",";
      jsonString += "\"altitude\": \"" + String(data.altitude, 6) + "\",";
      jsonString += "\"v_cell_1\": \"" + String(data.v_cell_1, 10) + "\",";
      jsonString += "\"v_cell_2\": \"" + String(data.v_cell_2, 10) + "\",";
      jsonString += "\"v_cell_3\": \"" + String(data.v_cell_3, 10) + "\",";
      jsonString += "\"v_cell_4\": \"" + String(data.v_cell_4, 10) + "\",";
      jsonString += "\"v_batt\": \"" + String(data.v_batt, 10) + "\",";
      jsonString += "\"v_llc\": \"" + String(data.v_llc, 10) + "\",";
      jsonString += "\"v_hlc\": \"" + String(data.v_hlc, 10) + "\",";
      jsonString += "\"v_phrp_3v3\": \"" + String(data.v_phrp_3v3, 10) + "\",";
      jsonString += "\"ch_1\": \"" + String(data.ch_1) + "\",";
      jsonString += "\"ch_2\": \"" + String(data.ch_2) + "\",";
      jsonString += "\"ch_3\": \"" + String(data.ch_3) + "\",";
      jsonString += "\"ch_4\": \"" + String(data.ch_4) + "\",";
      jsonString += "\"ch_5\": \"" + String(data.ch_5) + "\",";
      jsonString += "\"ch_6\": \"" + String(data.ch_6) + "\",";
      jsonString += "\"ch_7\": \"" + String(data.ch_7) + "\",";
      jsonString += "\"ch_8\": \"" + String(data.ch_8) + "\",";
      jsonString += "\"ch_9\": \"" + String(data.ch_9) + "\",";
      jsonString += "\"ch_10\": \"" + String(data.ch_10) + "\",";
      jsonString += "\"ch_11\": \"" + String(data.ch_11) + "\",";
      jsonString += "\"ch_12\": \"" + String(data.ch_12) + "\",";
      jsonString += "\"ch_13\": \"" + String(data.ch_13) + "\",";
      jsonString += "\"ch_14\": \"" + String(data.ch_14) + "\",";
      jsonString += "\"pwm_bm_1\": \"" + String(data.pwm_bm_1) + "\",";
      jsonString += "\"pwm_bm_2\": \"" + String(data.pwm_bm_2) + "\",";
      jsonString += "\"pwm_bm_3\": \"" + String(data.pwm_bm_3) + "\",";
      jsonString += "\"pwm_bm_4\": \"" + String(data.pwm_bm_4) + "\",";
      jsonString += "\"pwm_ss\": \"" + String(data.pwm_ss) + "\",";
      jsonString += "\"n_satellites\": \"" + String(data.c_s) + "\",";
      jsonString += "\"failsafe\": \"" + String(data.failsafe) + "\",";
      jsonString += "\"dead_switch\": \"" + String(data.dead_switch) + "\",";
      jsonString += "\"i_esc_1\": \"" + String(data.i_esc_1) + "\",";
      jsonString += "\"i_esc_2\": \"" + String(data.i_esc_2) + "\",";
      jsonString += "\"i_esc_3\": \"" + String(data.i_esc_3) + "\",";
      jsonString += "\"i_esc_4\": \"" + String(data.i_esc_4) + "\"";
      jsonString += "}";

      Serial.println(jsonString);

      break;
    }
  }
  //Serial.println("ewa");
}

void toPayload(uint8_t chunked_payload[chunks_count][chunk_size], uint8_t* payload)
{
  int buffer_index = 0;
  for (int i = 0; i < chunks_count; i++)
  {
    for (int j = 1; j < chunk_size; j++)
    {
      if (buffer_index < payload_length)
      {
        payload[buffer_index] = buffer[i][j];
        buffer_index++;
      }
    }
  }
}