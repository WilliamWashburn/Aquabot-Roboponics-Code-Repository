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
  
  pinMode(buttonRight,INPUT_PULLUP);
  pinMode(buttonLeft,INPUT_PULLUP);
  pinMode(buttonUp,INPUT_PULLUP);
  pinMode(buttonDown,INPUT_PULLUP);

  Serial.println("Setup Complete");
  Serial.println("Beginning loop, press a button to move that motor");
}

void loop() {
  
  if (digitalRead(buttonLeft) == LOW) {
    Serial.println("left button pressed"); 
  }

  if (digitalRead(buttonRight) == LOW) {
    Serial.println("right button pressed");
  }

  if (digitalRead(buttonUp) == LOW) {
    Serial.println("up button pressed");
  }

  if (digitalRead(buttonDown) == LOW) {
    Serial.println("down button pressed");
  }

  
}
