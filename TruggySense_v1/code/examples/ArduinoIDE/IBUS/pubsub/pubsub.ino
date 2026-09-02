/*
 * rosserial PubSub Example
 * Prints "hello world!" and toggles led
 */

#include <ros.h>
// #include <std_msgs/String.h>
// #include <std_msgs/Empty.h>
#include <std_msgs/Int16.h>

ros::NodeHandle  node_handle;


// void messageCb( const std_msgs::Empty& toggle_msg){
//   digitalWrite(13, HIGH-digitalRead(13));   // blink the led
// }

//ros::Subscriber<std_msgs::Empty> sub("toggle_led", messageCb );

std_msgs::Int16 channel_msg;
ros::Publisher pub_radio("radio", &channel_msg);

void setup()
{
  pinMode(13, OUTPUT);
  node_handle.initNode();
  node_handle.advertise(pub_radio);
  //node_handle.subscribe(sub);
}

int channel1 = 1000;

void loop()
{
  channel_msg.data = channel1;
  pub_radio.publish( &channel_msg );
  node_handle.spinOnce();

  channel1++;

  if(channel1>=2000)
    channel1=1000;

  delay(500);
}
