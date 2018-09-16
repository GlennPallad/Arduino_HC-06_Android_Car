#include <Arduino.h>

#define RIGHT_PWM			3
#define LEFT_PWM			2
#define RIGHT_MOTOR_IN1		50
#define RIGHT_MOTOR_IN2		51
#define LEFT_MOTOR_IN1		52
#define LEFT_MOTOR_IN2		53

void initMotors();
void rightForward();
void rightBack();
void leftForward();
void leftBack();

void forward();
void back();
void turnLeft();
void turnRight();

void leftStop();
void rightStop();
void stop();
