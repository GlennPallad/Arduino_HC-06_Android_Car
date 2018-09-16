#include "motor.h"

void initMotors() {
	delay(10);
	pinMode(RIGHT_MOTOR_IN1, OUTPUT);
	pinMode(RIGHT_MOTOR_IN2, OUTPUT);
	pinMode(LEFT_MOTOR_IN1, OUTPUT);
	pinMode(LEFT_MOTOR_IN2, OUTPUT);
	pinMode(RIGHT_PWM, OUTPUT);
	pinMode(LEFT_PWM, OUTPUT);
	delay(10);
}

void rightForward() {
	digitalWrite(RIGHT_MOTOR_IN1, LOW);
	digitalWrite(RIGHT_MOTOR_IN2, HIGH);
}

void rightBack() {
	digitalWrite(RIGHT_MOTOR_IN1, HIGH);
	digitalWrite(RIGHT_MOTOR_IN2, LOW);
}

void leftForward() {
	digitalWrite(LEFT_MOTOR_IN1, HIGH);
	digitalWrite(LEFT_MOTOR_IN2, LOW);
}

void leftBack() {
	digitalWrite(LEFT_MOTOR_IN1, LOW);
	digitalWrite(LEFT_MOTOR_IN2, HIGH);
}

void forward() {
	rightForward();
	leftForward();
}

void back() {
	rightBack();
	leftBack();
}

void turnLeft() {
	rightForward();
	delay(60);
	rightStop();
}

void turnRight() {
	leftForward();
	delay(60);
	leftStop();
}

void leftStop() {
	digitalWrite(LEFT_MOTOR_IN1, LOW);
	digitalWrite(LEFT_MOTOR_IN2, LOW);
}

void rightStop() {
	digitalWrite(RIGHT_MOTOR_IN1, LOW);
	digitalWrite(RIGHT_MOTOR_IN2, LOW);
}

void stop() {
	leftStop();
	rightStop();
}