
 //{----------------------Libraries ---------------------------------------------------

//libraries for temperature sensor
#include <OneWire.h> 
#include <DallasTemperature.h>

//libraries for EC sensor
#include "DFRobot_EC.h"
#include <EEPROM.h>

//library for talking to wifi module over serial without disrupting serial to arduino. This is needed for using Arduino Uno
//#include <SoftwareSerial.h>
//}

//{----------------------------SENSORS & WIFI-------------------------------------------------

//pins for software serial connection, again, only for Arduino Uno
//#define RX 10
//#define TX 11

//pin for temperature sensor
#define ONE_WIRE_BUS 7

//info for pH probe
#define pHPin A0

//Pin for EC probe
#define ECPIN A1

//info for wifi module
String AP = "aquabot";       // SSID Name
String PASS = "roboponics"; // Network Password
String API = "O46TGAFOY3QTK7GK"; //Get this from ThingSpeak.com
String HOST = "api.thingspeak.com";
String PORT = "80";
String Temperature = "field1"; //This can be configured on ThingSpeak.com
String pH = "field2";
String EC = "field3";
//SoftwareSerial esp8266(RX,TX); //Needed for Arduino Uno

//This is used for establishing communication with the ec sensor
DFRobot_EC ec;

// Setup a oneWire instance to communicate with any OneWire devices  
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

//variables for communicating with the wifi module
int countTrueCommand;
int countTimeCommand; 
boolean found = false;
//}

//{-----------------------------STEPPER-----------------------------------------------------------------

//The motor is controlled through these 3 pins
int PUL=11; //define Pulse pin
int DIR=10; //define Direction pin
int ENA=12; //define Enable Pin

//The delayTime is the time (in microseconds) between steps
int delayTime = 800; //between 0 and 32767
//pulseWidth is the time (in microseconds) it waits for the motor to make a step
int pulseWidth = 50; //this should be kept constant
//}

//{------------------------------TIME---------------------------------------------------------------------------------------------------------------
unsigned long timeNow = 0;
unsigned long timeLast = 0;

//Time start Settings:

int startingHour = 10; // set your starting hour here, not below at int hour. This ensures accurate daily correction of time
int seconds = 0;
int minutes = 55;
int hours = startingHour;
int days = 0;

//Accuracy settings

int dailyErrorFast = 0; // set the average number of milliseconds your microcontroller's time is fast on a daily basis
int dailyErrorBehind = 0; // set the average number of milliseconds your microcontroller's time is behind on a daily basis

int correctedToday = 1; // do not change this variable, one means that the time has already been corrected today for the error in your boards crystal. This is true for the first day because you just set the time when you uploaded the sketch.
//}
 
//{------------------------LIGHTS---------------------------------------------------------------------------------------------------
//Time setting for Lights
int hourLightsOn = 7; //this is 24 hour time
int minuteLightsOn = 0;
int hourLightsOff = 1; //this is 24 hour time
int minuteLightsOff = 0;

//Pin Definitions
const int lightRelay = 49;
//}

//{----------------------SETUP-----------------------------------------------------------------------------
void setup() {
	Serial.begin(9600);
	Serial2.begin(115200); //Change Serial2 to ESP8266 for Arduino Uno throughout code

	Serial.println("Connecting to Access Point");
	sendCommand("AT",5,"OK");
	sendCommand("AT+CWMODE=1",5,"OK");
	sendCommand("AT+CWJAP=\""+ AP +"\",\""+ PASS +"\"",20,"OK");
	
	
	pinMode(lightRelay,OUTPUT);
	digitalWrite(lightRelay,LOW); //turn lights on

	Serial.println("Serial Commands");
	Serial.println("    '0' turns lights off");
	Serial.println("    '1' turns lights on");
  
	pinMode (PUL, OUTPUT);
	pinMode (DIR, OUTPUT);
	pinMode (ENA, OUTPUT);

	//this is for adding a button
	pinMode (6, OUTPUT);
	pinMode(5,INPUT_PULLUP);
	digitalWrite(6,LOW);

  pinMode(35,OUTPUT);
  digitalWrite(35,LOW);

	digitalWrite(ENA,HIGH) ; //pull low to enable the motor
  digitalWrite(4,HIGH);
	
	ec.begin();
	sensors.begin(); //this is the one wire bus that can connect more sensors
	
	Serial.println("Serial Commands for Stepper Control:");
	Serial.println("    '+' enables the motor");
	Serial.println("    '-' disables the motor");
	Serial.println("    'w' sets direction as up");
	Serial.println("    's' sets direction as down");
}

//}

//{-------------------LOOP----------------------------------------------------------------
void loop() {
	handleSerial();
	updateTime();
	checkLights();
	handleSerial();
	//detectCollision();
	stepMotor();
	
	/*
	float tempData = getTempData();
	Serial.println("Temperature reading is " + String(tempData) + " degrees Celsius.");
	
	handleSerial();
	
	float pHData = getpHData(pHPin);
	Serial.println("pH reading is " + String(pHData));
	
	handleSerial();
	
	float ECData = getECData(tempData, ECPIN);
	Serial.println("EC reading is " + String(ECData) + " ms/cm");
*/
	handleSerial();
	
	/*
	uploadToThingSpeak(tempData, Temperature,API,HOST);
	uploadToThingSpeak(pHData,pH,API,HOST);
	uploadToThingSpeak(ECData,EC,API,HOST);
	*/
}

//}

//---------------FUNCTION DEFINTIONS--------------------------------------------
void checkLights() {
  if (hours == hourLightsOn && minutes == minuteLightsOn) {
    digitalWrite(lightRelay,LOW); //turn lights on
  }
  
  if (hours == hourLightsOff && minutes == minuteLightsOff) {
    digitalWrite(lightRelay,HIGH); //turn lights off
  }
}

void updateTime(){
  timeNow = millis()/1000; // the number of milliseconds that have passed since boot
  seconds = timeNow - timeLast;//the number of seconds that have passed since the last time 60 seconds was reached.
  
  if (seconds == 60) {
    timeLast = timeNow;
    minutes = minutes + 1;
  }
  
  //if one minute has passed, start counting milliseconds from zero again and add one minute to the clock.
  
  if (minutes == 60){ 
    minutes = 0;
    hours = hours + 1;
  }
  
  // if one hour has passed, start counting minutes from zero and add one hour to the clock
  
  if (hours == 24){
    hours = 0;
    days = days + 1;
    }
    
    //if 24 hours have passed , add one day
  
  if (hours ==(24 - startingHour) && correctedToday == 0){
    delay(dailyErrorFast*1000);
    seconds = seconds + dailyErrorBehind;
    correctedToday = 1;
  }
  
  //every time 24 hours have passed since the initial starting time and it has not been reset this day before, add milliseconds or delay the progran with some milliseconds. 
  //Change these varialbes according to the error of your board. 
  // The only way to find out how far off your boards internal clock is, is by uploading this sketch at exactly the same time as the real time, letting it run for a few days 
  // and then determine how many seconds slow/fast your boards internal clock is on a daily average. (24 hours).
  
  if (hours == 24 - startingHour + 2) { 
    correctedToday = 0;
  }
  
  //let the sketch know that a new day has started for what concerns correction, if this line was not here the arduiono
  // would continue to correct for an entire hour that is 24 - startingHour. 
  
  
  
  //Serial.print("The time is:           ");
  //Serial.print(days);
	//Serial.print(":");
  //Serial.print(hours);
  //Serial.print(":");
  //Serial.print(minutes);
  //Serial.print(":"); 
  //Serial.println(seconds); 
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


//-------------------getTempData()-------------------------------------------------------
float getTempData(){
	//Serial.println("requesting temperature...");
	sensors.requestTemperatures();
	float temp = sensors.getTempCByIndex(0);
	Serial.println("Temperature is " + String(temp) + " degrees Celsius.");
	return temp;
}

//------------------getpHData()---------------------------------------------------------
float getpHData(const int pHPin) {
  return 7.0;
}
//------------------getECData()---------------------------------------------------------
float getECData(float temperature, int EC_PIN) {
	float voltage,ecValue = 668;
	static unsigned long timepoint = millis();
    voltage = analogRead(EC_PIN)/1024.0*5000;   // read the voltage
    ecValue =  ec.readEC(voltage,temperature);  // convert voltage to EC with temperature compensation
    ec.calibration(voltage,temperature);          // calibration process by Serail CMD
	return ecValue;
}

//----------------sendCommand-----------------------------------------------------------
void sendCommand(String command, int maxTime, char readReplay[]) {
	
	//Serial.print(countTrueCommand);
	//Serial.print(". at command => ");
	//Serial.print(command);
	//Serial.print(" ");
	while(countTimeCommand < (maxTime*1))
	{
		Serial2.println(command);//at+cipsend
		if(Serial2.find(readReplay))//ok
		{
			found = true;
			break;
		}

		countTimeCommand++;
	}

	if(found == true)
	{
		//Serial.println("OYI");
		countTrueCommand++;
		countTimeCommand = 0;
	}

	if(found == false)
	{
		//Serial.println("Fail");
		countTrueCommand = 0;
		countTimeCommand = 0;
	}

	found = false;
}

//----------------uploadToThingSpeak-----------------------------------------------------------

bool uploadToThingSpeak(int sensorValue, String field, String API, String HOST) {
	//This (below) will help understand the AT Commands, use a serial monitor to send commands directly to ESP8266
	//https://room-15.github.io/blog/2015/03/26/esp8266-at-command-reference/#AT+CIPSEND
	
	String getData = "GET /update?api_key="+ API +"&"+ field +"="+String(sensorValue);
	sendCommand("AT+CIPMUX=1",5,"OK"); //enables multiple connections (MAX 4)
	sendCommand("AT+CIPSTART=0,\"TCP\",\""+ HOST +"\","+ PORT,15,"OK"); //connects to the ThingSpeak server
	sendCommand("AT+CIPSEND=0," +String(getData.length()+4),4,">"); //sends length of data to be sent
	Serial2.println(getData);delay(1500);countTrueCommand++; //this actually sends the data
	sendCommand("AT+CIPCLOSE=0",5,"OK"); //closes connection
}

//----------------handleSerial-----------------------------------------------------------

void handleSerial() {
  while (Serial.available() > 0) {
    char incomingCharacter = Serial.read();
    switch (incomingCharacter) {
      case '0':
        digitalWrite(lightRelay,HIGH);
        Serial.println("off");
        digitalWrite(13,LOW);
        break;

    case '1':
		digitalWrite(lightRelay,LOW); 
		Serial.println("on");
		digitalWrite(13,HIGH);
		break;
		
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
  case 'c':
    digitalWrite(35,HIGH);
    delay(50);
    digitalWrite(35,LOW);
    break;
    }
  }
}
