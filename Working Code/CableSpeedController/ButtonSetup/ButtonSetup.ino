//button pins
int buttonRight = 2;
int buttonLeft = 3;
int updownSwitch = 4;
int upDownButton = 5;
int buttonTop;
int buttonBottom;

bool upState;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  pinMode(buttonRight,INPUT_PULLUP);
  pinMode(buttonLeft,INPUT_PULLUP);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (digitalRead(buttonLeft) == LOW) {
    Serial.println("left button pressed"); 
  }

  if (digitalRead(buttonRight) == LOW) {
    Serial.println("right button pressed");
  }

  if (digitalRead(updownSwitch) == LOW) {
    upState = true;
  }
  else {
    upState = false;
  }

  if (digitalRead(upDownButton) == LOW) {
    if (upState = true) {
      Serial.println("Go up");
    }
    else {
      Serial.println("Go down");
    }
  }
}
