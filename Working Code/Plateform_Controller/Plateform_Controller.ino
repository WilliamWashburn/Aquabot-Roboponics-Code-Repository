/* Sketch for controlling the plateform through serial commands
 * Created by Will Washburn, August 2nd, 2019.
 */

/*

//This program is used to test out various speeds for a stepper motor
//The motor will spin back and forth at an initial speed
//To change the speed, open a serial monitor and send the arduino a new delayTime
//The delayTime can range from 0 to 32767, the shorter the delayTime, the faster the motor moves
//To slow or speed up the motor further, increase or decrease the Micro Step's on the stepper motor driver
//IMPORTANT NOTE ABOUT STEPPER MOTOR DRIVER: For the switches on the side that control the microsteps and current, down is on and up is off

     Arduino | TB6600 (driver)
---------------------------------
           7 | ENA+
         GND | ENA-
           6 | DIR+
         GND | DIR-
           5 | PUL+
         GND | PUL-
*though any pin can be selected for ENA+, DIR+, and PUL+
       
The pins for ENA+,DIR+,PUL+ can be any pin (just change the code)
Pull ENA+ to LOW to enable motor, pull to HIGH to disable the motor (freely rotates)

     Stepper | TB6600 (driver)
-----------------------------------
       Black | B-
       Green | B+
        Blue | A-
         Red | A+
     
The colors do not really matter, just find the pairs by checking for low resistance between
When you find the two pairs, either can be connected to A or B and either of the sides of the pairs can be connected to + or -

Power Supply | TB6600 (driver)
---------------------------------
          +V | VCC
         COM | GND

*/

//The motor is controlled through these 3 pins
int PUL=10; //define Pulse pin
int DIR=9; //define Direction pin
int ENA=11; //define Enable Pin

//The delayTime is the time (in microseconds) between steps
int delayTime = 800; //between 0 and 32767
//pulseWidth is the time (in microseconds) it waits for the motor to make a step
int pulseWidth = 50; //this should be kept constant

void setup() {
	Serial.begin(9600);
	pinMode (PUL, OUTPUT);
	pinMode (DIR, OUTPUT);
	pinMode (ENA, OUTPUT);

	//this is for adding a button
	pinMode (6, OUTPUT);
	pinMode(5,INPUT_PULLUP);
	digitalWrite(6,LOW);

	digitalWrite(ENA,HIGH); //pull low to enable the motor

	Serial.println("Serial Commands for Stepper Control:");
	Serial.println("    '+' enables the motor");
	Serial.println("    '-' disables the motor");
	Serial.println("    'w' sets direction as up");
	Serial.println("    's' sets direction as down");
}

void loop() {
	handleSerial();
	detectCollision();
	stepMotor();
}

void stepMotor(){
	digitalWrite(PUL,HIGH);
	delayMicroseconds(pulseWidth);
	digitalWrite(PUL,LOW);
	delayMicroseconds(delayTime);
}

void detectCollision() {
	if (digitalRead(5) == LOW) {
		Serial.println("object detected!");
		digitalWrite(DIR, !digitalRead(DIR));
		while (digitalRead(5) == LOW){
			stepMotor();
		}
		Serial.println("object cleared!");
		Serial.println();
	}
}

void handleSerial() {
	while (Serial.available() > 0) {
		char incomingCharacter = Serial.read();
		switch (incomingCharacter) {
			case 'w':
				digitalWrite(DIR,HIGH); //up
				break;
			case 's':
				digitalWrite(DIR,LOW); //down
				break;
			case '-':
				digitalWrite(ENA,HIGH); //turn off
				break;
			case '+':
				digitalWrite(ENA,LOW); //turn on
				break;
			case '2':
				delayTime =200;
				break;
			case '3':
				delayTime =300;
				break;
			case '4':
				delayTime =400;
				break;
			case '5':
				delayTime =500;
				break;
			case '6':
				delayTime =600;
				break;
			case '7':
				delayTime = 700;
				break;
			case '8':
				delayTime = 800;
				break;
		}
	}
}
