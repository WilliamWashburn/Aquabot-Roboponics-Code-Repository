/* Hook Up Scheme
ESP8266 | Arduino
-----------------
     RX | 11
     TX | 10
    GND | GND
    VCC | 3.3V
  CH_PD | 3.3V
 GPIO 0 | None
 GPIO 2 | None

DS18B20 | Arduino
------------------
     VCC | 5v
     GND | GND
    Data | 2

pH probe | Arduino
------------------
      V+ | 5v
       G | GND
       G | GND
      Po | A0
*/

//----------------------Libraries ---------------------------------------------------

//libraries for temperature sensor
#include <OneWire.h> 
#include <DallasTemperature.h>

//library for talking to wifi module over serial without disrupting serial to arduino
#include <SoftwareSerial.h>

//--------------------Pin Definitions-------------------------------------------------
//pins for software serial connection
#define RX 11
#define TX 10

//pin for temperature sensor
#define ONE_WIRE_BUS 2

// Setup a oneWire instance to communicate with any OneWire devices  
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

//------------------Variable Definitions-----------------------------------------------
//info for wifi module
String AP = "peanut";       // SSID Name
String PASS = "williamw"; // Network Password
String API = "O46TGAFOY3QTK7GK"; //Get this from ThingSpeak.com
String HOST = "api.thingspeak.com";
String PORT = "80";
String Temperature = "field1"; //This can be configured on ThingSpeak.com
String pH = "field2";
int countTrueCommand;
int countTimeCommand; 
boolean found = false; 
int valSensor = 1;
SoftwareSerial esp8266(RX,TX); 

//info for pH probe
const int analogInPin = A0;
unsigned long int avgValue;
float b;
int buf[10],temp;

//--------------------Setup()------------------------------------------------------------
void setup() {
	Serial.begin(9600);
	sensors.begin(); //this is the one wire bus that can connect more sensors
	esp8266.begin(115200);
	sendCommand("AT",5,"OK");
	sendCommand("AT+CWMODE=1",5,"OK");
	sendCommand("AT+CWJAP=\""+ AP +"\",\""+ PASS +"\"",20,"OK");
}

//--------------------Loop()-------------------------------------------------------------
void loop() {
	int tempData = getTempData();
	int pHData = getpHData();
	
	uploadToThingSpeak(tempData, Temperature,API,HOST);
	uploadToThingSpeak(pHData,pH,API,HOST);
}

//-------------------getTempData()-------------------------------------------------------
int getTempData(){
  Serial.println("requesting temperature...");
	sensors.requestTemperatures();
	float temp = sensors.getTempCByIndex(0);
  Serial.println("Temperature is " + String(temp) + " degrees Celsius.");
	return temp;
}

//------------------getpHData()---------------------------------------------------------
int getpHData() {
	int pHValue = 0;
  Serial.println("collecting 10 pH samples...");
	for (int i=0; i < 10; i++) {
		buf[i]=analogRead(analogInPin);
		delay(10);
	}

	//this sorts the array
	for(int i=0;i<9;i++){
		for(int j=i+1;j<10;j++){
			if(buf[i]>buf[j]) {
				temp=buf[i];
				buf[i]=buf[j];
				buf[j]=temp;
			}
		}
	}
	avgValue=0;
	//this gets rid of the highest two and lowest two values
	for (int i = 2; i<8;i++){
		avgValue+=buf[i]; //summation of values
		float pHVol=(float)avgValue*5.0/1024/6; //dividing by 6 finds the avgerage value, multiplying by 5/1024 converts the value from 0 to 1024 scale to 0 to 5 scale 
		float pHValue = -5.70 * pHVol + 21.34; //this is the curve the pH probe uses for converting voltage to pH reading
	}
  Serial.println("pH reading is " + String(pHValue));
	return pHValue;
}

void sendCommand(String command, int maxTime, char readReplay[]) {
	Serial.print(countTrueCommand);
	Serial.print(". at command => ");
	Serial.print(command);
	Serial.print(" ");
	while(countTimeCommand < (maxTime*1))
	{
		esp8266.println(command);//at+cipsend
		if(esp8266.find(readReplay))//ok
		{
			found = true;
			break;
		}

		countTimeCommand++;
	}

	if(found == true)
	{
		Serial.println("OYI");
		countTrueCommand++;
		countTimeCommand = 0;
	}

	if(found == false)
	{
		Serial.println("Fail");
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
	esp8266.println(getData);delay(1500);countTrueCommand++; //this actually sends the data
	sendCommand("AT+CIPCLOSE=0",5,"OK"); //closes connection
}
