//Written by Will Washburn
//Time keeping adapted from Ruben Marc Speybrouck https://www.instructables.com/id/TESTED-Timekeeping-on-ESP8266-Arduino-Uno-WITHOUT-/


unsigned long timeNow = 0;
unsigned long timeLast = 0;

//Time start Settings:

int startingHour = 11; // set your starting hour here, not below at int hour. This ensures accurate daily correction of time
int seconds = 0;
int minutes = 24;
int hours = startingHour;
int days = 0;

//Accuracy settings

int dailyErrorFast = 0; // set the average number of milliseconds your microcontroller's time is fast on a daily basis
int dailyErrorBehind = 0; // set the average number of milliseconds your microcontroller's time is behind on a daily basis

int correctedToday = 1; // do not change this variable, one means that the time has already been corrected today for the error in your boards crystal. This is true for the first day because you just set the time when you uploaded the sketch.  
 
//Time setting for Lights
int hourLightsOn = 7; //this is 24 hour time
int minuteLightsOn = 0;
int hourLightsOff = 19; //this is 24 hour time
int minuteLightsOff = 0;

//Pin Definitions
const int lightRelay = 49;

void setup() {
  Serial.begin(9600);
  pinMode(lightRelay,OUTPUT);
  digitalWrite(lightRelay,LOW); //turn lights on

  Serial.println("Serial Commands");
  Serial.println("    '0' turns lights off");
  Serial.println("    '1' turns lights on");
}

void loop() {
  handleSerial();
  updateTime();
  checkLights();
}

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
  
  
  
  Serial.print("The time is:           ");
  Serial.print(days);
  Serial.print(":");
  Serial.print(hours);
  Serial.print(":");
  Serial.print(minutes);
  Serial.print(":"); 
  Serial.println(seconds); 
}

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
    }
  }
}
