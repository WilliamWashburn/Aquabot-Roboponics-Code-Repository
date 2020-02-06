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

int motordelay = 500; //this is the desired motor delay. If multiple motors are running, the time will be "cut" so that each motor is still experiencing this delay

bool buttonLeftState;
bool buttonRightState;
bool buttonUpState;
bool buttonDownState;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  //declaring pins for buttons as inputs. Input_pullup makes it read high when not connected to ground(when button is pressed)
  pinMode(buttonRight,INPUT_PULLUP);
  pinMode(buttonLeft,INPUT_PULLUP);
  pinMode(buttonUp,INPUT_PULLUP);
  pinMode(buttonDown,INPUT_PULLUP);

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
  digitalWrite(ENA_left,LOW);
  digitalWrite(ENA_right,LOW);
  digitalWrite(ENA_up,LOW);
  digitalWrite(ENA_down,LOW);

/*
  //setting motor direction
  digitalWrite(DIR_left,HIGH);
  digitalWrite(DIR_right,HIGH);
  digitalWrite(DIR_up,HIGH);
  digitalWrite(DIR_down,HIGH);
*/


  Serial.println("Setup Complete");
  Serial.println("Beginning loop, press a button to move that motor");
}

void loop() {
  /*
   * Current thinking of logic:
   * Starting at a specified position (x,y), if a button (up, down, left, or right), the cart will move in that direction.
   * The speed it moves will be based off of the speed of the main set of cables (the left and right for horizontal and the up and down for vertical)
   * A step of the main pairs will occur one per time interval (whatever delay between pulse seems to give smoothest operation)
   * Since the number of steps the secondary pair will take will be less than 1, a value is needed to count the partial steps, once this value reached above 1, a step will be taken and the value will decrease by one
   * 
   * Needs:
   * speed variable
   * position variables
   * secondary cable partial step count
   * 
   * Pseudo Code:
   * if (left button pressed) {
   *   moveCartLeft()
   * }
   * 
   * void moveCartLeft() {
   *   setDirleft(HIGH);setDirright(LOW); //set the direction of the primary motors to whatever would move the cart the correct direction
   *   stepMotor(left);stepMotor(right); //step the primary cables once
   *   nbrOfSteps = calcSecondaryStep(x,y,dir) //calculate the cooresponding nbr of steps the secondary need to take, the direction the cart is moving with impact if the number is negative of positive
   *   stepCountHor = stepCountHor + nbrOfSteps //add the nbr of steps to the step count, this should be a global variable
   *   
   *   //stepCountHor will be positive if needs to move slightly out (loosen) and negative if needs to move slightly in (tighened)
   *   
   *   if (stepCountHor > 1) {
   *     setDir(left, HIGH);setDir(right,HIGH); //set direction that would tighten the cables
   *   ``stepMotor(up);stepMotor(down); //step both motors
   *     stepCountHor = stepCountHor - 1;
   *   }
   *   
   *   if (stepCountHor < 1) {
   *     setDir(left, LOW);setDir(right,LOW); //set direction that would tighten the cables
   *   ``stepMotor(up);stepMotor(down); //step both motors
   *     stepCountHor = stepCountHor + 1;
   *   }
   * }
   * 
   * void setDir(int motor, bool dir) {
   *    
   * }
   * 
   */
 
  int delaytime = setDelay();
  buttonLeftState = digitalRead(buttonLeft);
  buttonRightState = digitalRead(buttonRight);
  buttonUpState = digitalRead(buttonUp);
  buttonDownState = digitalRead(buttonDown);
  
  if ( buttonLeftState == LOW) {
    //Serial.println("left button pressed");
    
    digitalWrite(PUL_left,LOW);
    delayMicroseconds(delaytime);
    digitalWrite(PUL_left,HIGH);
    delayMicroseconds(delaytime);
  }

  if (buttonRightState == LOW) {

    digitalWrite(PUL_right,LOW);
    delayMicroseconds(delaytime);
    digitalWrite(PUL_right,HIGH);
    delayMicroseconds(delaytime);
  }

  if (buttonUpState == LOW) {
    //Serial.println("up button pressed");

    digitalWrite(PUL_up,LOW);
    delayMicroseconds(delaytime);
    digitalWrite(PUL_up,HIGH);
    delayMicroseconds(delaytime);
  }

  if (buttonDownState == LOW) {
    //Serial.println("down button pressed");

    stepMotor(PUL_down,delaytime);
  }
}

//I think since the motion is based on step count, the delayTime will stay constant, so this argument could be removed
void stepMotor(int PUL, int delayTime){
  digitalWrite(PUL,LOW);
  delayMicroseconds(delayTime);
  digitalWrite(PUL,HIGH);
  delayMicroseconds(delayTime);
}

int setDelay(){
  int count = 0; //nbr of motors with button pressed
  int delayTime;
  if (buttonLeftState == LOW){count = count + 1;}
  if (buttonRightState == LOW){count = count +1;}
  if (buttonUpState == LOW){count = count +1;}
  if (buttonDownState== LOW){count = count +1;}

  if (count!=0) {
    delayTime = motordelay/count;
    Serial.println(count);
  }

  else {
    delayTime=motordelay; //no motor is moving so it doesnt matter, just avoiding dividing by zero
  }

  return delayTime;
}
