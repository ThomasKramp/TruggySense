/**
 * micro-ROS test sketch
 *
 * Tests micro-ROS on a Teensy 4.1 module
 * connected to a host computer.
 */


#include <Arduino.h>                  // Must include this first for Arduino functions
#include <micro_ros_platformio.h>
#include <rcl/rcl.h>
#include <rclc/rclc.h>
#include <std_msgs/msg/int32.h>

#ifndef LED_BUILTIN
  #define LED_BUILTIN 13
#endif

// micro-ROS objects
rcl_publisher_t publisher;
rcl_node_t node;
rclc_support_t support;
rcl_allocator_t allocator;
std_msgs__msg__Int32 msg;

bool connected = false;

void setup() {
  // Initialize Serial and LED
  Serial.begin(115200);
  delay(2000);
  pinMode(LED_BUILTIN, OUTPUT);

  // Set up micro-ROS transport
  set_microros_serial_transports(Serial);

  allocator = rcl_get_default_allocator();

  // Attempt to connect
  while (!connected) {
    Serial.println("Trying to connect to micro-ROS agent...");
    rcl_ret_t ret;

    // Initialize support
    ret = rclc_support_init(&support, 0, NULL, &allocator);
    if (ret != RCL_RET_OK) {
      Serial.println("Failed to init support, retrying...");
      delay(1000);
      continue;
    }

    // Initialize node
    ret = rclc_node_init_default(&node, "teensy_node", "", &support);
    if (ret != RCL_RET_OK) {
      Serial.println("Failed to init node, retrying...");
      rclc_support_fini(&support);
      delay(1000);
      continue;
    }

    // Initialize publisher
    ret = rclc_publisher_init_default(
      &publisher,
      &node,
      ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32),
      "teensy_counter"
    );
    if (ret != RCL_RET_OK) {
      Serial.println("Failed to init publisher, retrying...");
      rcl_node_fini(&node);
      rclc_support_fini(&support);
      delay(1000);
      continue;
    }

    // Successfully connected
    connected = true;
    Serial.println("Connected to micro-ROS agent!");
  }

  msg.data = 0;
}

void loop() {
  // Blink LED
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);

  // Publish micro-ROS message
  msg.data++;
  rcl_publish(&publisher, &msg, NULL);
}