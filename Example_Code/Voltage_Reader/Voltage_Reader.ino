const int pin = A0;
int voltage = 999;

void setup() {
  Serial.begin(9600);
}

void loop() {
  voltage = analogRead(pin);
  Serial.println("Base reading is " + String(voltage) );
  delay(500);
  Serial.println("Reading converted to 5v is " + String(5.0*voltage/1023.0));
  Serial.println();
  delay(2000);
}
