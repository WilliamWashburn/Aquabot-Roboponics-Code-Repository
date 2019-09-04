#include <Aquabot.h>

//Time start Settings:

int startingHour = 16; // set your starting hour here, not below at int hour. This ensures accurate daily correction of time
int seconds = 0;
int minutes = 33;
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
int hourLightsOff = 1; //this is 24 hour time
int minuteLightsOff = 0;


Stepper plateform(11,10,12,800,50); //(PUL,DIR,ENA,delaytime,pulsewidth)
Stepper cart(6,5,7,2000,50);
Stepper camera(3,2,4,5000,50);

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

  digitalWrite(35,LOW);

  Serial.println("Serial Commands");
  Serial.println("    '+' plateform enable");
  Serial.println("    '-' plateform disable");
  Serial.println("    'w' plateform up");
  Serial.println("    's' plateform down");Serial.println();
  
  Serial.println("    'i' cart enable");
  Serial.println("    'k' cart disable");
  Serial.println("    'd' cart away from door");
  Serial.println("    'a' cart toward the door");Serial.println();
  
  Serial.println("    'u' camera enable");
  Serial.println("    'j' camera disable");
  Serial.println("    'm' camera hold on");
  Serial.println("    'n' camera hold off");
  Serial.println("    'e' camera clockwise");
  Serial.println("    'q' camera counter-clockwise");Serial.println();
  
  Serial.println("    '0' turns lights off");
  Serial.println("    '1' turns lights on");Serial.println();

  Serial.println("    'c' captures a photo'");
}

void loop() {
	plateform.stepMotor();
  cart.stepMotor();
  camera.stepMotor();
  //camera.hold(); //only is held if camera.holdOn() is called
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
      case 'd':
        cart.up();
        break;
      case 'a':
        cart.down();
        break;
      case 'u':
        camera.enable();
        break;
      case 'j':
        camera.disable();
        break;
      case 'e':
        camera.up();
        break;
      case 'q':
        camera.down();
        break;
      case 'm':
        camera.holdOn();
        break;
      case 'n':
        camera.holdOff();
      case '1':
        growLights.lightsOn();
        break;
      case '0':
        growLights.lightsOff();
        break;
      case 'c':
        digitalWrite(35,HIGH);
        delay(50);
        digitalWrite(35,LOW);
        break;
      case 't':
        time.printTime();
        break;
		}
	}
}
