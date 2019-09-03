#include <Aquabot.h>

//Time start Settings:

int startingHour = 11; // set your starting hour here, not below at int hour. This ensures accurate daily correction of time
int seconds = 0;
int minutes = 24;
int hours = startingHour;
int days = 0;

//Accuracy settings

int dailyErrorFast = 0; // set the average number of milliseconds your microcontroller's time is fast on a daily basis
int dailyErrorBehind = 0; // set the average number of milliseconds your microcontroller's time is behind on a daily basis

int correctedToday = 1; // do not change this variable, one means that the time has already been corrected today for the error in your boards crystal. This is true for the first day because you just set the time when you uploaded the sketch.
//}

//Time setting for Lights
int hourLightsOn = 7; //this is 24 hour time
int minuteLightsOn = 0;
int hourLightsOff = 19; //this is 24 hour time
int minuteLightsOff = 0;


Stepper plateform(11,10,12,1000,50);
Stepper cart(6,5,7,800,50);
Stepper camera(3,2,4,800,50);

Light growLights(49);

Time time(startingHour,
          seconds,
          minutes,
          days,
          dailyErrorFast,
          dailyErrorBehind,
          correctedToday,
          hourLightsOn,
          minuteLightsOn,
          hourLightsOff,
          minuteLightsOff);

void setup() {
  Serial.begin(9600);
  Serial.println("Starting program..");
}

void loop() {
	plateform.stepMotor();
  cart.stepMotor();
  camera.stepMotor();
	handleSerial();

  
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
      case 'i':
        cart.enable();
        break;
      case 'k':
        cart.disable();
        break;
      case 'a':
        cart.up();
        break;
      case 'd':
        cart.down();
        break;
      case 'u':
        camera.enable();
        break;
      case 'j':
        camera.disable();
        break;
      case 'q':
        camera.up();
        break;
      case 'e':
        camera.down();
        break;
      case '1':
        growLights.lightsOn();
        break;
      case '0':
        growLights.lightsOff();
        break;
		}
	}
}
