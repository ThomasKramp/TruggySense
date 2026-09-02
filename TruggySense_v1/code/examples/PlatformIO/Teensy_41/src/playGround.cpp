#include "X6B.h"
#include "encoderv2.h"

// #include <std_msgs/Int16.h>
// #include "ros.h"
// #include <std_msgs/UInt16MultiArray.h>
// #include <std_msgs/Int16.h>
#include <Servo.h>

// ROS objects

//
// ros::NodeHandle node_handle;
// std_msgs::UInt16MultiArray channel_msg;

// ros::Publisher pub_radio("radio", &channel_msg);

// void vehicleMovementCb(const std_msgs::UInt16MultiArray &movement_data);
// ros::Subscriber<std_msgs::UInt16MultiArray> sub_vehicle_movement("vehicle_movement", vehicleMovementCb);

// servo objects
Servo steer_servo;
Servo motor_RR;
Servo motor_RL;
Servo motor_LR;
Servo motor_LL;

// volatile long enc_w_fr_c = 0;

// volatile long t0 = 0;
// volatile long t1 = 0;
// volatile bool time_toggler = true;
// volatile long delta = 0;
// volatile float f_pulse = 0;
// volatile float f_wheel = 0;
// volatile float rpm_wheel = 0;
// volatile float v = 0;

EncoderV2 encoder;

// void it_w_fr();
void actuate();

void setup()
{
  // put your setup code here, to run once:
  pinMode(13, OUTPUT);
  // digitalWrite(13, !digitalRead(13));

  Serial.begin(9600);
  // attachInterrupt(5, it_w_fr, FALLING);

  encoder = EncoderV2();

  // RC-communication initialize
  initX6B(Serial8);

  // Initialize servos
  motor_RR.attach(22);    // 22 motor RR
  steer_servo.attach(33); // 33 Servo

  // // rosserial initialize
  // node_handle.initNode();
  // node_handle.advertise(pub_radio);
  // node_handle.subscribe(sub_vehicle_movement);
}

void loop()
{
  // Read out the data from IBUS
  readData();

  // Publish the readed out data if success right after this
  // channel_msg.data_length = FRAME_LENGTH - 2;
  // channel_msg.data = channel.data();

  // pub_radio.publish(&channel_msg);
  // node_handle.spinOnce();

  // steer_servo.writeMicroseconds(getChannel(3));

  // Serial.println(getChannel(0));

  //Move car
  //actuate();

  // Serial.print(t0);
  // Serial.print(" ");
  // Serial.print(t1);
  // Serial.print(" ");
  // Serial.print(delta);
  // Serial.print(" ");
  // Serial.print(f_pulse);
  // Serial.print(" ");
  // Serial.print(encoder.getWheelFrequency(FR));

  // Serial.println(millis());
  // Serial.print("FR ");
  // Serial.println(encoder.getWheelCounter(FR));
  // Serial.print("FL ");
  // Serial.println(encoder.getWheelCounter(FL));
  // Serial.print("RR ");
  // Serial.println(encoder.getWheelCounter(RR));
  // Serial.print("RL ");
  // Serial.println(encoder.getWheelCounter(RL));
  // Serial.println(" ");

  // Serial.println(millis());
  // Serial.print("FR ");
  // Serial.println(encoder.getWheelSpeed(FR));
  // Serial.print("FL ");
  // Serial.println(encoder.getWheelSpeed(FL));
  // Serial.print("RR ");
  // Serial.println(encoder.getWheelSpeed(RR));
  // Serial.print("RL ");
  // Serial.println(encoder.getWheelSpeed(RL));
  // Serial.println(" ");

  actuate();

  // Serial.println(time_toggler);
  digitalWrite(13, !digitalRead(13));
  delay(166);
}

void actuate(){
  motor_RR.writeMicroseconds(getChannel(0));
  steer_servo.writeMicroseconds(getChannel(1));
}

// void it_w_fr(){
//   //Atomic operation
//   cli();
//   if(time_toggler)
//   {
//     time_toggler = false;
//     t0 = micros();
//     delta = t0-t1;
//   }
//   else
//   {
//     time_toggler = true;
//     t1 = micros();
//     delta = t1-t0;
//   }

//   f_pulse = 1/(delta/1000000.0);

//   f_wheel = f_pulse/6.0;

//   rpm_wheel = f_wheel * 60;

//   //enc_w_fr_c++; //Not used in this setup

  
//   sei();
// }

// void vehicleMovementCb(const std_msgs::UInt16MultiArray& movement_data)
// {
//   motor_RR.writeMicroseconds(movement_data.data[0]);
//   steer_servo.writeMicroseconds(movement_data.data[1]);
// }