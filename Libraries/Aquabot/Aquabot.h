/*
  Morse.h - Library for flashing Morse code.
  Created by David A. Mellis, November 2, 2007.
  Released into the public domain.
*/
#ifndef Aquabot_h
#define Aquabot_h

#include "Arduino.h"

//{----------STEPPER---------------------------------
class Stepper
{
  public:
    Stepper(int PUL, int DIR, int ENA, int delayTime, int pulseWidth);
    void stepMotor();
	void disable();
	void enable();
	void up();
	void down();
	
  private:
	int _ENA;
	int _DIR;
	int _PUL;
	
	int _delayTime;
	int _pulseWidth;
};
//}


//{----------LIGHT---------------------------------

class Light
{
	public:
		Light(int lightRelay);
		void lightsOn();
		void lightsOff();
	private:
		int _lightRelay;
};
//}

//{----------TIME---------------------------------

class Time
{
	public:
		Time(
		int startingHour,
		int seconds,
		int minutes,
		int days,
		int dailyErrorFast,
		int dailyErrorBehind,
		int correctedToday,
		int hourLightsOn,
		int minuteLightsOn,
		int hourLightsOff,
		int minuteLightsOff
		);
		
		bool lightStatus; //for keeping track of status of the lights (on or off)
		void updateTime();
		void checkLights();
		
	private:
		//variables for holding time
		int _startingHour;
		int _seconds;
		int _minutes;
		int _hours;
		int _days;
		unsigned long _timeNow;
		unsigned long _timeLast;

		
		//variable for time corrections
		int _dailyErrorFast;
		int _dailyErrorBehind;
		int _correctedToday;
	
		//variables for when lights go on or off
		int _hourLightsOn;
		int _minuteLightsOn;
		int _hourLightsOff;
		int _minuteLightsOff;
		
		
};

//}

//{----------CAMERA---------------------------------

class Camera
{
	public:
		Camera(int);
		void takePicture();
	private:
		int _camTriggerPin;
};
//}

#endif