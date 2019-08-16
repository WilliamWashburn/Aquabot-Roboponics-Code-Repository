//Written by Will Washburn

/* Hook Up Scheme
ESP8266 | Arduino
-----------------
     RX | 19
     TX | 18
    GND | GND
    VCC | 3.3V
  CH_PD | 3.3V
 GPIO 0 | None
 GPIO 2 | None

DS18B20  | Arduino
------------------
     VCC | 5v
     GND | GND
    Data | 22

pH probe | Arduino
------------------
      V+ | 5v
       G | GND
       G | GND
      Po | A0
	  
	EC probe | Arduino
------------------
	V+ (red) | 5v
	G (black)| GND
Data (blue)  | A7

*/

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
//{--------------------Pin Definitions-------------------------------------------------

//pins for software serial connection, again, only for Arduino Uno
//#define RX 10
//#define TX 11

//pin for temperature sensor
#define ONE_WIRE_BUS 7

//info for pH probe
#define pHPin A0

//Pin for EC probe
#define ECPIN A1
//}

//{------------------Variable Definitions-----------------------------------------------
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

//variable for stepper motor if attached, included to disable it
int ENA = 11;

//}

//--------------------Setup()------------------------------------------------------------
void setup() {
	Serial.println("Beginning Setup");
  
	Serial.begin(9600);
	Serial2.begin(115200); //Change Serial2 to ESP8266 for Arduino Uno throughout code
	
	ec.begin();
	sensors.begin(); //this is the one wire bus that can connect more sensors

	Serial.println("Connecting to Access Point");
	sendCommand("AT",5,"OK");
	sendCommand("AT+CWMODE=1",5,"OK");
	sendCommand("AT+CWJAP=\""+ AP +"\",\""+ PASS +"\"",20,"OK");

  pinMode(ENA,OUTPUT);
  digitalWrite(ENA, HIGH);
}

//--------------------Loop()-------------------------------------------------------------
void loop() {
	float tempData = getTempData();
	Serial.println("Temperature reading is " + String(tempData) + " degrees Celsius.");

	float pHData = getpHData(pHPin);
	Serial.println("pH reading is " + String(pHData));

	float ECData = getECData(tempData, ECPIN);
	Serial.println("EC reading is " + String(ECData) + " ms/cm");

	uploadToThingSpeak(tempData, Temperature,API,HOST);
	uploadToThingSpeak(pHData,pH,API,HOST);
	uploadToThingSpeak(ECData,EC,API,HOST);
}

//-------------------getTempData()-------------------------------------------------------
float getTempData(){
	//Serial.println("requesting temperature...");
	sensors.requestTemperatures();
	float temp = sensors.getTempCByIndex(0);
	//Serial.println("Temperature is " + String(temp) + " degrees Celsius.");
	return temp;
}

//------------------getpHData()---------------------------------------------------------
float getpHData(const int pHPin) {
unsigned long int avgValue;  //Store the average value of the sensor feedback
float b;
int buf[10], temp;
  for (int i = 0; i < 10; i++) //Get 10 sample value from the sensor for smooth the value
  {
    buf[i] = analogRead(pHPin);
    delay(10);
  }
  for (int i = 0; i < 9; i++) //sort the analog from small to large
  {
    for (int j = i + 1; j < 10; j++)
    {
      if (buf[i] > buf[j])
      {
        temp = buf[i];
        buf[i] = buf[j];
        buf[j] = temp;
      }
    }
  }
  avgValue = 0;
  for (int i = 2; i < 8; i++) {             //take the average value of 6 center sample
    avgValue += buf[i];
  }
  float phValue = (float)avgValue * 5.0 / 1024 / 6; //convert the analog into millivolt
  Serial.println("    Voltage reading: " + String(phValue));
  phValue = -40.05 * phValue + 107.39;                //convert the millivolt into pH value
  Serial.print("    pH:");
  Serial.print(phValue, 2);
  Serial.println(" ");
  Serial.println();
  delay(800);
  return pHValue;
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