/*
  Morse.cpp - Library for flashing Morse code.
  Created by David A. Mellis, November 2, 2007.
  Released into the public domain.
*/

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
}

void Stepper::stepMotor()
{
	if(digitalRead(_ENA)==LOW) {
		digitalWrite(_PUL,HIGH);
		delayMicroseconds(_pulseWidth);
		digitalWrite(_PUL,LOW);
		delayMicroseconds(_delayTime);
	}
	
}

void Stepper::enable()
{
	digitalWrite(_ENA,LOW);
}

void Stepper::disable()
{
	digitalWrite(_ENA,HIGH);
}

void Stepper::up()
{
	digitalWrite(_DIR,HIGH);
}

void Stepper::down()
{
	digitalWrite(_DIR,LOW);
}

void Stepper::hold()
{
	if(_held = true){
		digitalWrite(_ENA,HIGH);
		digitalWrite(_DIR,HIGH);
		delay(10);
		digitalWrite(_DIR,LOW);
		delay(10);
	}
}

void Stepper::holdOn()
{
	_held = true;
}

void Stepper::holdOff()
{
	_held = false;
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
	
	int _hours = startingHour;
	int _startingHour = startingHour;
	int _seconds = seconds;
	int _minutes = minutes;
	int _days = days;
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
	_seconds = _timeNow - _timeLast;//the number of seconds that have passed since the last time 60 seconds was reached.
  
	if (_seconds == 60) {
		_timeLast = _timeNow;
		_minutes = _minutes + 1;
	}
  
	//if one minute has passed, start counting milliseconds from zero again and add one minute to the clock.

	if (_minutes == 60){ 
		_minutes = 0;
		_hours = _hours + 1;
	}
  
  // if one hour has passed, start counting minutes from zero and add one hour to the clock
  
	if (_hours == 24){
		_hours = 0;
		_days = _days + 1;
    }
    
    //if 24 hours have passed , add one day
  
  if (_hours ==(24 - _startingHour) && _correctedToday == 0){
    delay(_dailyErrorFast*1000);
    _seconds = _seconds + _dailyErrorBehind;
    _correctedToday = 1;
  }
  
  //every time 24 hours have passed since the initial starting time and it has not been reset this day before, add milliseconds or delay the progran with some milliseconds. 
  //Change these varialbes according to the error of your board. 
  // The only way to find out how far off your boards internal clock is, is by uploading this sketch at exactly the same time as the real time, letting it run for a few days 
  // and then determine how many seconds slow/fast your boards internal clock is on a daily average. (24 hours).
  
  if (_hours == 24 - _startingHour + 2) { 
    _correctedToday = 0;
  }
  
  //let the sketch know that a new day has started for what concerns correction, if this line was not here the arduiono
  // would continue to correct for an entire hour that is 24 - startingHour. 
  
  
  
  //Serial.print("The time is:           ");
  //Serial.print(_days);
	//Serial.print(":");
  //Serial.print(_hours);
  //Serial.print(":");
  //Serial.print(_minutes);
  //Serial.print(":"); 
  //Serial.println(_seconds); 
}

void Time::checkLights(){
	if (_hours == _hourLightsOn && _minuteLightsOn <= _minutes <= _minuteLightsOn + 5) {
		lightStatus = true;
	}
  
	if (_hours == _hourLightsOff && _minuteLightsOff <= _minutes <= _minuteLightsOff + 5) {
		lightStatus = false;
	}
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

//}

//{ Sensor Class

//}


