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


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  //declaring pins for buttons as inputs. Input_pullup makes it read high when not connected to ground(when button is pressed)
  pinMode(buttonRight,INPUT_PULLUP);
  pinMode(buttonLeft,INPUT_PULLUP);
  pinMode(buttonUp,INPUT_PULLUP);
  pinMode(buttonDown,INPUT_PULLUP);

  //declaring all pins for motors as outputs  
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

  //setting motor direction
  digitalWrite(DIR_left,HIGH);
  digitalWrite(DIR_right,HIGH);
  digitalWrite(DIR_up,HIGH);
  digitalWrite(DIR_down,HIGH);



  Serial.println("Setup Complete");
  Serial.println("Beginning loop, press a button to move that motor");
}

void loop() {
  
  if (digitalRead(buttonLeft) == LOW) {
    Serial.println("left button pressed");
       
    digitalWrite(PUL_left,LOW);
    delayMicroseconds(500);
    digitalWrite(PUL_left,HIGH);
    delayMicroseconds(500);
  }

  if (digitalRead(buttonRight) == LOW) {
    Serial.println("right button pressed");
    
    digitalWrite(PUL_right,LOW);
    delayMicroseconds(500);
    digitalWrite(PUL_right,HIGH);
    delayMicroseconds(500);
  }

  if (digitalRead(buttonUp) == LOW) {
    Serial.println("up button pressed");

    digitalWrite(PUL_up,LOW);
    delayMicroseconds(500);
    digitalWrite(PUL_up,HIGH);
    delayMicroseconds(500);
  }

  if (digitalRead(buttonDown) == LOW) {
    Serial.println("down button pressed");

    digitalWrite(PUL_down,LOW);
    delayMicroseconds(500);
    digitalWrite(PUL_down,HIGH);
    delayMicroseconds(500);
  }
}
