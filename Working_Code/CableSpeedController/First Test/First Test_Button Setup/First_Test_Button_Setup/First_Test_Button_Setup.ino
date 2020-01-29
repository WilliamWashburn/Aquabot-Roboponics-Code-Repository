//button pins
int buttonRight = A2;
int buttonLeft = A3;
int buttonUp = A1;
int buttonDown = A0;

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
