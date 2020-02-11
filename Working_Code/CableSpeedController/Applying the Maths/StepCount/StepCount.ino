#include <math.h>

//button pins
int buttonRight = A2;
int buttonLeft = A3;
int buttonUp = A1;
int buttonDown = A0;

//motor driver pins

//left is motor 4
int ENA_left = 11;
int DIR_left = 12;
int PUL_left = 13;

//right is motor 1
int ENA_right = 7;
int DIR_right = 6;
int PUL_right = 5;

//up is motor 2
int ENA_up = 10;
int DIR_up = 9;
int PUL_up = 8;

//down is motor 3
int ENA_down = 2;
int DIR_down = 3;
int PUL_down = 4;

int motordelay = 500; //this is the desired motor delay. If multiple motors are running, the time will be "cut" so that each motor seems to be still experiencing this delay

bool buttonLeftState;
bool buttonRightState;
bool buttonUpState;
bool buttonDownState;

//Global Variables
int mc = 32;        //Microstep setting
int dia = 2;        //Gear diameter in inches
int B = 70;     //Base workspace dimension in inches
int H = 60;   //Height workspace dimension in inches

double x = 0;             //Current x-position
double y = 0;             //Current y-position
double stepCountHor = 0;  //Horizontal step counter
double stepCountVer = 0;  //Vertical step counter
double nbrOfSteps =0;    //Step Counter

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  //declaring pins for buttons as inputs. Input_pullup makes it read high when not connected to ground(when button is pressed)
  pinMode(buttonRight, INPUT_PULLUP);
  pinMode(buttonLeft, INPUT_PULLUP);
  pinMode(buttonUp, INPUT_PULLUP);
  pinMode(buttonDown, INPUT_PULLUP);

  //declaring all pins for motors as outputs. On Arduinos, digitial pins default to input
  pinMode(PUL_left, OUTPUT);
  pinMode(PUL_right, OUTPUT);
  pinMode(PUL_up, OUTPUT);
  pinMode(PUL_down, OUTPUT);

  pinMode(ENA_left, OUTPUT);
  pinMode(ENA_right, OUTPUT);
  pinMode(ENA_up, OUTPUT);
  pinMode(ENA_down, OUTPUT);

  pinMode(DIR_left, OUTPUT);
  pinMode(DIR_right, OUTPUT);
  pinMode(DIR_up, OUTPUT);
  pinMode(DIR_down, OUTPUT);

  //enabling all motors. LOW is enabled. HIGH is disabled
  digitalWrite(ENA_left, LOW);
  digitalWrite(ENA_right, LOW);
  digitalWrite(ENA_up, LOW);
  digitalWrite(ENA_down, LOW);


  Serial.println("Setup Complete");
  Serial.println("Beginning loop, press a button to move that motor");
}

void loop() {
  
  int delaytime = setDelay();

  //read button states
  buttonLeftState = digitalRead(buttonLeft);
  buttonRightState = digitalRead(buttonRight);
  buttonUpState = digitalRead(buttonUp);
  buttonDownState = digitalRead(buttonDown);

  /*if (buttonLeftState == LOW) {
    //Serial.println("left button pressed");

    digitalWrite(PUL_left, LOW);
    delayMicroseconds(delaytime);
    digitalWrite(PUL_left, HIGH);
    delayMicroseconds(delaytime);
    }

    if (buttonRightState == LOW) {

    digitalWrite(PUL_right, LOW);
    delayMicroseconds(delaytime);
    digitalWrite(PUL_right, HIGH);
    delayMicroseconds(delaytime);
    }

    if (buttonUpState == LOW) {
    //Serial.println("up button pressed");

    digitalWrite(PUL_up, LOW);
    delayMicroseconds(delaytime);
    digitalWrite(PUL_up, HIGH);
    delayMicroseconds(delaytime);
    }

    if (buttonDownState == LOW) {
    //Serial.println("down button pressed");

    stepMotor(PUL_down, delaytime);
    }
  */
}

void moveCartLeft() {
  setDir(DIR_left, HIGH); setDir(DIR_right, LOW);                             //set the direction of the primary motors to whatever would move the cart the correct direction
  stepMotor(PUL_left, motordelay); stepMotor(PUL_right, motordelay);          //step the primary cables once
  nbrOfSteps = calcSecondaryStep(x, y, 3);                                    //calculate the cooresponding nbr of steps the secondary need to take, the direction the cart is moving with impact if the number is negative of positive
  stepCountVer = stepCountVer + nbrOfSteps;                                   //add the nbr of steps to the step count, this should be a global variable

  //stepCountVer will be positive if needs to move slightly out (loosen) and negative if needs to move slightly in (tightened)

  if (stepCountVer > 1) {
    setDir(DIR_up, HIGH); setDir(DIR_down, HIGH); //set direction that would tighten the cables
    stepMotor(PUL_up, motordelay); stepMotor(PUL_down, motordelay); //step both motors
    stepCountVer = stepCountVer - 1;
  }
  if (stepCountVer < -1) {
    setDir(DIR_up, LOW); setDir(DIR_down, LOW);
    stepMotor(PUL_up, motordelay); stepMotor(PUL_down, motordelay);
    stepCountVer = stepCountVer + 1;
  }
}

void moveCartRight() {
  setDir(DIR_left, LOW); setDir(DIR_right, HIGH);
  stepMotor(PUL_left, motordelay); stepMotor(PUL_right, motordelay);
  nbrOfSteps = calcSecondaryStep(x, y, 4);
  stepCountVer = stepCountVer + nbrOfSteps;

  if (stepCountVer > 1) {
    setDir(DIR_up, HIGH); setDir(DIR_down, HIGH);
    stepMotor(PUL_up, motordelay); stepMotor(PUL_down, motordelay);
    stepCountVer = stepCountVer - 1;
  }

  if (stepCountVer < -1) {
    setDir(DIR_up, LOW); setDir(DIR_down, LOW);
    stepMotor(PUL_up, motordelay); stepMotor(PUL_down, motordelay);
    stepCountVer = stepCountVer + 1;
  }
}

void moveCartUp() {
  setDir(DIR_up, HIGH); setDir(DIR_down, LOW);
  stepMotor(PUL_up, motordelay); stepMotor(PUL_down, motordelay);
  nbrOfSteps = calcSecondaryStep(x, y, 1);
  stepCountHor = stepCountHor + nbrOfSteps;

  if (stepCountHor > 1) {
    setDir(DIR_left, HIGH); setDir(DIR_right, HIGH);
    stepMotor(PUL_left, motordelay); stepMotor(PUL_right, motordelay);
    stepCountHor = stepCountHor - 1;
  }

  if (stepCountHor < -1) {
    setDir(DIR_left, LOW); setDir(DIR_right, LOW);
    stepMotor(PUL_left, motordelay); stepMotor(PUL_right, motordelay);
    stepCountHor = stepCountHor + 1;
  }
}

void moveCartDown() {
  setDir(DIR_up, LOW); setDir(DIR_down, HIGH);
  stepMotor(PUL_up, motordelay); stepMotor(PUL_down, motordelay);
  nbrOfSteps = calcSecondaryStep(x, y, 2);
  stepCountHor = stepCountHor + nbrOfSteps;

  if (stepCountHor > 1) {
    setDir(DIR_left, HIGH); setDir(DIR_right, HIGH);
    stepMotor(PUL_left, motordelay); stepMotor(PUL_right, motordelay);
    stepCountHor = stepCountHor - 1;
  }

  if (stepCountHor < -1) {
    setDir(DIR_left, LOW); setDir(DIR_right, LOW);
    stepMotor(PUL_left, motordelay); stepMotor(PUL_right, motordelay);
    stepCountHor = stepCountHor + 1;
  }
}

void setDir(int motor, bool dir) {
  digitalWrite(motor, dir);
}

//I think since the motion is based on step count, the delayTime will stay constant, so this argument could be removed
void stepMotor(int PUL, int delayTime) {
  digitalWrite(PUL, LOW);
  delayMicroseconds(delayTime);
  digitalWrite(PUL, HIGH);
  delayMicroseconds(delayTime);
}

int setDelay() {
  int count = 0; //nbr of motors with button pressed
  int delayTime;
  if (buttonLeftState == LOW) {
    count = count + 1;
  }
  if (buttonRightState == LOW) {
    count = count + 1;
  }
  if (buttonUpState == LOW) {
    count = count + 1;
  }
  if (buttonDownState == LOW) {
    count = count + 1;
  }

  if (count != 0) {
    delayTime = motordelay / count;
    Serial.println(count);
  }

  else {
    delayTime = motordelay; //no motor is moving so it doesnt matter, just avoiding dividing by zero
  }

  return delayTime;
}

//delta returns the change of length in the top cable over the change in length of the horizontal cable
double delta(double x, double y) {
  double dx = (dia * PI) / (200 * mc); double dy = dx;
  double lACD = sqrt(pow(x, 2) + pow(H - y, 2)) + sqrt(pow(B - x, 2) + pow(H - y, 2)) - sqrt(pow(x + dx, 2) + pow(H - y - dy, 2)) - sqrt(pow(B - x - dx, 2) + pow(H - y - dy, 2));
  double lBCD = sqrt(pow(x, 2) + pow(y, 2)) + sqrt(pow(x, 2) + pow(H - y, 2)) - sqrt(pow(x + dx, 2) + pow(y + dy, 2)) - sqrt(pow(x + dx, 2) + pow(H - y - dy, 2));
  double del = lACD / lBCD;
  return del;
}

//Calculates the number of secondary steps that need to be taken.
//Dir: 1 is up, 2 is down, 3 is left, 4 is right
double calcSecondaryStep(double x, double y, int dir) {
  switch (dir) {
    case 1:
      return 1 / delta(x, y);
      break;
    case 2:
      return 1 / delta(x, y);
      break;
    case 3:
      return delta(x, y);
      break;
    case 4:
      return delta(x, y);
      break;
  }
}
