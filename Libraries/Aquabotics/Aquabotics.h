/* 
	Aquabotics.h - Library created for automating a hydroponic system.
	Created by Will Washburn, August 2, 2019.
*/

#ifndef Aquabotics_h
#define	Aquabotics_h

#include "Arduino.h"

class Sensors{
	public:
	private:
}

class Light{
	public:
		LightSetup(int startingHour, int seconds, int minutes, int days, int dailyErrorFast, int dailyErrorBehind, int hourLightsOn, int minuteLightsOn, int hourLightsOff, int minuteLightsOff, const int lightRelay);
		void checkLights();
	private:
}

class Time{
	public:
	private:
}

class Stepper{
	public:
		Stepper(int ENA, int DIR, int PUL, int delayTime, int pulseWidth, int BUMP1, int BUMP2);
		void stepMotor();
		void detectCollision();
		void enable();
		void disable();
		void up();
		void down();
	private:
		int _ENA;
		int _DIR;
		int _PUL;
		int _delayTime;
		int _pulseWidth;
		int _BUMP1;
		int _BUMP2;
}


#endif
