#include "Arduino.h"
#include "Aquabotics.h"

Aquabot::Stepper(int ENA, int DIR, int PUL, int delayTime, int pulseWidth, int BUMP1, int BUMP2) {
	pinMode (PUL,OUTPUT);
	pinMode (DIR,OUTPUT);
	pinMode (ENA,OUTPUT);
	pinMode (BUMP1, INPUT_PULLUP);
	pinMode (BUMP1, INPUT_PULLUP);
	
	digitalWrite(ENA,HIGH); //turns off
	
	_PUL = PUL;
	_DIR = DIR;
	_ENA = ENA;
	_delayTime = delayTime;
	_pulseWidth = pulseWidth;
	_BUMP1 = BUMP1;
	_BUMP2 = BUMP2;
}

void Aquabot::stepMotor() {
	digitalWrite(_PUL,HIGH);
	delayMicroseconds(_pulseWidth);
	digitalWrite(_PUL,LOW);
	delayMicroseconds(_delayTime);
}

void Aquabot::enable() {
	digitalWrite(_ENA, LOW);
}

void Aquabot::disable() {
	digitalWrite(_ENA, HIGH);
}

void Aquabot::up() {
	digitalWrite(_DIR, HIGH);
}

void Aquabot::down() {
	digitalWrite(_DIR, LOW);
}

void Aquabot::detectCollision() {
	if (digitalRead(_BUMP1) == LOW || digitalRead(_BUMP2) == LOW) {
		Serial.println("object detected!");
		digitalWrite(_DIR, !digitalRead(_DIR));
		while (digitalRead(_BUMP1) == LOW || digitalRead(_BUMP2) == LOW){
			stepMotor();
		}
		Serial.println("object cleared!");
		Serial.println();
	}
}