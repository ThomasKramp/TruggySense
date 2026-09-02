#include "tasks.h"

#include <Arduino.h>

#include <TimerInterrupt_Generic.h>
#include <ISR_Timer_Generic.h>

TeensyTimer global_timer(TEENSY_TIMER_1);
// Init Teensy_ISR_Timer
// Each Teensy_ISR_Timer can service 16 different ISR-based timers
ISR_Timer task_isr_timer;

uint8_t tasks_initialized = 0;

IBUS x6b_receiver;
radio_t radio_data;

Encoder wheel_encoder;
encoder_t encoder_data;

Temperature temperature;
temperature_t temperature_data;

Actuate vehicle_actuate;
vehicle_actuation_t vehicle_actuation_data;

VoltageMeasure voltageMeasure;
CurrentMeasure currentMeasure;
voltage_t voltage_data;
current_t current_data;

IMU imu;
movement_t movement_data;

SDLogger sd_logger;
logging_data_t log_data;
volatile uint8_t newWriteFile = true;

localization_t gps_data;
GPSDriver gps;

BaseStation baseStation;

volatile uint8_t dead_switch;
volatile uint8_t error;

volatile uint8_t sd_error;
volatile uint8_t bs_error;
volatile uint8_t imu_error;
volatile uint8_t gps_error;

uint8_t initializeTasks()
{
    if (tasks_initialized == 0)
    {
        tasks_initialized = 1;

        // Initialize IBUS
        x6b_receiver.init();
        Serial.println("IBUS OK");

        // Initialize Wheel Encoder
        wheel_encoder = Encoder();
        encoder_data = {0};

        if (!vehicle_actuate.init())
        {
            Serial.println("SERVO error");
            error = 1;
        }

        if (!temperature.init())
        {
            Serial.println("Temperature error");
            error = 1;
        }

        if (!imu.init())
        {
            Serial.println("IMU error");
            imu_error = 1;
        }

        if (!sd_logger.init())
        {
            Serial.println("SD error");

            sd_error = 1;
        }

        if (!gps.init())
        {
            Serial.println("GPS error");
            gps_error = 1;
        }

        if (!baseStation.init())
        {
            Serial.println("base station module error");
            bs_error = 1;
        }

        // Define tasks clock
        //  Interval in microsecs
        if (global_timer.attachInterruptInterval(SYS_TIMER_INTERVAL_US, sysTick))
        {
            Serial.println("Starting  ITimer OK, millis() = " + String(millis()));
        }
        else
        {
            Serial.println("Can't set ITimer correctly. Select another freq. or interval");
            error = 1;
        }

        // Setup timers for tasks
        // First 3 are mandatory to drive with the vehicle
        task_isr_timer.setInterval(TIMER_INTERVAL_IBUS, taskReadIbus);
        task_isr_timer.setInterval(TIMER_INTERVAL_SAFETY, taskSafety);
        task_isr_timer.setInterval(TIMER_INTERVAL_ACTUATE, taskActuate);

        // These are "aditional" to improve driveability with a given algorithm
        task_isr_timer.setInterval(TIMER_INTERVAL_ENCODER, taskReadEncoder);
        task_isr_timer.setInterval(TIMER_INTERVAL_TEMPERATURE, taskReadTemperature); // Produces DEAD signal arround +- 8s now and then
        task_isr_timer.setInterval(TIMER_INTERVAL_ADC, taskReadVAndI);
        if (!imu_error)
            task_isr_timer.setInterval(TIMER_INTERVAL_IMU, taskReadIMU);
        if (!gps_error)
            task_isr_timer.setInterval(TIMER_INTERVAL_GPS, taskReadGps);
        task_isr_timer.setInterval(TIMER_INTERVAL_LOG, taskLog);
        if (!sd_error)
            task_isr_timer.setInterval(TIMER_INTERVAL_SAVE_LOG, taskSaveLog);
        if (!bs_error)
            task_isr_timer.setInterval(TIMER_INTERVAL_BASE_STATION, taskWriteBaseStation);
    }
    // Serial.println(error || sd_error);

    return !error || !sd_error || !bs_error || !imu_error || !gps_error;
}

void taskReadGps()
{
    if (!gps_error)
    {
        gps.readGps();
        localization_t cp_loc;

        cp_loc.date = gps.getDate();
        cp_loc.time = gps.getTime();
        cp_loc.altitude = gps.getAltitude();
        cp_loc.latitude = gps.getLatitude();
        cp_loc.longitude = gps.getLongitude();
        cp_loc.velocity = gps.getVelocity();
        cp_loc.connected_satellites = gps.getConnectedSatelites();

        cli();
        gps_data = cp_loc;
        sei();
    }
}

void taskReadIbus()
{
    cli();
    x6b_receiver.read();

    radio_t cp_radio;

    cp_radio.ch_1 = x6b_receiver.getChannel(x6b_receiver.ch_1);
    cp_radio.ch_2 = x6b_receiver.getChannel(x6b_receiver.ch_2);
    cp_radio.ch_3 = x6b_receiver.getChannel(x6b_receiver.ch_3);
    cp_radio.ch_4 = x6b_receiver.getChannel(x6b_receiver.ch_4);
    cp_radio.ch_5 = x6b_receiver.getChannel(x6b_receiver.ch_5);
    cp_radio.ch_6 = x6b_receiver.getChannel(x6b_receiver.ch_6);
    cp_radio.ch_7 = x6b_receiver.getChannel(x6b_receiver.ch_7);
    cp_radio.ch_8 = x6b_receiver.getChannel(x6b_receiver.ch_8);
    cp_radio.ch_9 = x6b_receiver.getChannel(x6b_receiver.ch_9);
    cp_radio.ch_10 = x6b_receiver.getChannel(x6b_receiver.ch_10);
    //cp_radio.ch_10 = 2000;
    cp_radio.ch_11 = x6b_receiver.getChannel(x6b_receiver.ch_11);
    cp_radio.ch_12 = x6b_receiver.getChannel(x6b_receiver.ch_12);
    cp_radio.ch_13 = x6b_receiver.getChannel(x6b_receiver.ch_13);
    cp_radio.ch_14 = x6b_receiver.getChannel(x6b_receiver.ch_14);
    cp_radio.failsafe = x6b_receiver.isFailsafe();
    
    radio_data = cp_radio;
    sei();
}

void taskReadEncoder()
{
    // cli();
    encoder_t cp_enc;

    cp_enc.rpm_wheel_1 = wheel_encoder.getWheelSpeed(wheel_encoder.FL);
    cp_enc.rpm_wheel_2 = wheel_encoder.getWheelSpeed(wheel_encoder.RL);
    cp_enc.rpm_wheel_3 = wheel_encoder.getWheelSpeed(wheel_encoder.RR);
    cp_enc.rpm_wheel_4 = wheel_encoder.getWheelSpeed(wheel_encoder.FR);

    cli();
    encoder_data = cp_enc;
    sei();
    // TODO: Motor encoder
}

void taskReadTemperature()
{
    temperature.read();

    temperature_t cp_temperature_data;

    cp_temperature_data.t_bm_1 = temperature.getBySource(temperature.bm_1);
    cp_temperature_data.t_bm_2 = temperature.getBySource(temperature.bm_2);
    cp_temperature_data.t_bm_3 = temperature.getBySource(temperature.bm_3);
    cp_temperature_data.t_bm_4 = temperature.getBySource(temperature.bm_4);
    cp_temperature_data.t_esc_1 = temperature.getBySource(temperature.esc_1);
    cp_temperature_data.t_esc_2 = temperature.getBySource(temperature.esc_2);
    cp_temperature_data.t_esc_3 = temperature.getBySource(temperature.esc_3);
    cp_temperature_data.t_esc_4 = temperature.getBySource(temperature.esc_4);
    cp_temperature_data.t_hlc = temperature.getBySource(temperature.hlc);
    cp_temperature_data.t_llc = temperature.getBySource(temperature.llc);
    cp_temperature_data.t_ss = temperature.getBySource(temperature.ss);
    cp_temperature_data.t_bp = temperature.getBySource(temperature.bp);

    cli();
    temperature_data = cp_temperature_data;
    sei();
}

void taskReadVAndI()
{
    currentMeasure.ReadCurrent();

    voltageMeasure.ReadVoltage();

    voltage_t cp_voltage;
    current_t cp_current;

    cp_current.i_hlc = currentMeasure.getCurrentBySource(currentMeasure.hlc);
    cp_current.i_llc = currentMeasure.getCurrentBySource(currentMeasure.llc);
    cp_current.i_ss = currentMeasure.getCurrentBySource(currentMeasure.ss);
    cp_current.i_esc_1 = currentMeasure.getCurrentBySource(currentMeasure.esc_1);
    cp_current.i_esc_2 = currentMeasure.getCurrentBySource(currentMeasure.esc_2);
    cp_current.i_esc_3 = currentMeasure.getCurrentBySource(currentMeasure.esc_3);
    cp_current.i_esc_4 = currentMeasure.getCurrentBySource(currentMeasure.esc_4);

    cp_voltage.v_hlc = voltageMeasure.getVoltageBySource(voltageMeasure.hlc);
    cp_voltage.v_llc = voltageMeasure.getVoltageBySource(voltageMeasure.llc);
    cp_voltage.v_phrp_3v3 = voltageMeasure.getVoltageBySource(voltageMeasure.per);
    cp_voltage.v_cell_1 = voltageMeasure.getVoltageBySource(voltageMeasure.b_1);
    cp_voltage.v_cell_2 = voltageMeasure.getVoltageBySource(voltageMeasure.b_2);
    cp_voltage.v_cell_3 = voltageMeasure.getVoltageBySource(voltageMeasure.b_3);
    cp_voltage.v_cell_4 = voltageMeasure.getVoltageBySource(voltageMeasure.b_4);
    cp_voltage.v_batt = voltageMeasure.getVoltageBySource(voltageMeasure.bat);

    cli();
    current_data = cp_current;
    voltage_data = cp_voltage;
    sei();
}

void taskActuate()
{
    cli();
    uint16_t cp_motor_rr = vehicle_actuation_data.pwm_bm_1;
    // uint16_t cp_motor_rl = vehicle_actuation_data.pwm_bm_2;
    // uint16_t cp_motor_fr = vehicle_actuation_data.pwm_bm_3;
    // uint16_t cp_motor_fl = vehicle_actuation_data.pwm_bm_4;
    uint16_t cp_steer = vehicle_actuation_data.pwm_ss;

    uint16_t cp_n_dead_switch = dead_switch;
    sei();

    // Only if deadswitch is not activated (reversed logic), then we can actuate the vehicle.
    if (!cp_n_dead_switch)
    {
        if (vehicle_actuate.isDisabled())
        {
            vehicle_actuate.enableAll();
        }

        vehicle_actuate.write(vehicle_actuate.M_RR, cp_motor_rr);
        vehicle_actuate.write(vehicle_actuate.SS, cp_steer);
    }
    else
    {
    interrupts_enable();
        if (!vehicle_actuate.isDisabled())
        {
            vehicle_actuate.disableAll();
        }
    }
}

void taskReadIMU()
{
    if (!imu_error)
    {
        imu.computeData();

        movement cp_movement_data = {0};

        cp_movement_data.roll = imu.getDataBySource(imu.pitch); // Change these because of orientation of the IMU
        cp_movement_data.pitch = imu.getDataBySource(imu.roll); // Change these because of orientation of the IMU
        cp_movement_data.yaw = imu.getDataBySource(imu.yaw);

        cp_movement_data.w = imu.getDataBySource(imu.w);
        cp_movement_data.i = imu.getDataBySource(imu.j); // Change these because of orientation of the IMU
        cp_movement_data.j = imu.getDataBySource(imu.i); // Change these because of orientation of the IMU
        cp_movement_data.k = imu.getDataBySource(imu.k);

        cli();
        movement_data = cp_movement_data;
        sei();
    }
}

void taskLog()
{
    cli();
    radio_t cp_radio_data = radio_data;
    encoder_t cp_encoder = encoder_data;
    uint8_t cp_dead_switch = dead_switch;
    temperature_t cp_temperature_data = temperature_data;
    current_t cp_current_data = current_data;
    voltage_t cp_voltage_data = voltage_data;
    movement_t cp_movement_data = movement_data;
    vehicle_actuation_t cp_vehicle_actuation_data = vehicle_actuation_data;
    localization_t cp_gps_data = gps_data;
    sei();

    logging_data_t cp_log_data = {0};
    cp_log_data.timestamp = millis();
    cp_log_data.rpm_wheel_1 = cp_encoder.rpm_wheel_1;
    cp_log_data.rpm_wheel_2 = cp_encoder.rpm_wheel_2;
    cp_log_data.rpm_wheel_3 = cp_encoder.rpm_wheel_3;
    cp_log_data.rpm_wheel_4 = cp_encoder.rpm_wheel_4;

    cp_log_data.t_llc = cp_temperature_data.t_llc;
    cp_log_data.t_hlc = cp_temperature_data.t_hlc;
    cp_log_data.t_bm_1 = cp_temperature_data.t_bm_1;
    cp_log_data.t_esc_1 = cp_temperature_data.t_esc_1;
    cp_log_data.t_ss = cp_temperature_data.t_ss;
    cp_log_data.t_bp = cp_temperature_data.t_bp;

    cp_log_data.i_esc_1 = cp_current_data.i_esc_1;
    cp_log_data.i_hlc = cp_current_data.i_hlc;
    cp_log_data.i_llc = cp_current_data.i_llc;
    cp_log_data.i_ss = cp_current_data.i_ss;

    cp_log_data.v_cell_1 = cp_voltage_data.v_cell_1;
    cp_log_data.v_cell_2 = cp_voltage_data.v_cell_2;
    cp_log_data.v_cell_3 = cp_voltage_data.v_cell_3;
    cp_log_data.v_cell_4 = cp_voltage_data.v_cell_4;
    cp_log_data.v_batt = cp_voltage_data.v_batt;
    cp_log_data.v_hlc = cp_voltage_data.v_hlc;
    cp_log_data.v_llc = cp_voltage_data.v_llc;
    cp_log_data.v_phrp_3v3 = cp_voltage_data.v_phrp_3v3;

    cp_log_data.yaw = cp_movement_data.yaw;
    cp_log_data.pitch = cp_movement_data.pitch;
    cp_log_data.roll = cp_movement_data.roll;

    cp_log_data.w = cp_movement_data.w;
    cp_log_data.i = cp_movement_data.i;
    cp_log_data.j = cp_movement_data.j;
    cp_log_data.k = cp_movement_data.k;

    cp_log_data.ch_1 = cp_radio_data.ch_1;
    cp_log_data.ch_2 = cp_radio_data.ch_2;
    cp_log_data.ch_3 = cp_radio_data.ch_3;
    cp_log_data.ch_4 = cp_radio_data.ch_4;
    cp_log_data.ch_5 = cp_radio_data.ch_5;
    cp_log_data.ch_6 = cp_radio_data.ch_6;
    cp_log_data.ch_7 = cp_radio_data.ch_7;
    cp_log_data.ch_8 = cp_radio_data.ch_8;
    cp_log_data.ch_9 = cp_radio_data.ch_9;
    cp_log_data.ch_10 = cp_radio_data.ch_10;
    cp_log_data.ch_11 = cp_radio_data.ch_11;
    cp_log_data.ch_12 = cp_radio_data.ch_12;
    cp_log_data.ch_13 = cp_radio_data.ch_13;
    cp_log_data.ch_14 = cp_radio_data.ch_14;

    cp_log_data.pwm_bm_1 = cp_vehicle_actuation_data.pwm_bm_1;
    cp_log_data.pwm_ss = cp_vehicle_actuation_data.pwm_ss;

    cp_log_data.failsafe = cp_radio_data.failsafe;

    cp_log_data.longitude = cp_gps_data.longitude;
    cp_log_data.latitude = cp_gps_data.latitude;
    cp_log_data.altitude = cp_gps_data.altitude;
    cp_log_data.velocity = cp_gps_data.velocity;
    cp_log_data.date = cp_gps_data.date;
    cp_log_data.time = cp_gps_data.time;
    cp_log_data.c_s = cp_gps_data.connected_satellites;

    cp_log_data.dead_switch = cp_dead_switch;

    //Serial.println(log_data.w);

    cli();
    log_data = cp_log_data;
    sei();
}

void taskSaveLog()
{
    if (!sd_error)
    {
        cli();
        uint8_t cp_dead_switch = dead_switch;
        logging_data_t cp_log_data = log_data;
        sei();

        if (!cp_dead_switch)
        {
            if (newWriteFile)
            {
                // sd_logger.generateUniqueFilenameAndUse();
                if (sd_logger.init())
                {
                    newWriteFile = false;
                    // Serial.print("Creating the file: ");
                    // Serial.println(sd_logger.getFileName());
                }
                else
                {
                    // Serial.println("Failed to init!");
                    sd_error = 1;
                }
            }

            if (!newWriteFile)
            {
                sd_logger.logData(cp_log_data);
                // Serial.print("file size:  ");
                // Serial.println(sd_logger.getFileSize());
            }
        }
        else
        {
            if (!newWriteFile)
            {
                sd_logger.closeFile();
                newWriteFile = true;
                // Serial.print("Closing the file: ");
                // Serial.println(sd_logger.getFileName());
            }
        }
    }
}

void taskWriteBaseStation()
{
    if (!bs_error)
    {
        cli();
        logging_data_t cp_logging_data = log_data;
        sei();

        //Set bs_error on 1 failure. This to prevent hangings.
        bs_error = !baseStation.sendData(cp_logging_data);
        //uint8_t temp = baseStation.sendData(cp_logging_data);
        if(bs_error){
            Serial.println("An error in sending has occured");
            Serial.println(bs_error);
        }
    }
}

void taskSafety()
{
    cli();
    uint16_t cp_ch_10 = radio_data.ch_10;
    uint8_t temp_dead_switch = true;

    if (cp_ch_10 >= 1850 && cp_ch_10 <= 2050){
        temp_dead_switch = false;
    }
        
    dead_switch = temp_dead_switch;
    sei();
}

void sysTick()
{
    task_isr_timer.run();
}