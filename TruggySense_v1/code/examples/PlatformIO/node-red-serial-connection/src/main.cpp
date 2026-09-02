#include <Arduino.h>

uint8_t ticks;

String jsonTemplate = R"({
"timestamp": %d,
"date": %d,
"time": %d,
"t_llc": %.6f,
"t_hlc": %.6f,
"t_bp": %.6f,
"t_ss": %.6f,
"t_bm_1": %.6f,
"t_bm_2": %.6f,
"t_bm_3": %.6f,
"t_bm_4": %.6f,
"t_esc_1": %.6f,
"t_esc_2": %.6f,
"t_esc_3": %.6f,
"t_esc_4": %.6f,
"i_ss": %.10f,
"i_llc": %.10f,
"i_hlc": %.10f,
"roll": %.6f,
"pitch": %.6f,
"yaw": %.6f,
"w": %.6f,
"i": %.6f,
"j": %.6f,
"k": %.6f,
"rpm_wheel_1": %.6f,
"rpm_wheel_2": %.6f,
"rpm_wheel_3": %.6f,
"rpm_wheel_4": %.6f,
"rpm_bm_1": %.6f,
"rpm_bm_2": %.6f,
"rpm_bm_3": %.6f,
"rpm_bm_4": %.6f,
"latitude": %.6f,
"longitude": %.6f,
"velocity": %.6f,
"altitude": %.6f,
"v_cell_1": %.10f,
"v_cell_2": %.10f,
"v_cell_3": %.10f,
"v_cell_4": %.10f,
"v_batt": %.10f,
"v_llc": %.10f,
"v_hlc": %.10f,
"v_phrp_3v3": %.10f,
"ch_1": %d,
"ch_2": %d,
"ch_3": %d,
"ch_4": %d,
"ch_5": %d,
"ch_6": %d,
"ch_7": %d,
"ch_8": %d,
"ch_9": %d,
"ch_10": %d,
"ch_11": %d,
"ch_12": %d,
"ch_13": %d,
"ch_14": %d,
"pwm_bm_1": %d,
"pwm_bm_2": %d,
"pwm_bm_3": %d,
"pwm_bm_4": %d,
"pwm_ss": %d,
"n_satellites": %d,
"n_failsafe": %d,
"dead_switch": %d,
"i_esc_1": %d,
"i_esc_2": %d,
"i_esc_3": %d,
"i_esc_4": %d
})";


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
    uint8_t n_failsafe; // when failsafe is active or not. 1 if not active, 0 if active.
    uint8_t dead_switch;// A deadswitch on the transmitter side if something goes wrong
    uint8_t i_esc_1;    // Current usage of esc 1
    uint8_t i_esc_2;    // Current usage of esc 2
    uint8_t i_esc_3;    // Current usage of esc 3
    uint8_t i_esc_4;    // Current usage of esc 4
};
typedef struct logging_data logging_data_t;

logging_data_t logging_data;

void setup()
{
  Serial.begin(115200);
  // Example values to fill in the JSON template
}

void loop()
{
  delay(500);

  // Serial.print("{ \"custom_data\":\"Hello world!");
  // Serial.print(ticks);
  // Serial.println(" ticks\"}");
  // ticks++;

  logging_data.timestamp = ticks;

  logging_data.roll = (ticks*10)%180;

  logging_data.ch_1 = (ticks*10+950)%2050;
  logging_data.ch_2 = (ticks*5+950)%2050;
  logging_data.ch_3 = (ticks*2+950)%2050;
  logging_data.ch_4 = (ticks*1+950)%2050;

  String jsonString = "{";
  jsonString += "\"timestamp\": \"" + String(logging_data.timestamp) + "\",";
  jsonString += "\"date\": \"" + String(logging_data.date) + "\",";
  jsonString += "\"time\": \"" + String(logging_data.time) + "\",";
  jsonString += "\"t_llc\": \"" + String(logging_data.t_llc, 6) + "\",";
  jsonString += "\"t_hlc\": \"" + String(logging_data.t_hlc, 6) + "\",";
  jsonString += "\"t_bp\": \"" + String(logging_data.t_bp, 6) + "\",";
  jsonString += "\"t_ss\": \"" + String(logging_data.t_ss, 6) + "\",";
  jsonString += "\"t_bm_1\": \"" + String(logging_data.t_bm_1, 6) + "\",";
  jsonString += "\"t_bm_2\": \"" + String(logging_data.t_bm_2, 6) + "\",";
  jsonString += "\"t_bm_3\": \"" + String(logging_data.t_bm_3, 6) + "\",";
  jsonString += "\"t_bm_4\": \"" + String(logging_data.t_bm_4, 6) + "\",";
  jsonString += "\"t_esc_1\": \"" + String(logging_data.t_esc_1, 6) + "\",";
  jsonString += "\"t_esc_2\": \"" + String(logging_data.t_esc_2, 6) + "\",";
  jsonString += "\"t_esc_3\": \"" + String(logging_data.t_esc_3, 6) + "\",";
  jsonString += "\"t_esc_4\": \"" + String(logging_data.t_esc_4, 6) + "\",";
  jsonString += "\"i_ss\": \"" + String(logging_data.i_ss, 10) + "\",";
  jsonString += "\"i_llc\": \"" + String(logging_data.i_llc, 10) + "\",";
  jsonString += "\"i_hlc\": \"" + String(logging_data.i_hlc, 10) + "\",";
  jsonString += "\"roll\": \"" + String(logging_data.roll, 6) + "\",";
  jsonString += "\"pitch\": \"" + String(logging_data.pitch, 6) + "\",";
  jsonString += "\"yaw\": \"" + String(logging_data.yaw, 6) + "\",";
  jsonString += "\"w\": \"" + String(logging_data.w, 6) + "\",";
  jsonString += "\"i\": \"" + String(logging_data.i, 6) + "\",";
  jsonString += "\"j\": \"" + String(logging_data.j, 6) + "\",";
  jsonString += "\"k\": \"" + String(logging_data.k, 6) + "\",";
  jsonString += "\"rpm_wheel_1\": \"" + String(logging_data.rpm_wheel_1, 6) + "\",";
  jsonString += "\"rpm_wheel_2\": \"" + String(logging_data.rpm_wheel_2, 6) + "\",";
  jsonString += "\"rpm_wheel_3\": \"" + String(logging_data.rpm_wheel_3, 6) + "\",";
  jsonString += "\"rpm_wheel_4\": \"" + String(logging_data.rpm_wheel_4, 6) + "\",";
  jsonString += "\"rpm_bm_1\": \"" + String(logging_data.rpm_bm_1, 6) + "\",";
  jsonString += "\"rpm_bm_2\": \"" + String(logging_data.rpm_bm_2, 6) + "\",";
  jsonString += "\"rpm_bm_3\": \"" + String(logging_data.rpm_bm_3, 6) + "\",";
  jsonString += "\"rpm_bm_4\": \"" + String(logging_data.rpm_bm_4, 6) + "\",";
  jsonString += "\"latitude\": \"" + String(logging_data.latitude, 6) + "\",";
  jsonString += "\"longitude\": \"" + String(logging_data.longitude, 6) + "\",";
  jsonString += "\"velocity\": \"" + String(logging_data.velocity, 6) + "\",";
  jsonString += "\"altitude\": \"" + String(logging_data.altitude, 6) + "\",";
  jsonString += "\"v_cell_1\": \"" + String(logging_data.v_cell_1, 10) + "\",";
  jsonString += "\"v_cell_2\": \"" + String(logging_data.v_cell_2, 10) + "\",";
  jsonString += "\"v_cell_3\": \"" + String(logging_data.v_cell_3, 10) + "\",";
  jsonString += "\"v_cell_4\": \"" + String(logging_data.v_cell_4, 10) + "\",";
  jsonString += "\"v_batt\": \"" + String(logging_data.v_batt, 10) + "\",";
  jsonString += "\"v_llc\": \"" + String(logging_data.v_llc, 10) + "\",";
  jsonString += "\"v_hlc\": \"" + String(logging_data.v_hlc, 10) + "\",";
  jsonString += "\"v_phrp_3v3\": \"" + String(logging_data.v_phrp_3v3, 10) + "\",";
  jsonString += "\"ch_1\": \"" + String(logging_data.ch_1) + "\",";
  jsonString += "\"ch_2\": \"" + String(logging_data.ch_2) + "\",";
  jsonString += "\"ch_3\": \"" + String(logging_data.ch_3) + "\",";
  jsonString += "\"ch_4\": \"" + String(logging_data.ch_4) + "\",";
  jsonString += "\"ch_5\": \"" + String(logging_data.ch_5) + "\",";
  jsonString += "\"ch_6\": \"" + String(logging_data.ch_6) + "\",";
  jsonString += "\"ch_7\": \"" + String(logging_data.ch_7) + "\",";
  jsonString += "\"ch_8\": \"" + String(logging_data.ch_8) + "\",";
  jsonString += "\"ch_9\": \"" + String(logging_data.ch_9) + "\",";
  jsonString += "\"ch_10\": \"" + String(logging_data.ch_10) + "\",";
  jsonString += "\"ch_11\": \"" + String(logging_data.ch_11) + "\",";
  jsonString += "\"ch_12\": \"" + String(logging_data.ch_12) + "\",";
  jsonString += "\"ch_13\": \"" + String(logging_data.ch_13) + "\",";
  jsonString += "\"ch_14\": \"" + String(logging_data.ch_14) + "\",";
  jsonString += "\"pwm_bm_1\": \"" + String(logging_data.pwm_bm_1) + "\",";
  jsonString += "\"pwm_bm_2\": \"" + String(logging_data.pwm_bm_2) + "\",";
  jsonString += "\"pwm_bm_3\": \"" + String(logging_data.pwm_bm_3) + "\",";
  jsonString += "\"pwm_bm_4\": \"" + String(logging_data.pwm_bm_4) + "\",";
  jsonString += "\"pwm_ss\": \"" + String(logging_data.pwm_ss) + "\",";
  jsonString += "\"n_satellites\": \"" + String(logging_data.c_s) + "\",";
  jsonString += "\"n_failsafe\": \"" + String(logging_data.n_failsafe) + "\",";
  jsonString += "\"dead_switch\": \"" + String(logging_data.dead_switch) + "\",";
  jsonString += "\"i_esc_1\": \"" + String(logging_data.i_esc_1) + "\",";
  jsonString += "\"i_esc_2\": \"" + String(logging_data.i_esc_2) + "\",";
  jsonString += "\"i_esc_3\": \"" + String(logging_data.i_esc_3) + "\",";
  jsonString += "\"i_esc_4\": \"" + String(logging_data.i_esc_4) + "\"";
  jsonString += "}";
  
  Serial.println(jsonString);

  ticks++;
}