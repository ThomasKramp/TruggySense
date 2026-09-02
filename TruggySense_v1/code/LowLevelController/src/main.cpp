#include <Arduino.h>

#include "tasks.h"

#define DEBUG_TIMER 500
long debug_timer = 0;

long sim_ROS2_timer = 0;

void highLevelControllerSim();

void setup()
{
  delay(2000);
  Serial.begin(9600);

  // Initialize system
  if (!initializeTasks())
  {
    Serial.println("An error occured during startup!");
    // Do something with the LEDs.
  }

  debug_timer = millis();
  sim_ROS2_timer = millis();
}

void loop()
{
  highLevelControllerSim();
  
  if (millis() - debug_timer > DEBUG_TIMER)
  {
    debug_timer = millis();

    // cli();
    // radio_t cp_radio_data = radio_data;
    // encoder_t cp_encoder = encoder_data;
    // uint8_t cp_dead_switch = dead_switch;
    // temperature_t cp_temperature_data = temperature_data;
    // vehicle_actuation_t cp_vehicle_actuation_data = vehicle_actuation_data;
    // current_t cp_current_data = current_data;
    // voltage_t cp_voltage_data = voltage_data;
    // movement_t cp_movement_data = movement_data;
    // localization_t cp_gps_data = gps_data;
    // sei();

    // Serial.println("\r\nSAFETY\r\ndead_switch: " + String(cp_dead_switch) +
    //                " FS: " + String(cp_radio_data.failsafe));

    // Serial.println("\r\nIBUS\r\n1: " + String(cp_radio_data.ch_1) +
    //                " 2: " + String(cp_radio_data.ch_2) +
    //                " 3: " + String(cp_radio_data.ch_3) +
    //                " 4: " + String(cp_radio_data.ch_4) +
    //                " 5: " + String(cp_radio_data.ch_5) +
    //                " 6: " + String(cp_radio_data.ch_6) +
    //                " 7: " + String(cp_radio_data.ch_7) +
    //                " 8: " + String(cp_radio_data.ch_8) +
    //                " 9: " + String(cp_radio_data.ch_9) +
    //                " 10: " + String(cp_radio_data.ch_10) +
    //                " FS: " + String(cp_radio_data.failsafe));

    // Serial.println("\r\nENCODER\r\nFL: " + String(cp_encoder.rpm_wheel_1) +
    //                " RL: " + String(cp_encoder.rpm_wheel_2) +
    //                " RR: " + String(cp_encoder.rpm_wheel_3) +
    //                " FR: " + String(cp_encoder.rpm_wheel_4));

    // Serial.println("\r\nTEMP\r\nhlc: " + String(cp_temperature_data.t_hlc) +
    //                " llc: " + String(cp_temperature_data.t_llc) +
    //                " esc_1: " + String(cp_temperature_data.t_esc_1) +
    //                " bm_1: " + String(cp_temperature_data.t_bm_1) +
    //                " ss: " + String(cp_temperature_data.t_ss) +
    //                " bp: " + String(cp_temperature_data.t_bp));

    // Serial.println("\r\nCURRENT\r\nhlc: " + String(cp_current_data.i_hlc, 4) +
    //                " llc: " + String(cp_current_data.i_llc, 4) +
    //                " esc_1: " + String(cp_current_data.i_esc_1, 4) +
    //                " ss: " + String(cp_current_data.i_ss, 4));

    // Serial.println("\r\nVOLTAGE\r\nhlc: " + String(cp_voltage_data.v_hlc, 4) +
    //                " llc: " + String(cp_voltage_data.v_llc, 4) +
    //                " 3V3PER: " + String(cp_voltage_data.v_phrp_3v3, 4) +
    //                " C1: " + String(cp_voltage_data.v_cell_1, 4) +
    //                " C2: " + String(cp_voltage_data.v_cell_2, 4) +
    //                " C3: " + String(cp_voltage_data.v_cell_3, 4) +
    //                " C4: " + String(cp_voltage_data.v_cell_4, 4) +
    //                " BATT: " + String(cp_voltage_data.v_batt, 4));

    // Serial.println("\r\nIMU\r\nroll: " + String(cp_movement_data.roll) +
    //                " pitch: " + String(cp_movement_data.pitch) +
    //                " yaw: " + String(cp_movement_data.yaw) +
    //                " w: " + String(cp_movement_data.w) +
    //                " i: " + String(cp_movement_data.i) +
    //                " j: " + String(cp_movement_data.j) +
    //                " k: " + String(cp_movement_data.k));

    // Serial.println("\r\nGPS\r\nLAT: " + String(cp_gps_data.latitude,8) +
    //                " LNG: " + String(cp_gps_data.longitude,8) +
    //                " ALT: " + String(cp_gps_data.altitude) +
    //                " DATE: " + String(cp_gps_data.date) +
    //                " TIME: " + String(cp_gps_data.time) +
    //                " #SAT: " + String(cp_gps_data.connected_satellites));
  }
}

void highLevelControllerSim()
{
  cli();
  radio_t cp_radio = radio_data;
  sei();
  vehicle_actuation_t cp_act;

  // Below replaces a task that needs to be handled by the high-level controller
  if (millis() - sim_ROS2_timer > 20)
  {
    sim_ROS2_timer = millis();
    
    cp_act.pwm_bm_1 = cp_radio.ch_2;
    if(cp_radio.ch_9 <= 1050)
      cp_act.pwm_bm_1 = 0.5 * cp_radio.ch_2 + 750; // from 1000-2000 to 1250-1750
    
    cp_act.pwm_ss = cp_radio.ch_4;

    cli();
    vehicle_actuation_data = cp_act;
    sei();
  }
}