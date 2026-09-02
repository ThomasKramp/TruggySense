#include "ibus.h"
#include <ros.h>
#include <std_msgs/Int16.h>
#include <Servo.h> 

//ROS objects
ros::NodeHandle node_handle;
std_msgs::Int16 channel_msg;
ros::Publisher pub_radio("radio", &channel_msg);

//Callbacks for subscribes
void cbVehicleControl
ros::Subscriber<std_msgs::Int16> sub("toggle_led", messageCb );



// servo objects
Servo steer_servo;
Servo motor_RR;
Servo motor_RL;
Servo motor_LR;
Servo motor_LL;

void setup() {
  // put your setup code here, to run once:
  pinMode(13, OUTPUT);
  Serial.begin(115200);
  Serial.println("Testing");

  //RC-communication initialize
  initIBUS(Serial8);

  //Initialize servos
  motor_RR.attach(22); //22 motor RR
  steer_servo.attach(33); //33 Servo

  //rosserial initialize
  node_handle.initNode();
  node_handle.advertise(pub_radio);
}

void loop() {
  //Read out the data from IBUS
  readData();

  //Publish the readed out data if success right after this
  channel_msg.data = getChannel(0);
  pub_radio.publish( &channel_msg );
  node_handle.spinOnce();

  //Control servo objects
  motor_RR.writeMicroseconds(getChannel(0));
  steer_servo.writeMicroseconds(getChannel(1));

  delay(5);
}
