#include "motor.h"
#include "HC_SR04.h"
#include "soft_serial.h"

SoftwareSerial softSerial(SOFT_RX, SOFT_TX);
int speeds[3] = {100, 175, 250};
int currentSpeed = speeds[0];
// 0 safe to go, 1 stop.
int collisionAvoidanceFlag = 0;
int isCurrentStatusD = 0;
void setup() {
	// Initialize hard serial.
	Serial.begin(9600);
	// Initialize soft serial.
	softSerial.begin(9600);
	// Initialize motors.
	initMotors();
	analogWrite(RIGHT_PWM, currentSpeed);
	analogWrite(LEFT_PWM, currentSpeed);
	// Initialize HC_SR04
	initHC_SR04();
}

void loop() {
	// Uncomment to enable Loopback test(comment all other codes in loop()).
	// loopBackTest();

	startHC_SR04();
	if (distance < 30 && isCurrentStatusD)
	{
		stop();
		collisionAvoidanceFlag = 1;
	} else {
		collisionAvoidanceFlag = 0;
	}

	// Get message from HC-0x.
	String cmd = soft_RX_getMessage();
	if (cmd != "")
	{
		// Uncomment to forward cmd to TX(monitor), so we can see what was received from SOFT_RX.
		Serial.println(cmd);
		
		if (cmd == "D" && collisionAvoidanceFlag != 1)
		{
			forward();
			isCurrentStatusD = 1;
		} else if (cmd == "R")
		{
			back();
			isCurrentStatusD = 0;
		} else if (cmd == "Left" && collisionAvoidanceFlag != 1)
		{
			turnLeft();
		} else if (cmd == "Right" && collisionAvoidanceFlag != 1)
		{
			turnRight();
		} else if (cmd == "P")
		{
			stop();
			isCurrentStatusD = 0;
		} else if (cmd == "Speed_1")
		{
			currentSpeed = speeds[0];
			analogWrite(RIGHT_PWM, currentSpeed);
			analogWrite(LEFT_PWM, currentSpeed);
			Serial.print("currentSpeed: ");
			Serial.println(currentSpeed);
		} else if (cmd == "Speed_2")
		{
			currentSpeed = speeds[1];
			analogWrite(RIGHT_PWM, currentSpeed);
			analogWrite(LEFT_PWM, currentSpeed);
			Serial.print("currentSpeed: ");
			Serial.println(currentSpeed);
		} else if (cmd == "Speed_3")
		{
			currentSpeed = speeds[2];
			analogWrite(RIGHT_PWM, currentSpeed);
			analogWrite(LEFT_PWM, currentSpeed);
			Serial.print("currentSpeed: ");
			Serial.println(currentSpeed);
		} else {
			Serial.println("Something went wrong: "+cmd);;
		}
	}
	cmd = "";
	delay(10);
}

String soft_RX_getMessage(){
	String s_msg = "";
	char a;
	while(softSerial.available()) {
		a = softSerial.read();
		s_msg+=String(a);
	}
	return s_msg;
}

void loopBackTest() {
	// Initialize hard serial.
	Serial.begin(9600);
	softSerial.println("biu");
	Serial.println(RX_getMessage());
	delay(1000);
}

String RX_getMessage(){
	String msg = "";
	char a;

	while(Serial.available()) {
		a = Serial.read();
		msg+=String(a);
	}
	return msg;
}