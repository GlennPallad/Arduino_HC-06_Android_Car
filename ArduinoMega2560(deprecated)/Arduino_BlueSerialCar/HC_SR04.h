#define TRIG 49
#define ECHO 48
#define SONIC 0.034

long duration;
int distance;

void initHC_SR04 () {
	pinMode(TRIG, OUTPUT);
	pinMode(ECHO, INPUT);
}

// Must be placed in loop().
void startHC_SR04 () {
	digitalWrite(TRIG, LOW);
	delayMicroseconds(2);

	digitalWrite(TRIG, HIGH);
	delayMicroseconds(10);

	duration = pulseIn(ECHO, HIGH);
	distance = duration * SONIC/2;
}