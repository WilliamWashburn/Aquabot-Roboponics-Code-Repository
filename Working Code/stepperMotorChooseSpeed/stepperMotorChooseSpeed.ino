/*

//This program is used to test out various speeds for a stepper motor
//The motor will spin back and forth at an initial speed
//To change the speed, open a serial monitor and send the arduino a new delayTime
//The delayTime can range from 0 to 32767, the shorter the delayTime, the faster the motor moves
//To slow or speed up the motor further, increase or decrease the Micro Step's on the stepper motor driver
//IMPORTANT NOTE ABOUT STEPPER MOTOR DRIVER: For the switches on the side that control the microsteps and current, down is on and up is off

     Arduino | TB6600 (driver)
---------------------------------
           7 | ENA+
         GND | ENA-
           6 | DIR+
         GND | DIR-
           5 | PUL+
         GND | PUL-
       
The pins for ENA+,DIR+,PUL+ can be any pin (just change the code)
Pull ENA+ to LOW to enable motor, pull to HIGH to disable the motor (freely rotates)

     Stepper | TB6600 (driver)
-----------------------------------
       Black | B-
       Green | B+
        Blue | A-
         Red | A+
     
The colors do not really matter, just find the pairs by checking for low resistance between
When you find the two pairs, either can be connected to A or B and either of the sides of the pairs can be connected to + or -

Power Supply | TB6600 (driver)
---------------------------------
          +V | VCC
         COM | GND

*/

//The motor is controlled through these 3 pins
int PUL=5; //define Pulse pin
int DIR=6; //define Direction pin
int ENA=7; //define Enable Pin

//The delayTime is the time (in microseconds) between steps
int delayTime = 500; //between 0 and 32767
//pulseWidth is the time (in microseconds) it waits for the motor to make a step
int pulseWidth = 50; //this should be kept constant

void setup() {
  Serial.begin(9600);
  pinMode (PUL, OUTPUT);
  pinMode (DIR, OUTPUT);
  pinMode (ENA, OUTPUT);

  digitalWrite(ENA,LOW);

}

void loop() {
  //checks to see if data is available on the serial buffer, if it is, it sets that to the delayTime
  if (Serial.available() > 0) {
      delayTime = receiveSerialData().toInt();
  }
  Serial.println("The delay time is: " + String(delayTime));
  
  //Starts moving forward
  Serial.println("Forward");
  for (int i=0; i<6400; i++)
  {
    digitalWrite(DIR,LOW);
    digitalWrite(PUL,HIGH);
    delayMicroseconds(pulseWidth);
    digitalWrite(PUL,LOW);
    delayMicroseconds(delayTime);
  }

  //Starts moving backward
  Serial.println("Backward");
  for (int i=0; i<6400; i++)
  {
    digitalWrite(DIR,HIGH);
    digitalWrite(PUL,HIGH);
    delayMicroseconds(pulseWidth);
    digitalWrite(PUL,LOW);
    delayMicroseconds(delayTime);
  }
}
//to increase the revolutions, change the 6400 in the for loops
//the revolutions this corresponds to is based on the number of microsteps





//This function receives serial communications and returns
//For this program, it allows you to type in numbers in the serial monitor to change the delayTime
String receiveSerialData() {

  int incomingByte; // for incoming serial data
  int lengthOfIncomingData;
  int * allBytesArray;
  String converted = "";

  //checks to see if there is anything waiting on the serial buffer
  //Serial.println("Serial.available() is returing: " + String(Serial.available()));

  //if there is anything on the buffer, it stops checking, reads what was there, and returns what you sent as a string
  if (Serial.available() > 0) {
    //Serial.println("theres new data oh boy");delay(500);
   
    lengthOfIncomingData = Serial.available();
    //Serial.println("you sent me " + String(lengthOfIncomingData) + " numbers");
    //delay(500);
    allBytesArray = new int[lengthOfIncomingData];
    //Serial.println("I just created an array with " + String(lengthOfIncomingData) + " elements");delay(500);

    for (int i=0; i < lengthOfIncomingData; i++){
      incomingByte = Serial.read();
      allBytesArray[i] = incomingByte;
    }
    //Serial.print("The array is [");
    for (int i=0; i < lengthOfIncomingData; i++){
      //Serial.print(" " + String(allBytesArray[i]));
    }
    //Serial.println(" ]");
    for (int i=0; i < lengthOfIncomingData; i++){
      converted += String(char(allBytesArray[i]));
    }
    //Serial.println(converted);
    return converted;
    converted = "";
  }
  //delay(1500);
}
