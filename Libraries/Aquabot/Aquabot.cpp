
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
}

void Stepper::holdOff()
{
	_held = false;
	//Serial.println("holdOff() was called, _held =false");
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

//}

//{ Sensor Class

//}


