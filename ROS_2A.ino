/*
 * rosserial Publisher Example
 * Prints "hello world!"
 */
#define USE_USBCON

#include <ros.h>
#include <geometry_msgs/Twist.h>
#include <std_msgs/Int8MultiArray.h>

#include <Zumo32U4.h>

ros::NodeHandle_<ArduinoHardware, 1, 1, 500, 100>  nh;

std_msgs::Int8MultiArray std_prox;
ros::Publisher proximity("proximity", &std_prox);


void ros_handler( const geometry_msgs::Twist& cmd_msg){
  float x = cmd_msg.linear.x;
  float z = cmd_msg.angular.z;

  if (z > 0) left(100);
  else if (z < 0) right(100);
  
  if (x < 0) backward(100);
  else if (x > 0) forward(100);
  
  stop();
}
ros::Subscriber<geometry_msgs::Twist>sub("/turtle1/cmd_vel", ros_handler);

Zumo32U4Motors motors;
Zumo32U4ProximitySensors proxSensors;

void setup()
{
  nh.initNode();
  nh.subscribe(sub);
  nh.advertise(proximity);

  proxSensors.initThreeSensors();
  uint16_t defaultBrightnessLevels[] = {1,2,3,4,5,6,7,8,9,10};
  proxSensors.setBrightnessLevels(defaultBrightnessLevels, 10);
}

void publishProx()
{
  proxSensors.read();
  int8_t prox[6] = {
          proxSensors.countsLeftWithLeftLeds(),
          proxSensors.countsLeftWithRightLeds(),
          proxSensors.countsFrontWithLeftLeds(),
          proxSensors.countsFrontWithRightLeds(),
          proxSensors.countsRightWithLeftLeds(),
          proxSensors.countsRightWithRightLeds()
          };

  std_prox.data = prox;
  std_prox.data_length = 6;
  
  proximity.publish( &std_prox );
}

void loop()
{
  static uint16_t lastSampleTime = 0;

  if ((uint16_t)(millis() - lastSampleTime) >= 200)
  {
    publishProx();
    lastSampleTime = millis();
  }
  
  nh.spinOnce();
  //delay(1);
}

void forward(int time) {
  runMotors(100,100);
  delay(time);
}

void backward(int time) {
  runMotors(-100,-100);
  delay(time);
}

void left(int time) {
  runMotors(-100,100);
  delay(time);
}

void right(int time) {
  runMotors(100,-100);
  delay(time);
}

void stop()
{
  runMotors(0,0);
}

void runMotors( int left, int right)
{
  motors.setLeftSpeed(left);
  motors.setRightSpeed(right);
}
