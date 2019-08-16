
#include "Aquabotics.h"

//Variable Defintions
/*Need to provide:
	Stepper
		Pins for ENA,DIR,PUL
		Time for delay and pulse width
		Pins for cooresponding bumper switches
	Time
		Current time, time adjust on/off times
		pin for relay
*/


//plateform motor pins
int PUL1=10; //define Pulse pin
int DIR1=9; //define Direction pin
int ENA1=11; //define Enable Pin

//The delayTime is the time (in microseconds) between steps
int delayTime1 = 800; //between 0 and 32767
//pulseWidth is the time (in microseconds) it waits for the motor to make a step
int pulseWidth1 = 50; //this should be kept constant

//creating stepper motor instances: sets pinmodes, disables the motor, 
Stepper plateform(int ENA1, int DIR1, int PUL1, int delayTime1, int pulseWidth1, int BUMP1, int BUMP2)
Stepper track(int ENA2, int DIR2, int PUL2, int delayTime2, int pulseWidth2, int BUMP3, int BUMP4)

void setup() {
	Serial.begin(9600);
  
	Serial.println("Serial Commands for Stepper Control:");
	Serial.println("    '+' enables the plateform motor");
	Serial.println("    '-' disables the plateform motor");
	Serial.println("    '0' enables the track motor");
	Serial.println("    '9' disables the track motor");
	Serial.println("    'w' sends plateform up");
	Serial.println("    's' sends plateform down");
	Serial.println("    'a' sends track left?");
	Serial.println("    'd' sends track right?");
}

void loop() {
	handleSerial();

	plateform.stepMotor();
	track.stepMotor();

	plateform.detectCollision();
	track.detectCollision();
}


void handleSerial() {
	while (Serial.available() > 0) {
		char incomingCharacter = Serial.read();
		switch (incomingCharacter) {
			case 'w':
				plateform.up();
				break;
			case 's':
				plateform.down();
				break;
			case '-':
				plateform.disable();
				break;
			case '+':
				plateform.enable();
				break;
			case 'a':
				track.up();
				break;
			case 'd':
				track.down();
				break;
			case '9':
				track.disable();
				break;
			case '0':
				track.enable();
				break;
    }
  }
}
