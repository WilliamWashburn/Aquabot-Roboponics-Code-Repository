
#include "Arduino.h"
#include "Aquabot.h"

//{ Stepper Class
Stepper::Stepper(int PUL, int DIR, int ENA, int delayTime, int pulseWidth)
{
	pinMode(PUL,OUTPUT);
	pinMode (DIR, OUTPUT);
	pinMode (ENA, OUTPUT);
	
	digitalWrite(ENA,HIGH); //pull low to enable the motor
	
	_PUL = PUL;
	_DIR = DIR;
	_ENA = ENA;
	_pulseWidth = pulseWidth;
	_delayTime = delayTime;
	
	bool _held = false;
	bool _moving = false;
	
	long _stepCount = 0;
}

void Stepper::stepMotor()
{
	if(digitalRead(_ENA)==LOW && _held == false) {
		digitalWrite(_PUL,HIGH);
		delayMicroseconds(_pulseWidth);
		digitalWrite(_PUL,LOW);
		delayMicroseconds(_delayTime);
		//do not put any serial.prints here, it will cause problems moving the motor
		//probably any delay between stepMotor being called will result in choppy motion/vibration
		
		if (digitalRead(_DIR) == HIGH) {
			_stepCount = _stepCount + 1;
		}
		
		if (digitalRead(_DIR) == LOW) {
			_stepCount = _stepCount - 1;
		}
	}
}

void Stepper::enable()
{
	digitalWrite(_ENA,LOW); //pull low to enable the motor
}

void Stepper::disable()
{
	digitalWrite(_ENA,HIGH); //pull low to enable the motor
}

void Stepper::up()
{
	digitalWrite(_DIR,HIGH);
}

void Stepper::down()
{
	digitalWrite(_DIR,LOW);
}

bool Stepper::held()
{
	return _held;
}

void Stepper::holdOn()
{
	_held = true;
	//Serial.println("holdOn() was called _held = true");
	Serial.println(_stepCount);
}

void Stepper::holdOff()
{
	_held = false;
	//Serial.println("holdOff() was called, _held =false");
}

void Stepper::goToStepCount(long step)
{
	if (step > _stepCount) {
		up();
		holdOff();
		while ( step > _stepCount) {
			stepMotor();
		}
		holdOn();
	}
	
	else if (step < _stepCount) {
		down();
		holdOff();
		while ( step < _stepCount) {
			stepMotor();
		}
		holdOn();
	}
	
	else {
	}
	delay(500);
}

void Stepper::zero()
{
	_stepCount = 0;
}

//}

//{ Light Class
Light::Light(int lightRelay)
{
	pinMode(lightRelay,OUTPUT);
	digitalWrite(lightRelay,LOW); //turns on lights
	
	_lightRelay = lightRelay;
}

void Light::lightsOn()
{
		digitalWrite(_lightRelay,LOW); //turns on lights
}

void Light::lightsOff()
{
		digitalWrite(_lightRelay,HIGH); //turns on lights
}	
//}

//{ Time Class
Time::Time(int startingHour,
		int seconds,
		int minutes,
		int days,
		int dailyErrorFast,
		int dailyErrorBehind,
		int correctedToday,
		int hourLightsOn,
		int minuteLightsOn,
		int hourLightsOff,
		int minuteLightsOff)
{
	unsigned long _timeNow = 0;
	unsigned long _timeLast = 0;
	
	int _dailyErrorFast = dailyErrorFast;
	int _dailyErrorBehind = dailyErrorBehind;
	int _correctedToday = correctedToday;
	int _hourLightsOn = hourLightsOn;
	int _minuteLightsOn = minuteLightsOn;
	int _hourLightsOff = hourLightsOff;
	int _minuteLightsOff = minuteLightsOff;
}

void Time::updateTime(){
	_timeNow = millis()/1000; // the number of milliseconds that have passed since boot
	seconds = _timeNow - _timeLast;//the number of seconds that have passed since the last time 60 seconds was reached.
  
	if (seconds == 60) {
		_timeLast = _timeNow;
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
  
  if (hours ==(24 - startingHour) && _correctedToday == 0){
    delay(_dailyErrorFast*1000);
    seconds = seconds + _dailyErrorBehind;
    _correctedToday = 1;
  }
  
  //every time 24 hours have passed since the initial starting time and it has not been reset this day before, add milliseconds or delay the progran with some milliseconds. 
  //Change these varialbes according to the error of your board. 
  // The only way to find out how far off your boards internal clock is, is by uploading this sketch at exactly the same time as the real time, letting it run for a few days 
  // and then determine how many seconds slow/fast your boards internal clock is on a daily average. (24 hours).
  
  if (hours == 24 - startingHour + 2) { 
    _correctedToday = 0;
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

void Time::checkLights(){
	if (hours == _hourLightsOn && _minuteLightsOn <= minutes <= _minuteLightsOn + 5) {
		lightStatus = true;
	}
  
	if (hours == _hourLightsOff && _minuteLightsOff <= minutes <= _minuteLightsOff + 5) {
		lightStatus = false;
	}
}

void Time::printTime(){
	Serial.print("The time is:           ");
	Serial.print(days);
	Serial.print(":");
	Serial.print(hours);
	Serial.print(":");
	Serial.print(minutes);
	Serial.print(":"); 
	Serial.println(seconds); 
}

//}

//{ Camera Class

Camera::Camera(int camTriggerPin)
{
	pinMode(camTriggerPin, OUTPUT);
	digitalWrite(camTriggerPin, LOW);
	
	int _camTriggerPin = camTriggerPin;
}

void Camera::takePicture()
{
	digitalWrite(_camTriggerPin, HIGH);
	delay(10);
	digitalWrite(_camTriggerPin, LOW);
}


//}

//{ Wifi Class
Wifi::Wifi()
{
	Serial2.begin(115200);
	
	int _countTrueCommand;
	int _countTimeCommand;
	bool _found = false;
	
	
}

void Wifi::connectToAP(String AP,String PASS){
	sendCommand("AT",5,"OK");
	sendCommand("AT+CWMODE=1",5,"OK");
	sendCommand("AT+CWJAP=\""+ AP +"\",\""+ PASS +"\"",20,"OK");

	_AP = AP;
	_PASS = PASS;
	Serial.println("wifi setup complete"); Serial.println();
}

void Wifi::connectToThingSpeak(String API, String HOST, String PORT){
	_API = API;
	_HOST = HOST;
	_PORT = PORT;
	
	Serial.println(_API);
	Serial.println(_HOST);
	Serial.println(_PORT);


}

void Wifi::uploadToThingSpeak(double sensorValue, String field)
{
	//This (below) will help understand the AT Commands, use a serial monitor to send commands directly to ESP8266
	//https://room-15.github.io/blog/2015/03/26/esp8266-at-command-reference/#AT+CIPSEND
	String getData = "GET /update?api_key="+ _API +"&"+ field +"="+String(sensorValue);
	sendCommand("AT+CIPMUX=1",5,"OK"); //enables multiple connections (MAX 4)
	sendCommand("AT+CIPSTART=0,\"TCP\",\""+ _HOST +"\","+ _PORT,20,"OK"); //connects to the ThingSpeak server
	sendCommand("AT+CIPSEND=0," +String(getData.length()+4),4,">"); //sends length of data to be sent
	Serial2.println(getData);delay(1500);_countTrueCommand++; //this actually sends the data
	sendCommand("AT+CIPCLOSE=0",5,"OK"); //closes connection
}

void Wifi::sendCommand(String command, int maxTime, char readReplay[])
{
	
	Serial.print(_countTrueCommand);
	Serial.print(". at command => ");
	Serial.print(command);
	Serial.print(" ");
	while(_countTimeCommand < (maxTime*1))
	{
		Serial2.println(command);//at+cipsend
		if(Serial2.find(readReplay))//ok
		{
			_found = true;
			break;
		}

		_countTimeCommand++;
	}

	if(_found == true)
	{
		Serial.println("OYI");
		_countTrueCommand++;
		_countTimeCommand = 0;
	}

	if(_found == false)
	{
		Serial.println("Fail");
		_countTrueCommand = 0;
		_countTimeCommand = 0;
	}

	_found = false;
}

//}


//{ Sensor Class

//}


