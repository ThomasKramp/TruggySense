/**
 * @file tasks.cpp
 * @author Robbe Elsermans (creator)
 * @author Thomas Kramp (revisor)
 * @brief Implementation of tasks.h
 * @version 2.0
 * @date 2026-05-25
 * 
 * @copyright Copyright (c) 2025 Robbe Elsermans, All rights reserved.
 * This project is released under the CC BY-NC-SA licence.
 * See licence.txt for more information.
 */
#include <Arduino.h>                    // The arduino include must precede these two libraries.
#include <TimerInterrupt_Generic.h>     // These two libraries brake with multiple imports.
#include <ISR_Timer_Generic.h>          // That's why they are placed in the .cpp file.

#include "tasks.h"

// #define PRINTDEBUG
#define INIT_DELAY 378

TeensyTimer global_timer(TEENSY_TIMER_1);
// Each Teensy_ISR_Timer can service 16 different ISR-based timers
ISR_Timer task_isr_timer;

uint8_t initializeTasks() {
    if (tasks_initialized == 0) {
        tasks_initialized = 1;

        // Initialize I2C
        i2c = I2C();

        #pragma region Initialize Indicator Led
            indicator_led = new LED(PIN_LED);
            if (!indicator_led->init()) { return false; }
            indicator_led->write(HIGH);
            debugSerial.println(indicator_led->getErrorReport());
            delay(INIT_DELAY);
        #pragma endregion

        #pragma region Initialize Steering Servo
            // steering_servo = new Motor("DS3225 - Steering servo", STEERING_SERVO_PIN, 119, 37.8);
            steering_servo = new Motor("LW-30MG - Steering servo", PIN_SERVO, 119, 37.8);
            if (!steering_servo->init()) { return false; }
            debugSerial.println(steering_servo->getErrorReport());
            delay(INIT_DELAY);
        #pragma endregion

        #pragma region Initialize Motor
            // motor_fr = new Motor("Torox 185/Kuron 825 - FR ESC/Motor", PIN_FR_MOTOR, 93, 45);
            // if (!motor_fr->init()) { return false; }
            // debugSerial.println(motor_fr->getErrorReport());
            // delay(INIT_DELAY);

            // motor_fl = new Motor("Torox 185/Kuron 825 - FL ESC/Motor", PIN_FL_MOTOR, 93, 45);
            // if (!motor_fl->init()) { return false; }
            // debugSerial.println(motor_fl->getErrorReport());
            // delay(INIT_DELAY);

            motor_rr = new Motor("Torox 185/Kuron 825 - RR ESC/Motor", PIN_RR_MOTOR, 93, 45);
            if (!motor_rr->init()) { return false; }
            debugSerial.println(motor_rr->getErrorReport());
            delay(INIT_DELAY);

            // motor_rl = new Motor("Torox 185/Kuron 825 - RL ESC/Motor", PIN_RL_MOTOR, 93, 45);
            // if (!motor_rl->init()) { return false; }
            // debugSerial.println(motor_rl->getErrorReport());
            // delay(INIT_DELAY);
        #pragma endregion

        #pragma region Initialize Cooling Fan
            cooling_fan = new Fan(PIN_FAN);
            if (!cooling_fan->init()) { return false; }
            debugSerial.println(cooling_fan->getErrorReport());
            delay(INIT_DELAY);
        #pragma endregion

        #pragma region Initialize Remote Control
            remote_control_receiver = new FS_X6B(ibusSerial);
            if (!remote_control_receiver->init()) { return false; }
            debugSerial.println(remote_control_receiver->getErrorReport());
            delay(INIT_DELAY);
        #pragma endregion

        #pragma region Initialize GPS
            gnss_gps = new U_BLOX(gpsSerial);
            if (!gnss_gps->init()) { return false; }
            debugSerial.println(gnss_gps->getErrorReport());
            delay(INIT_DELAY);
        #pragma endregion
      
        #pragma region Initialize Encoders
            encoder_fr = new Encoder("FR", PIN_FR_ENCODER, POSITION::FR);
            if (!encoder_fr->init()) { return false; }
            debugSerial.println(encoder_fr->getErrorReport());
            delay(INIT_DELAY);

            encoder_fl = new Encoder("FL", PIN_FL_ENCODER, POSITION::FL);
            if (!encoder_fl->init()) { return false; }
            debugSerial.println(encoder_fl->getErrorReport());
            delay(INIT_DELAY);

            encoder_rr = new Encoder("RR", PIN_RR_ENCODER, POSITION::RR);
            if (!encoder_rr->init()) { return false; }
            debugSerial.println(encoder_rr->getErrorReport());
            delay(INIT_DELAY);

            encoder_rl = new Encoder("RL", PIN_RL_ENCODER, POSITION::RL);
            if (!encoder_rl->init()) { return false; }
            debugSerial.println(encoder_rl->getErrorReport());
            delay(INIT_DELAY);
        #pragma endregion

        #pragma region Initialize Voltage Sensing
            voltage_sensor_bc_1 = new Supply("Cell 1",    PIN_BATTERY_CELL_1,  4.2);
            if (!voltage_sensor_bc_1->init()) { return false; }
            debugSerial.println(voltage_sensor_bc_1->getErrorReport());
            delay(INIT_DELAY);

            voltage_sensor_bc_2 = new Supply("Cell 2",    PIN_BATTERY_CELL_2,  8.4);
            if (!voltage_sensor_bc_2->init()) { return false; }
            debugSerial.println(voltage_sensor_bc_2->getErrorReport());
            delay(INIT_DELAY);

            voltage_sensor_bc_3 = new Supply("Cell 3",    PIN_BATTERY_CELL_3, 12.6);
            if (!voltage_sensor_bc_3->init()) { return false; }
            debugSerial.println(voltage_sensor_bc_3->getErrorReport());
            delay(INIT_DELAY);

            voltage_sensor_bc_4 = new Supply("Cell 4",    PIN_BATTERY_CELL_4, 16.8);
            if (!voltage_sensor_bc_4->init()) { return false; }
            debugSerial.println(voltage_sensor_bc_4->getErrorReport());
            delay(INIT_DELAY);

            voltage_sensor_3V3  = new Supply("3.3V Rail", PIN_3V3_RAIL      ,  3.3);
            if (!voltage_sensor_3V3->init()) { return false; }
            debugSerial.println(voltage_sensor_3V3->getErrorReport());
            delay(INIT_DELAY);

            voltage_sensor_5V0  = new Supply("5.0V Rail", PIN_5V0_RAIL      ,  5.1);
            if (!voltage_sensor_5V0->init()) { return false; }
            debugSerial.println(voltage_sensor_5V0->getErrorReport());
            delay(INIT_DELAY);

            const float rail_5V = voltage_sensor_5V0->getVoltage();
        #pragma endregion

        #pragma region Initialize Current Sensing
            high_current_sensor_fr = new ACS758("FR", PIN_FR_HIGH_CURRENT, rail_5V);
            if (!high_current_sensor_fr->init()) { return false; }
            debugSerial.println(high_current_sensor_fr->getErrorReport());
            delay(INIT_DELAY);

            // high_current_sensor_fl = new ACS758("FL", PIN_FL_HIGH_CURRENT, rail_5V);
            // if (!high_current_sensor_fl->init()) { return false; }
            // debugSerial.println(high_current_sensor_fl->getErrorReport());
            // delay(INIT_DELAY);

            // high_current_sensor_rr = new ACS758("RR", PIN_RR_HIGH_CURRENT, rail_5V);
            // if (!high_current_sensor_rr->init()) { return false; }
            // debugSerial.println(high_current_sensor_rr->getErrorReport());
            // delay(INIT_DELAY);

            // high_current_sensor_rl = new ACS758("RL", PIN_RL_HIGH_CURRENT, rail_5V);
            // if (!high_current_sensor_rl->init()) { return false; }
            // debugSerial.println(high_current_sensor_rl->getErrorReport());
            // delay(INIT_DELAY);

            low_current_sensor_hlc = new INA219("HLC", i2c, INA219_ADDRESS_2,  50, 4.0f, 5);
            if (!low_current_sensor_hlc->init()) { return false; }
            debugSerial.println(low_current_sensor_hlc->getErrorReport());
            delay(INIT_DELAY);

            low_current_sensor_llc = new INA219("LLC", i2c, INA219_ADDRESS_4, 100, 0.5f, 5);
            if (!low_current_sensor_llc->init()) { return false; }
            debugSerial.println(low_current_sensor_llc->getErrorReport());
            delay(INIT_DELAY);

            low_current_sensor_etc = new INA219("ETC", i2c, INA219_ADDRESS_1, 100, 2.0f, 5);
            if (!low_current_sensor_etc->init()) { return false; }
            debugSerial.println(low_current_sensor_etc->getErrorReport());
            delay(INIT_DELAY);
        #pragma endregion

        #pragma region Initialize Temperature Sensing
            // temperature_sensor_llc   = new LM75("LLC",  i2c, LM75_ADDRESS_8, FAULTS::FAULTS_4);
            // if (!temperature_sensor_llc->init()) { return false; }
            // debugSerial.println(temperature_sensor_llc->getErrorReport());
            // delay(INIT_DELAY);

            temperature_sensor_hlc   = new LM75("HLC",  i2c, LM75_ADDRESS_5,  FAULTS::FAULTS_4);
            if (!temperature_sensor_hlc->init()) { return false; }
            debugSerial.println(temperature_sensor_hlc->getErrorReport());
            delay(INIT_DELAY);

            temperature_sensor_bp    = new LM75("BP",   i2c, LM75_ADDRESS_4,  FAULTS::FAULTS_4);
            if (!temperature_sensor_bp->init()) { return false; }
            debugSerial.println(temperature_sensor_bp->getErrorReport());
            delay(INIT_DELAY);

            temperature_sensor_ss    = new LM75("SS",   i2c, LM75_ADDRESS_2,  FAULTS::FAULTS_4);
            if (!temperature_sensor_ss->init()) { return false; }
            debugSerial.println(temperature_sensor_ss->getErrorReport());
            delay(INIT_DELAY);

            temperature_sensor_bm_1  = new LM75("BM1",  i2c, LM75_ADDRESS_7,  FAULTS::FAULTS_4);
            if (!temperature_sensor_bm_1->init()) { return false; }
            debugSerial.println(temperature_sensor_bm_1->getErrorReport());
            delay(INIT_DELAY);

            // temperature_sensor_bm_2  = new LM75("BM2",  i2c, LM75_ADDRESS_8, FAULTS::FAULTS_4);
            // if (!temperature_sensor_bm_2->init()) { return false; }
            // debugSerial.println(temperature_sensor_bm_2->getErrorReport());
            // delay(INIT_DELAY);

            // temperature_sensor_bm_3  = new LM75("BM3",  i2c, LM75_ADDRESS_8, FAULTS::FAULTS_4);
            // if (!temperature_sensor_bm_3->init()) { return false; }
            // debugSerial.println(temperature_sensor_bm_3->getErrorReport());
            // delay(INIT_DELAY);

            // temperature_sensor_bm_4  = new LM75("BM4",  i2c, LM75_ADDRESS_8, FAULTS::FAULTS_4);
            // if (!temperature_sensor_bm_4->init()) { return false; }
            // debugSerial.println(temperature_sensor_bm_4->getErrorReport());
            // delay(INIT_DELAY);

            temperature_sensor_esc_1 = new LM75("ESC1", i2c, LM75_ADDRESS_1,  FAULTS::FAULTS_4);
            if (!temperature_sensor_esc_1->init()) { return false; }
            debugSerial.println(temperature_sensor_esc_1->getErrorReport());
            delay(INIT_DELAY);

            // temperature_sensor_esc_2 = new LM75("ESC2", i2c, LM75_ADDRESS_8, FAULTS::FAULTS_4);
            // if (!temperature_sensor_esc_2->init()) { return false; }
            // debugSerial.println(temperature_sensor_esc_2->getErrorReport());
            // delay(INIT_DELAY);

            // temperature_sensor_esc_3 = new LM75("ESC3", i2c, LM75_ADDRESS_8, FAULTS::FAULTS_4);
            // if (!temperature_sensor_esc_3->init()) { return false; }
            // debugSerial.println(temperature_sensor_esc_3->getErrorReport());
            // delay(INIT_DELAY);

            // temperature_sensor_esc_4 = new LM75("ESC4", i2c, LM75_ADDRESS_8, FAULTS::FAULTS_4);
            // if (!temperature_sensor_esc_4->init()) { return false; }
            // debugSerial.println(temperature_sensor_esc_4->getErrorReport());
            // delay(INIT_DELAY);
        #pragma endregion

        #pragma region Initialize Motion Sensing
            // imu_sensor = new PIM448(PIM448_ADDRESS_1, ACCEL_RANGE::RANGE_2G, GYRO_RANGE::RANGE_250, 
            //                         DLPF::DLPF_6, DLPF::DLPF_6, 10, 10, 
            //                         MAG_REFRESH_RATE::RATE_10HZ);
            imu_sensor = new BNO085(BNO085_ADDRESS_1);
            if (!imu_sensor->init()) { return false; }
            debugSerial.println(imu_sensor->getErrorReport());
            delay(INIT_DELAY);
        #pragma endregion

        #pragma region Initialize SD-Card Logging
            sd_logger = new SDCard(FILE_TYPE::JSON);
            if (!sd_logger->init()) { return false; }
            debugSerial.println(sd_logger->getErrorReport());
            delay(INIT_DELAY);
        #pragma endregion

        #pragma region Initialize Long Range Communication/Wireless Logging
            wireless_logger = new nRF24(PIN_LONG_RANGE_CE, PIN_LONG_RANGE_CSN);
            if (!wireless_logger->init()) { return false; }
            debugSerial.println(wireless_logger->getErrorReport());
        #pragma endregion

        #pragma region Initialize Schedular
            // Define tasks clock
            //  Interval in microsecs
            if (global_timer.attachInterruptInterval(SYS_TIMER_INTERVAL_US, sysTick)) {
                Serial.println("Starting ITimer OK, millis() = " + String(millis()));
            } else {
                Serial.println("Can't set ITimer correctly. Select another freq. or interval");
            }

            task_isr_timer.setInterval(TIMER_INTERVAL_LED,          taskWriteLed);
            task_isr_timer.setInterval(TIMER_INTERVAL_SERVO,        taskWriteServo);
            task_isr_timer.setInterval(TIMER_INTERVAL_MOTOR,        taskWriteMotor);
            task_isr_timer.setInterval(TIMER_INTERVAL_FAN,          taskWriteFan);
            task_isr_timer.setInterval(TIMER_INTERVAL_IBUS,         taskReceiveRemoteControl);
            task_isr_timer.setInterval(TIMER_INTERVAL_GPS,          taskReadGPS);
            task_isr_timer.setInterval(TIMER_INTERVAL_ENCODER,      taskReadEncoders);
            task_isr_timer.setInterval(TIMER_INTERVAL_VOLT,         taskReadVoltages);
            task_isr_timer.setInterval(TIMER_INTERVAL_CURRENT,      taskReadCurrents);
            task_isr_timer.setInterval(TIMER_INTERVAL_TEMPERATURE,  taskReadTemperature);
            task_isr_timer.setInterval(TIMER_INTERVAL_MOTION,       taskReadIMU);
            task_isr_timer.setInterval(TIMER_INTERVAL_LOG,          taskLogData);
            task_isr_timer.setInterval(TIMER_INTERVAL_SD_LOG,       taskWriteSDLogs);
            // task_isr_timer.setInterval(TIMER_INTERVAL_WIRELESS_LOG, taskSendLongRangeCommunication);
        #pragma endregion

        return true;
    }

    return false;
}

void taskWriteLed() {
    #ifdef PRINTDEBUG
        debugSerial.println("Indicator LED");
    #endif
    
    indicator_led->toggle();
}

void taskWriteServo() {
    #ifdef PRINTDEBUG
        debugSerial.println("Steering Servo");
    #endif
    
    interrupts_disable();
        uint16_t cp_servo = radio_data.ch_1;
        uint16_t cp_dead_switch = radio_data.ch_5;
    interrupts_enable();

    steering_servo->write(cp_servo, (bool)true);
    steering_servo->disable(cp_dead_switch);
}

void taskWriteMotor() {
    #ifdef PRINTDEBUG
        debugSerial.println("Motor Controls");
    #endif
    
    interrupts_disable();
        uint16_t cp_motor       = radio_data.ch_3;
        uint16_t cp_dead_switch = radio_data.ch_5;
        float cp_v_cell_1       = voltage_data.v_cell_1;
        float cp_v_cell_2       = voltage_data.v_cell_2;
        float cp_v_cell_3       = voltage_data.v_cell_3;
        float cp_v_cell_4       = voltage_data.v_cell_4;
    interrupts_enable();

    // motor_fr->write(cp_motor, (uint16_t)2000);
    // motor_fr->disable(cp_dead_switch);
    // motor_fr->disable(cp_v_cell_1, cp_v_cell_2, cp_v_cell_3, cp_v_cell_4);
    
    // motor_fl->write(cp_motor, (uint16_t)2000);
    // motor_fl->disable(cp_dead_switch);
    // motor_fl->disable(cp_v_cell_1, cp_v_cell_2, cp_v_cell_3, cp_v_cell_4);
    
    motor_rr->write(cp_motor, (uint16_t)2000);
    motor_rr->disable(cp_dead_switch);
    // motor_rr->disable(cp_v_cell_1, cp_v_cell_2, cp_v_cell_3, cp_v_cell_4);
    
    // motor_rl->write(cp_motor, (uint16_t)2000);
    // motor_rl->disable(cp_dead_switch);
    // motor_rl->disable(cp_v_cell_1, cp_v_cell_2, cp_v_cell_3, cp_v_cell_4);
}

void taskWriteFan() {
    #ifdef PRINTDEBUG
        debugSerial.println("Cooling Fan");
    #endif

    interrupts_disable();
        float cp_hlc_temperature = temperature_data.t_hlc;
    interrupts_enable();

    cooling_fan->write(cp_hlc_temperature);
}

void taskReceiveRemoteControl() {
    #ifdef PRINTDEBUG
        debugSerial.println("Remote Control");
    #endif

    remote_control_receiver->read();
    radio_t cp_radio_data = {0};

    cp_radio_data.ch_1  = remote_control_receiver->getChannel(CHANNEL_SOURCE::CH_1);
    cp_radio_data.ch_2  = remote_control_receiver->getChannel(CHANNEL_SOURCE::CH_2);
    cp_radio_data.ch_3  = remote_control_receiver->getChannel(CHANNEL_SOURCE::CH_3);
    cp_radio_data.ch_4  = remote_control_receiver->getChannel(CHANNEL_SOURCE::CH_4);
    cp_radio_data.ch_5  = remote_control_receiver->getChannel(CHANNEL_SOURCE::CH_5);
    cp_radio_data.ch_6  = remote_control_receiver->getChannel(CHANNEL_SOURCE::CH_6);
    cp_radio_data.ch_7  = remote_control_receiver->getChannel(CHANNEL_SOURCE::CH_7);
    cp_radio_data.ch_8  = remote_control_receiver->getChannel(CHANNEL_SOURCE::CH_8);
    cp_radio_data.ch_9  = remote_control_receiver->getChannel(CHANNEL_SOURCE::CH_9);
    cp_radio_data.ch_10 = remote_control_receiver->getChannel(CHANNEL_SOURCE::CH_10);
    cp_radio_data.failsafe = remote_control_receiver->isFailsafe();

    interrupts_disable();
        radio_data = cp_radio_data;
    interrupts_enable();
}

void taskReadGPS() {
    #ifdef PRINTDEBUG
        debugSerial.println("GPS");
    #endif
    
    gnss_gps->read();
    localization_t cp_gps_data = {0};

    cp_gps_data.date = gnss_gps->getDate();
    cp_gps_data.time = gnss_gps->getTime();
    cp_gps_data.latitude = gnss_gps->getLatitude();
    cp_gps_data.longitude = gnss_gps->getLongitude();
    cp_gps_data.velocity = gnss_gps->getVelocity();
    cp_gps_data.altitude = gnss_gps->getAltitude();
    cp_gps_data.connected_satellites = gnss_gps->getSatellites();

    interrupts_disable();
        gps_data = cp_gps_data;
    interrupts_enable();
}

void taskReadEncoders() {
    #ifdef PRINTDEBUG
        debugSerial.println("Wheel Encoders");
    #endif
    
    encoder_t cp_encoder_data = {0};

    cp_encoder_data.rpm_wheel_1 = encoder_fr->getWheelSpeed();
    cp_encoder_data.rpm_wheel_2 = encoder_fl->getWheelSpeed();
    cp_encoder_data.rpm_wheel_3 = encoder_rr->getWheelSpeed();
    cp_encoder_data.rpm_wheel_4 = encoder_rl->getWheelSpeed();

    interrupts_disable();
        encoder_data = cp_encoder_data;
    interrupts_enable();
}

void taskReadVoltages() {
    #ifdef PRINTDEBUG
        debugSerial.println("Voltage Rails");
    #endif

    voltage_t cp_voltage_data = {0};
    
    cp_voltage_data.v_cell_1   = voltage_sensor_bc_1->getVoltage();
    cp_voltage_data.v_cell_2   = voltage_sensor_bc_2->getVoltage();
    cp_voltage_data.v_cell_3   = voltage_sensor_bc_3->getVoltage();
    cp_voltage_data.v_cell_4   = voltage_sensor_bc_4->getVoltage();
    cp_voltage_data.v_rail_3v3 = voltage_sensor_3V3->getVoltage();
    cp_voltage_data.v_rail_5v0 = voltage_sensor_5V0->getVoltage();

    interrupts_disable();
        voltage_data = cp_voltage_data;
    interrupts_enable();
}

void taskReadCurrents() {
    #ifdef PRINTDEBUG
        debugSerial.println("High Currents");
    #endif

    current_t cp_current_data = {0};
    
    cp_current_data.i_esc_1 = high_current_sensor_fr->getCurrent();
    // cp_current_data.i_esc_2 = high_current_sensor_fl->getCurrent();
    // cp_current_data.i_esc_3 = high_current_sensor_rr->getCurrent();
    // cp_current_data.i_esc_4 = high_current_sensor_rl->getCurrent();

    #ifdef PRINTDEBUG
        debugSerial.println("Low Currents");
    #endif
    
    cp_current_data.i_hlc = low_current_sensor_hlc->getCurrent();
    cp_current_data.i_llc = low_current_sensor_llc->getCurrent();
    cp_current_data.i_etc = low_current_sensor_etc->getCurrent();

    interrupts_disable();
        current_data = cp_current_data;
    interrupts_enable();
}

void taskReadTemperature() {
    #ifdef PRINTDEBUG
        debugSerial.println("Temperatures");
    #endif
    
    temperature_t cp_temperature_data = {0};

    // cp_temperature_data.t_llc   = temperature_sensor_llc->getTemperature();
    cp_temperature_data.t_hlc   = temperature_sensor_hlc->getTemperature();
    cp_temperature_data.t_bp    = temperature_sensor_bp->getTemperature();
    cp_temperature_data.t_ss    = temperature_sensor_ss->getTemperature();
    cp_temperature_data.t_bm_1  = temperature_sensor_bm_1->getTemperature();
    // cp_temperature_data.t_bm_2  = temperature_sensor_bm_2->getTemperature();
    // cp_temperature_data.t_bm_3  = temperature_sensor_bm_3->getTemperature();
    // cp_temperature_data.t_bm_4  = temperature_sensor_bm_4->getTemperature();
    cp_temperature_data.t_esc_1 = temperature_sensor_esc_1->getTemperature();
    // cp_temperature_data.t_esc_2 = temperature_sensor_esc_2->getTemperature();
    // cp_temperature_data.t_esc_3 = temperature_sensor_esc_3->getTemperature();
    // cp_temperature_data.t_esc_4 = temperature_sensor_esc_4->getTemperature();

    interrupts_disable();
        temperature_data = cp_temperature_data;
    interrupts_enable();
}

void taskReadIMU() {
    #ifdef PRINTDEBUG
        debugSerial.println("IMU");
    #endif
    
    imu_sensor->read();
    movement_t cp_movement_data = {0};

    cp_movement_data.roll  = imu_sensor->getRoll();
    cp_movement_data.pitch = imu_sensor->getPitch();
    cp_movement_data.yaw   = imu_sensor->getYaw();
    cp_movement_data.w     = imu_sensor->getQuaternionR();
    cp_movement_data.i     = imu_sensor->getQuaternionI();
    cp_movement_data.j     = imu_sensor->getQuaternionJ();
    cp_movement_data.k     = imu_sensor->getQuaternionK();

    interrupts_disable();
        movement_data = cp_movement_data;
    interrupts_enable();
}

void taskLogData() {
    #ifdef PRINTDEBUG
        debugSerial.println("Log data");
    #endif
    
    interrupts_disable();
        localization_t  cp_gps_data         = gps_data;
        radio_t         cp_radio_data       = radio_data;
        encoder_t       cp_encoder_data     = encoder_data;
        voltage_t       cp_voltage_data     = voltage_data;
        current_t       cp_current_data     = current_data;
        temperature_t   cp_temperature_data = temperature_data;
        movement_t      cp_movement_data    = movement_data;
    interrupts_enable();

    logging_data_t cp_log_data = {0};
    cp_log_data.timestamp = millis();

    cp_log_data.date           = cp_gps_data.date;
    cp_log_data.time           = cp_gps_data.time;
    cp_log_data.latitude       = cp_gps_data.latitude;
    cp_log_data.longitude      = cp_gps_data.longitude;
    cp_log_data.velocity       = cp_gps_data.velocity;
    cp_log_data.altitude       = cp_gps_data.altitude;
    cp_log_data.c_s            = cp_gps_data.connected_satellites;

    cp_log_data.ch_1           = cp_radio_data.ch_1;
    cp_log_data.ch_2           = cp_radio_data.ch_2;
    cp_log_data.ch_3           = cp_radio_data.ch_3;
    cp_log_data.ch_4           = cp_radio_data.ch_4;
    cp_log_data.ch_5           = cp_radio_data.ch_5;
    cp_log_data.ch_6           = cp_radio_data.ch_6;
    cp_log_data.ch_7           = cp_radio_data.ch_7;
    cp_log_data.ch_8           = cp_radio_data.ch_8;
    cp_log_data.ch_9           = cp_radio_data.ch_9;
    cp_log_data.ch_10          = cp_radio_data.ch_10;
    cp_log_data.failsafe       = cp_radio_data.failsafe;

    cp_log_data.rpm_wheel_1    = cp_encoder_data.rpm_wheel_1;
    cp_log_data.rpm_wheel_2    = cp_encoder_data.rpm_wheel_2;
    cp_log_data.rpm_wheel_3    = cp_encoder_data.rpm_wheel_3;
    cp_log_data.rpm_wheel_4    = cp_encoder_data.rpm_wheel_4;

    cp_log_data.v_cell_1       = cp_voltage_data.v_cell_1;
    cp_log_data.v_cell_2       = cp_voltage_data.v_cell_2;
    cp_log_data.v_cell_3       = cp_voltage_data.v_cell_3;
    cp_log_data.v_cell_4       = cp_voltage_data.v_cell_4;
    cp_log_data.v_rail_3V3     = cp_voltage_data.v_rail_3v3;
    cp_log_data.v_rail_5v0     = cp_voltage_data.v_rail_5v0;

    cp_log_data.i_esc_1        = cp_current_data.i_esc_1;
    cp_log_data.i_esc_2        = cp_current_data.i_esc_2;
    cp_log_data.i_esc_3        = cp_current_data.i_esc_3;
    cp_log_data.i_esc_4        = cp_current_data.i_esc_4;
    cp_log_data.i_etc          = cp_current_data.i_etc;
    cp_log_data.i_llc          = cp_current_data.i_llc;
    cp_log_data.i_hlc          = cp_current_data.i_hlc;

    cp_log_data.t_llc          = cp_temperature_data.t_llc;
    cp_log_data.t_hlc          = cp_temperature_data.t_hlc;
    cp_log_data.t_bp           = cp_temperature_data.t_bp;
    cp_log_data.t_ss           = cp_temperature_data.t_ss;
    cp_log_data.t_bm_1         = cp_temperature_data.t_bm_1;
    cp_log_data.t_bm_2         = cp_temperature_data.t_bm_2;
    cp_log_data.t_bm_3         = cp_temperature_data.t_bm_3;
    cp_log_data.t_bm_4         = cp_temperature_data.t_bm_4;
    cp_log_data.t_esc_1        = cp_temperature_data.t_esc_1;
    cp_log_data.t_esc_2        = cp_temperature_data.t_esc_2;
    cp_log_data.t_esc_3        = cp_temperature_data.t_esc_3;
    cp_log_data.t_esc_4        = cp_temperature_data.t_esc_4;

    cp_log_data.roll           = cp_movement_data.roll;
    cp_log_data.pitch          = cp_movement_data.pitch;
    cp_log_data.yaw            = cp_movement_data.yaw;
    cp_log_data.w              = cp_movement_data.w;
    cp_log_data.i              = cp_movement_data.i;
    cp_log_data.j              = cp_movement_data.j;
    cp_log_data.k              = cp_movement_data.k;
    
    interrupts_disable();
        log_data = cp_log_data;
    interrupts_enable();
}

void taskWriteSDLogs() {
    #ifdef PRINTDEBUG
        debugSerial.println("SD-Card Logger");
    #endif

    interrupts_disable();
        logging_data_t cp_log_data    = log_data;
        uint16_t       cp_dead_switch = radio_data.ch_5;
    interrupts_enable();

    // debugSerial.println("\tRPM-FR: " + String(cp_log_data.rpm_wheel_1) + 
    //                     "\tRPM-FL: " + String(cp_log_data.rpm_wheel_2) + 
    //                     "\tRPM-RR: " + String(cp_log_data.rpm_wheel_3) + 
    //                     "\tRPM-RL: " + String(cp_log_data.rpm_wheel_4)
    //                 );
    
    if (cp_dead_switch > 1500)
        sd_logger->sendData(cp_log_data);
    else
        sd_logger->closeFile();
}

void taskSendLongRangeCommunication() {
    #ifdef PRINTDEBUG
        debugSerial.println("Long Range Communication");
    #endif

    interrupts_disable();
        logging_data_t cp_log_data    = log_data;
        uint16_t       cp_dead_switch = radio_data.ch_5;
    interrupts_enable();
    
    if (cp_dead_switch > 1500)
        wireless_logger->sendData(cp_log_data);
}

// void taskReadROS() {
//     #ifdef PRINTDEBUG
//         debugSerial.println("ROS Receive");
//     #endif
    
//     interrupts_disable();
//     //cooling_fan->write(radio_data.ch_4);
//     interrupts_enable();
// }

// void taskWriteROS() {
//     #ifdef PRINTDEBUG
//         debugSerial.println("ROS Transmit");
//     #endif
    
//     interrupts_disable();
//     //cooling_fan->write(radio_data.ch_4);
//     interrupts_enable();
// }

void sysTick() { task_isr_timer.run(); }