#include <Aquabot.h>

//for temperature sensor
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 8
// Create a onewire instanace
OneWire oneWire(ONE_WIRE_BUS);
// Declare a DS18B20 Instance and assing the OneWire reference to it.
DallasTemperature sensors(&oneWire);

//--------------------------Variables---------------------------------------------------------------------------
//Time start Settings:

int startingHour = 14; // set your starting hour here, not below at int hour. This ensures accurate daily correction of time
int startingMinutes =30 ;
int day = 0;

//Time Accuracy settings

int dailyErrorFast = 0; // set the average number of milliseconds your microcontroller's time is fast on a daily basis
int dailyErrorBehind = 0; // set the average number of milliseconds your microcontroller's time is behind on a daily basis

int correctedToday = 1; // do not change this variable, one means that the time has already been corrected today for the error in your boards crystal. This is true for the first day because you just set the time when you uploaded the sketch.

//Time setting for Lights
int hourLightsOn = 7; //this is 24 hour time
int minuteLightsOn = 0;
int hourLightsOff = 1; //this is 24 hour time
int minuteLightsOff = 0;

//Wifi variables
String AP = "aquabot";       // SSID Name
String PASS = "roboponics"; // Network Password
String API = "O46TGAFOY3QTK7GK"; //Get this from ThingSpeak.com
String HOST = "api.thingspeak.com";
String PORT = "80";
String Temp = "field1";
String pH = "field2";
String EC = "field3";
//-------------------------Initiation------------------------------------------------------------------------------
//Stepper Motor init
Stepper plateform(11,10,12,800,50); //(PUL,DIR,ENA,delaytime,pulsewidth)
Stepper cart(6,5,7,2000,50);
Stepper camera(3,2,4,10000,50);

//Light init
Light growLights(49);

//Time init
Time time(startingHour,
          startingMinutes,
          day,
          dailyErrorFast,
          dailyErrorBehind,
          correctedToday,
          hourLightsOn,
          minuteLightsOn,
          hourLightsOff,
          minuteLightsOff
          );

Wifi wifi; //if initializer does not have inputs, dont put parentasis because the compiler will think its a function declaration
double pHreading;
double ECreading;
double tempReading;

bool photoshootToday = false; //for keeping track of if there was a photoshoot today
//------------------------Setup--------------------------------------------------------------
void setup() {
  Serial.begin(9600);

  Serial3.begin(9600);//ph probe
  Serial1.begin(9600);//EC probe
  
  Serial3.println("C,0"); //This command switchs the device for continuous reading to single reading
  String response;
  response = Serial3.readStringUntil("\n"); //reads *OK<cr>
  Serial3.println("C,?");
  response = Serial3.readStringUntil("\n"); //reads ?C,0<cr>
  //Serial.print("pH probe response is : ");Serial.println(response);Serial.println();
  
  
  Serial1.println("C,0"); //This command switchs the device for continuous reading to single reading
  //Serial1.println("C,?");
  response = Serial1.readStringUntil("\n");
  //Serial.println(response);
  Serial1.println("O,EC,0");
  Serial1.println("O,TDS,1");

  sensors.begin(); //begins temperature sensor
  
  Serial.println("Starting program..");
  

  digitalWrite(37,OUTPUT);digitalWrite(37,LOW); //camera trigger pin (tells the arduino that is controlling the camera to take a photo)
  pinMode(36,INPUT); //confirmation pin (arduino controlling camera confirms to main arduino (running this script) that a photo was taken)
                            //36 will be low when not confirming and high when confirming
//wifi setup---------------------------------------------
  wifi.connectToAP(AP,PASS);
  wifi.connectToThingSpeak(API,HOST,PORT);
//{-------------------------------------------------------
  printSerialCommands();
  Serial.println("Setup Complete");
}
//}
//---------------------loop-----------------------------------------
void loop() {
  
  //check to see if it is time to take sensor readings
  while (0 == time.minutes && 0 <= time.seconds && time.seconds<= 20){
    Serial.println("Uploading to thingspeak. Please wait...");
    
    pHreading = takepHReading();
    Serial.print("pH reading: ");Serial.println(pHreading);
    Serial.print("Uploading...");wifi.uploadToThingSpeak(pHreading,pH);Serial.println("done.");
    delay(500);
    
    ECreading = takeECReading();
    ECreading = takeECReading(); //this is repeated because for some reason, the first time it is called, it returns 0
    if(ECreading == 0){Serial.println("EC ERROR");}
    else{
      Serial.print("EC reading: ");Serial.println(ECreading);
      Serial.print("Uploading...");wifi.uploadToThingSpeak(ECreading,EC);Serial.println("done.");  
    }
    delay(500);
    tempReading = takeTempReading();
    Serial.print("temp reading: ");Serial.println(tempReading);
    Serial.print("Uploading...");wifi.uploadToThingSpeak(tempReading,Temp);Serial.println("done.");
    delay(500);
    
    time.updateTime(); //keep updating time
  }
  Serial.println("Finished taking sensor readings");

  
  //check to see if it is time to photoshoot
  if ((2 == time.minutes && (0 <= time.seconds && time.seconds <= 2) && time.hours == 24) && photoshootToday ==  false){
    photoshoot();
    photoshootToday = true;
  }

  if (photoshootToday == true && time.hours == 24 && time.minutes == 3) {photoshootToday = false;} //this just keeps photoshoot() from being called multiple times in the time window above

  //check to see if the lights should be on or off
  if (time.seconds == 0 ) {
    if (time.checkLights()) {growLights.lightsOn();}
    else {growLights.lightsOff();}
  }

  //handle serial inputs
  handleSerial();

  //update the time
  time.updateTime();
}


double takeECReading(){
    Serial1.println("R");
    String ECReading = Serial1.readStringUntil("*OK\n");
    double ECReadingD = ECReading.toDouble();
    return ECReadingD;
}

//pH
double takepHReading() {                                 //if the hardware serial port_3 receives a char
    Serial3.println("R");
    String phReading = Serial3.readStringUntil("*OK\n");
    double phReadingD = phReading.toDouble();
    return phReadingD;
}

double takeTempReading() {
  sensors.requestTemperatures();
  return sensors.getTempFByIndex(0);
}



//--------------------handleSerial-------------------------------------------
void handleSerial() {
	while (Serial.available() > 0) {
		char incomingCharacter = Serial.read();
		switch (incomingCharacter) {
      case 'b':
        ECreading = takeECReading();
        Serial.println(ECreading);
      case 'z':
        Serial.println("zero called");
        cart.zero(); plateform.zero(); camera.zero();
        Serial.println("Coorinates zeroed");
        break;
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
        break;
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
        break;
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
        Serial.println("LIGHTS ON");
        growLights.lightsOn();
        break;
      case '0':
        Serial.println("LIGHTS OFF");
        growLights.lightsOff();
        break;
      case 'c':
        takePhoto();
        break;
      case 't':
        time.printTime();
        break;
      case '5':
        photoshoot();
        break;
      case 'x':
        plateform.enable();cart.enable();camera.enable();
        plateform.goToStepCount(20); //go to basically 0
        cart.goToStepCount(0);
        camera.goToStepCount(0);
        break;
        
      

		}
	}
}

void photoshoot(){

  growLights.lightsOn();
  plateform.enable();cart.enable();camera.enable();
  Serial.println("Motors enabled");

  /*
//plateform level 1
  //plateform.goToStepCount(9289);
  cart.goToStepCount(250);
  camera.goToStepCount(237);
  takePhoto();

  cart.goToStepCount(3315);
  camera.goToStepCount(204);
  takePhoto();


  cart.goToStepCount(7973);
  camera.goToStepCount(204);
  takePhoto();

  cart.goToStepCount(11154);
  camera.goToStepCount(174);
  takePhoto();
  
//plateform level 2
  plateform.goToStepCount(41899);
  cart.goToStepCount(11154);
  camera.goToStepCount(-174);
  takePhoto();

  cart.goToStepCount(7973);
  camera.goToStepCount(-204);
  takePhoto();

  cart.goToStepCount(3315);
  camera.goToStepCount(-204);
  takePhoto();

  cart.goToStepCount(250);
  camera.goToStepCount(-237);
  takePhoto();

  
//plateform level 3
  plateform.goToStepCount(71543);
  cart.goToStepCount(250);
  camera.goToStepCount(237);
  takePhoto();

  cart.goToStepCount(3315);
  camera.goToStepCount(204);
  takePhoto();

  
  camera.goToStepCount(200);
  cart.goToStepCount(7973);
  camera.goToStepCount(204);
  takePhoto();

  cart.goToStepCount(11154);
  camera.goToStepCount(174);
  takePhoto();
*/

  //plateform level 4
  plateform.goToStepCount(102505);
  cart.goToStepCount(11154);
  camera.goToStepCount(-174);
  takePhoto();

  cart.goToStepCount(7973);
  camera.goToStepCount(-204);
  takePhoto();

  cart.goToStepCount(3315);
  camera.goToStepCount(-204);
  takePhoto();

  cart.goToStepCount(250);
  camera.goToStepCount(-237);
  takePhoto();
  
/*
  //plateform level 5
  plateform.goToStepCount(132342);
  cart.goToStepCount(250);
  camera.goToStepCount(237);
  takePhoto();

  cart.goToStepCount(3315);
  camera.goToStepCount(204);
  takePhoto();

  
  cart.goToStepCount(7973);
  camera.goToStepCount(204);
  takePhoto();

  cart.goToStepCount(11154);
  camera.goToStepCount(174);
  takePhoto();

  */

  Serial.println("Photoshoot complete");

  //plateform.goToStepCount(0);
  //cart.goToStepCount(0);
  //camera.goToStepCount(0);

}
//-------------------------takePhoto----------------------------------------------
void takePhoto(){
  digitalWrite(37,HIGH);//trigger pin
  delay(500);
  digitalWrite(37,LOW);
  Serial.print("capturing photo");
  int count = 0;
  while(digitalRead(36)==LOW){ //confirmation pin
    Serial.print(".");
    delay(200);
    count++;
    if (count == 100){
      digitalWrite(37,HIGH);//trigger pin
      delay(1000);
      digitalWrite(37,LOW);
      count = 0;
    }
  }
  Serial.println("done");
}

void printSerialCommands() {
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
