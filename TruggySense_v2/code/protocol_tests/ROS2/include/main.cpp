/**
 * micro-ROS sketch - Teensy 4.1
 * Publishes all logging_data_t fields grouped by domain.
 * Subscribes to /hello (std_msgs/String).
 */

#include <Arduino.h>
#include <micro_ros_platformio.h>

#include <rcl/rcl.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>

// Message types
#include <std_msgs/msg/string.h>
#include <std_msgs/msg/u_int16_multi_array.h>
#include <std_msgs/msg/float32_multi_array.h>
#include <sensor_msgs/msg/nav_sat_fix.h>
#include <sensor_msgs/msg/imu.h>
#include <geometry_msgs/msg/vector3_stamped.h>

#ifndef LED_BUILTIN
  #define LED_BUILTIN 13
#endif

// ── Struct definition ────────────────────────────────────────────────────────

struct logging_data {
    uint32_t timestamp  = 0;
    uint32_t date       = 0;
    uint32_t time       = 0;
    float latitude      = 0;
    float longitude     = 0;
    float velocity      = 0;
    float altitude      = 0;
    uint8_t c_s         = 0;
    uint16_t ch_1  = 0, ch_2  = 0, ch_3  = 0, ch_4  = 0, ch_5  = 0;
    uint16_t ch_6  = 0, ch_7  = 0, ch_8  = 0, ch_9  = 0, ch_10 = 0;
    uint8_t failsafe    = 0;
    float rpm_wheel_1 = 0, rpm_wheel_2 = 0, rpm_wheel_3 = 0, rpm_wheel_4 = 0;
    float v_cell_1 = 0, v_cell_2 = 0, v_cell_3 = 0, v_cell_4 = 0;
    float v_rail_3V3 = 0, v_rail_5v0 = 0;
    uint8_t i_esc_1 = 0, i_esc_2 = 0, i_esc_3 = 0, i_esc_4 = 0;
    float i_etc = 0, i_llc = 0, i_hlc = 0;
    float t_llc = 0, t_hlc = 0, t_bp = 0, t_ss = 0;
    float t_bm_1 = 0, t_bm_2 = 0, t_bm_3 = 0, t_bm_4 = 0;
    float t_esc_1 = 0, t_esc_2 = 0, t_esc_3 = 0, t_esc_4 = 0;
    float roll = 0, pitch = 0, yaw = 0;
    float w = 0, i = 0, j = 0, k = 0;
};
typedef struct logging_data logging_data_t;
logging_data_t payload;

// ── micro-ROS core ───────────────────────────────────────────────────────────

rcl_allocator_t   allocator;
rclc_support_t    support;
rcl_node_t        node;
rclc_executor_t   executor;
bool connected = false;

// ── Publishers ───────────────────────────────────────────────────────────────

rcl_publisher_t pub_gps;        // sensor_msgs/NavSatFix   → /robot/gps
rcl_publisher_t pub_velocity;   // geometry_msgs/Vector3Stamped → /robot/velocity
rcl_publisher_t pub_rc;         // std_msgs/UInt16MultiArray   → /robot/rc_channels
rcl_publisher_t pub_rpm;        // std_msgs/Float32MultiArray  → /robot/wheel_rpm
rcl_publisher_t pub_voltage;    // std_msgs/Float32MultiArray  → /robot/voltage
rcl_publisher_t pub_current;    // std_msgs/Float32MultiArray  → /robot/current
rcl_publisher_t pub_temp;       // std_msgs/Float32MultiArray  → /robot/temperature
rcl_publisher_t pub_imu;        // sensor_msgs/Imu             → /robot/imu

sensor_msgs__msg__NavSatFix           msg_gps;
geometry_msgs__msg__Vector3Stamped    msg_velocity;
std_msgs__msg__UInt16MultiArray       msg_rc;
std_msgs__msg__Float32MultiArray      msg_rpm;
std_msgs__msg__Float32MultiArray      msg_voltage;
std_msgs__msg__Float32MultiArray      msg_current;
std_msgs__msg__Float32MultiArray      msg_temp;
sensor_msgs__msg__Imu                 msg_imu;

// Backing arrays for MultiArray messages (statically allocated)
uint16_t rc_data[10];
float    rpm_data[4];
float    voltage_data[6];   // 4 cells + 3V3 rail + 5V rail
float    current_data[7];   // 4 ESC + etc + llc + hlc
float    temp_data[12];     // llc, hlc, bp, ss, 4×bm, 4×esc

// ── Subscriber ───────────────────────────────────────────────────────────────

rcl_subscription_t sub_hello;
std_msgs__msg__String msg_hello;
char hello_buf[64];  // backing buffer for the incoming string

void hello_callback(const void * msgin) {
    const std_msgs__msg__String * msg =
        (const std_msgs__msg__String *)msgin;
    Serial.print("[HELLO] Received: ");
    Serial.println(msg->data.data);
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN)); // toggle LED on receipt
}

// ── Helpers ──────────────────────────────────────────────────────────────────

// Build a builtin_interfaces__msg__Time from millis() (no real clock source here)
static builtin_interfaces__msg__Time now_stamp() {
    builtin_interfaces__msg__Time t;
    uint32_t ms = millis();
    t.sec     = ms / 1000;
    t.nanosec = (ms % 1000) * 1000000U;
    return t;
}

// Attach a static C-array to a Float32MultiArray (no heap allocation)
static void assign_float_array(std_msgs__msg__Float32MultiArray * msg,
                               float * buf, size_t len) {
    msg->data.data     = buf;
    msg->data.size     = len;
    msg->data.capacity = len;
}

static void assign_uint16_array(std_msgs__msg__UInt16MultiArray * msg,
                                uint16_t * buf, size_t len) {
    msg->data.data     = buf;
    msg->data.size     = len;
    msg->data.capacity = len;
}

// ── Init helpers ─────────────────────────────────────────────────────────────

bool init_publishers() {
    rcl_ret_t ret;

    // GPS
    ret = rclc_publisher_init_default(&pub_gps, &node,
        ROSIDL_GET_MSG_TYPE_SUPPORT(sensor_msgs, msg, NavSatFix), "robot/gps");
    if (ret != RCL_RET_OK) return false;

    // Velocity
    ret = rclc_publisher_init_default(&pub_velocity, &node,
        ROSIDL_GET_MSG_TYPE_SUPPORT(geometry_msgs, msg, Vector3Stamped), "robot/velocity");
    if (ret != RCL_RET_OK) return false;

    // RC channels
    ret = rclc_publisher_init_default(&pub_rc, &node,
        ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, UInt16MultiArray), "robot/rc_channels");
    if (ret != RCL_RET_OK) return false;

    // Wheel RPM
    ret = rclc_publisher_init_default(&pub_rpm, &node,
        ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Float32MultiArray), "robot/wheel_rpm");
    if (ret != RCL_RET_OK) return false;

    // Voltage
    ret = rclc_publisher_init_default(&pub_voltage, &node,
        ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Float32MultiArray), "robot/voltage");
    if (ret != RCL_RET_OK) return false;

    // Current
    ret = rclc_publisher_init_default(&pub_current, &node,
        ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Float32MultiArray), "robot/current");
    if (ret != RCL_RET_OK) return false;

    // Temperature
    ret = rclc_publisher_init_default(&pub_temp, &node,
        ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Float32MultiArray), "robot/temperature");
    if (ret != RCL_RET_OK) return false;

    // IMU
    ret = rclc_publisher_init_default(&pub_imu, &node,
        ROSIDL_GET_MSG_TYPE_SUPPORT(sensor_msgs, msg, Imu), "robot/imu");
    if (ret != RCL_RET_OK) return false;

    return true;
}

bool init_subscriber() {
    // Give the incoming String message a backing buffer
    msg_hello.data.data     = hello_buf;
    msg_hello.data.size     = 0;
    msg_hello.data.capacity = sizeof(hello_buf);

    rcl_ret_t ret = rclc_subscription_init_default(&sub_hello, &node,
        ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, String), "hello");
    return (ret == RCL_RET_OK);
}

// ── Publish helpers ──────────────────────────────────────────────────────────

void publish_gps(const logging_data_t & p) {
    msg_gps.header.stamp    = now_stamp();
    msg_gps.latitude        = p.latitude;
    msg_gps.longitude       = p.longitude;
    msg_gps.altitude        = p.altitude;
    // STATUS_FIX = 0; COVARIANCE_TYPE_UNKNOWN = 0
    msg_gps.status.status   = 0;
    rcl_publish(&pub_gps, &msg_gps, NULL);
}

void publish_velocity(const logging_data_t & p) {
    msg_velocity.header.stamp = now_stamp();
    msg_velocity.vector.x     = p.velocity; // scalar speed on X axis
    msg_velocity.vector.y     = 0;
    msg_velocity.vector.z     = 0;
    rcl_publish(&pub_velocity, &msg_velocity, NULL);
}

void publish_rc(const logging_data_t & p) {
    rc_data[0] = p.ch_1;  rc_data[1] = p.ch_2;  rc_data[2] = p.ch_3;
    rc_data[3] = p.ch_4;  rc_data[4] = p.ch_5;  rc_data[5] = p.ch_6;
    rc_data[6] = p.ch_7;  rc_data[7] = p.ch_8;  rc_data[8] = p.ch_9;
    rc_data[9] = p.ch_10;
    rcl_publish(&pub_rc, &msg_rc, NULL);
}

void publish_rpm(const logging_data_t & p) {
    rpm_data[0] = p.rpm_wheel_1; rpm_data[1] = p.rpm_wheel_2;
    rpm_data[2] = p.rpm_wheel_3; rpm_data[3] = p.rpm_wheel_4;
    rcl_publish(&pub_rpm, &msg_rpm, NULL);
}

void publish_voltage(const logging_data_t & p) {
    voltage_data[0] = p.v_cell_1;   voltage_data[1] = p.v_cell_2;
    voltage_data[2] = p.v_cell_3;   voltage_data[3] = p.v_cell_4;
    voltage_data[4] = p.v_rail_3V3; voltage_data[5] = p.v_rail_5v0;
    rcl_publish(&pub_voltage, &msg_voltage, NULL);
}

void publish_current(const logging_data_t & p) {
    current_data[0] = p.i_esc_1; current_data[1] = p.i_esc_2;
    current_data[2] = p.i_esc_3; current_data[3] = p.i_esc_4;
    current_data[4] = p.i_etc;   current_data[5] = p.i_llc;
    current_data[6] = p.i_hlc;
    rcl_publish(&pub_current, &msg_current, NULL);
}

void publish_temp(const logging_data_t & p) {
    temp_data[0]  = p.t_llc;   temp_data[1]  = p.t_hlc;
    temp_data[2]  = p.t_bp;    temp_data[3]  = p.t_ss;
    temp_data[4]  = p.t_bm_1;  temp_data[5]  = p.t_bm_2;
    temp_data[6]  = p.t_bm_3;  temp_data[7]  = p.t_bm_4;
    temp_data[8]  = p.t_esc_1; temp_data[9]  = p.t_esc_2;
    temp_data[10] = p.t_esc_3; temp_data[11] = p.t_esc_4;
    rcl_publish(&pub_temp, &msg_temp, NULL);
}

void publish_imu(const logging_data_t & p) {
    msg_imu.header.stamp = now_stamp();
    // Quaternion (w, x, y, z) — your struct uses w, i, j, k
    msg_imu.orientation.w = p.w;
    msg_imu.orientation.x = p.i;
    msg_imu.orientation.y = p.j;
    msg_imu.orientation.z = p.k;
    // euler angles stored in angular_velocity for convenience
    // (replace with actual gyro data when available)
    msg_imu.angular_velocity.x = p.roll;
    msg_imu.angular_velocity.y = p.pitch;
    msg_imu.angular_velocity.z = p.yaw;
    rcl_publish(&pub_imu, &msg_imu, NULL);
}

// ── Setup ────────────────────────────────────────────────────────────────────

void setup() {
    Serial.begin(115200);
    delay(2000);
    pinMode(LED_BUILTIN, OUTPUT);

    // Populate test values
    payload.latitude   = 51.1789735f;
    payload.longitude  = 4.416421f;
    payload.velocity   = 1.5f;
    payload.altitude   = 10.0f;
    payload.ch_1  = 1500; payload.ch_2  = 1500; payload.ch_3  = 1000;
    payload.ch_4  = 1500; payload.ch_5  = 1000; payload.ch_6  = 1000;
    payload.v_cell_1   = 4.2f;  payload.v_cell_2   = 8.4f;
    payload.v_cell_3   = 12.6f; payload.v_cell_4   = 16.8f;
    payload.v_rail_3V3 = 3.3f;  payload.v_rail_5v0 = 5.1f;
    payload.t_llc      = 20.0f; payload.t_hlc      = 30.0f;
    payload.t_bp       = 25.0f;
    payload.w          = 1.0f;  // identity quaternion

    // Attach static backing arrays to MultiArray messages
    assign_uint16_array(&msg_rc,      rc_data,      10);
    assign_float_array (&msg_rpm,     rpm_data,      4);
    assign_float_array (&msg_voltage, voltage_data,  6);
    assign_float_array (&msg_current, current_data,  7);
    assign_float_array (&msg_temp,    temp_data,    12);

    set_microros_serial_transports(Serial);
    allocator = rcl_get_default_allocator();

    while (!connected) {
        Serial.println("Connecting to micro-ROS agent...");
        rcl_ret_t ret;

        ret = rclc_support_init(&support, 0, NULL, &allocator);
        if (ret != RCL_RET_OK) { delay(1000); continue; }

        ret = rclc_node_init_default(&node, "teensy_node", "", &support);
        if (ret != RCL_RET_OK) { rclc_support_fini(&support); delay(1000); continue; }

        if (!init_publishers()) {
            rcl_node_fini(&node);
            rclc_support_fini(&support);
            delay(1000);
            continue;
        }

        if (!init_subscriber()) {
            rcl_node_fini(&node);
            rclc_support_fini(&support);
            delay(1000);
            continue;
        }

        // Executor: 1 handle (the subscriber)
        rclc_executor_init(&executor, &support.context, 1, &allocator);
        rclc_executor_add_subscription(&executor, &sub_hello,
            &msg_hello, &hello_callback, ON_NEW_DATA);

        connected = true;
        Serial.println("Connected!");
    }
}

// ── Loop ─────────────────────────────────────────────────────────────────────

void loop() {
    // Spin executor to handle incoming /hello messages
    rclc_executor_spin_some(&executor, RCL_MS_TO_NS(10));

    // Publish all topics
    publish_gps(payload);
    publish_velocity(payload);
    publish_rc(payload);
    publish_rpm(payload);
    publish_voltage(payload);
    publish_current(payload);
    publish_temp(payload);
    publish_imu(payload);

    // Blink LED at 1 Hz
    digitalWrite(LED_BUILTIN, HIGH); delay(500);
    digitalWrite(LED_BUILTIN, LOW);  delay(500);
}