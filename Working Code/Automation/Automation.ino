#include <Aquabot.h>

//Time start Settings:

int startingHour = 15; // set your starting hour here, not below at int hour. This ensures accurate daily correction of time
int seconds = 0;
int minutes = 3;
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
Stepper camera(3,2,4,10000,50);

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

  digitalWrite(35,OUTPUT);digitalWrite(35,LOW);
  
  Serial.println("Serial Commands");
  Serial.println("    'h' enables all motors");
  Serial.println("    'g' disables all motors");Serial.println();
  
  Serial.println("    '+' plateform unhold");
  Serial.println("    '-' plateform hold");
  Serial.println("    'w' plateform up");
  Serial.println("    's' plateform down");Serial.println();
  
  Serial.println("    'i' cart unhold");
  Serial.println("    'k' cart hold");
  Serial.println("    'd' cart away from door");
  Serial.println("    'a' cart toward the door");Serial.println();
  
  Serial.println("    'u' camera unhold");
  Serial.println("    'j' camera hold");
  Serial.println("    'e' camera clockwise");
  Serial.println("    'q' camera counter-clockwise");Serial.println();
  
  Serial.println("    '0' turns lights off");
  Serial.println("    '1' turns lights on");Serial.println();

  Serial.println("    'c' captures a photo'");
}

void loop() {
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
				plateform.holdOn();
				break;
			case '+':
				plateform.holdOff();
        while(plateform.held() == false) {
          plateform.stepMotor();
          handleSerial();
        }
				break;
      case 'h':
        plateform.enable();cart.enable();camera.enable();
        Serial.println("Motors enabled");
        break;
      case 'g':
        plateform.disable();cart.disable();camera.disable();
        Serial.println("Motors disabled");
        break;
      case 'i':
        cart.holdOff();
        while(cart.held() == false) {
          cart.stepMotor();
          handleSerial();
        }
      case 'k':
        cart.holdOn();
        break;
      case 'd':
        cart.up();
        break;
      case 'a':
        cart.down();
        break;
      case 'u':
        camera.holdOff();
        while(camera.held() == false) {
          camera.stepMotor();
          handleSerial();
        }
      case 'j':
        camera.holdOn();
        break;
      case 'e':
        camera.down();
        break;
      case 'q':
        camera.up();
        break;
      case '1':
        growLights.lightsOn();
        break;
      case '0':
        growLights.lightsOff();
        break;
      case 'c':
        digitalWrite(35,HIGH);//trigger pin
        delay(50);
        digitalWrite(35,LOW);
        while(digitalRead(34)==LOW){ //confirmation pin
          Serial.print("capturing photo...    ");
          delay(500);
        }
        Serial.println("done");
        break;
      case 't':
        Serial.print("The time is currently: "); Serial.print(hours);
        time.printTime();
        break;
      case '5':
        cart.goToStepCount(0);
        camera.goToStepCount(-185);
        plateform.goToStepCount(71543);
        takePhoto();

        cart.goToStepCount(3887);
        camera.goToStepCount(-200);
        plateform.goToStepCount(71543);
        takePhoto();

        cart.goToStepCount(3887*2);
        camera.goToStepCount(-200);
        plateform.goToStepCount(71543);
        takePhoto();

        cart.goToStepCount(3887*3);
        camera.goToStepCount(-215);
        plateform.goToStepCount(71543);
        takePhoto();



        cart.goToStepCount(3887*3);
        camera.goToStepCount(215);
        plateform.goToStepCount(102505);
        takePhoto();

        cart.goToStepCount(3887*2);
        camera.goToStepCount(200);
        plateform.goToStepCount(102505);
        takePhoto();

        cart.goToStepCount(3887);
        camera.goToStepCount(200);
        plateform.goToStepCount(102505);
        takePhoto();

        cart.goToStepCount(0);
        camera.goToStepCount(185);
        plateform.goToStepCount(102505);
        takePhoto();

        break;
      case 'z':
        cart.zero(); plateform.zero(); camera.zero();
        Serial.println("Coorinates zeroed");
		}
	}
}

void takePhoto(){
  digitalWrite(35,HIGH);//trigger pin
  delay(50);
  digitalWrite(35,LOW);
  Serial.print("capturing photo...    ");
  while(digitalRead(34)==LOW){ //confirmation pin
    Serial.print(".");
    delay(500);
  }
  Serial.println("done");
}
