const int analogInPin = A0; 
int buf[10],temp;
unsigned long int avgValue;

void setup() {
 Serial.begin(9600);
}


 
void loop() {

  //takes 10 voltage measurements (analogRead returns int 0 to 1023)
 for(int i=0;i<10;i++){ 
  buf[i]=analogRead(analogInPin);
  delay(10);
 }

 //sorts the array by size of reading
 for(int i=0;i<9;i++){
  for(int j=i+1;j<10;j++){
   if(buf[i]>buf[j]){
    temp=buf[i];
    buf[i]=buf[j];
    buf[j]=temp;
   }
  }
 }

 //calculates the average value of the middle 6 values
 avgValue=0;
 for(int i=2;i<8;i++) {
  avgValue+=buf[i];
 }
 
 //converts from 0 to 1023 to 0 to 5
 float pHVol=(float)5.0*avgValue/1024/6;
 float phValue = -40.05 * pHVol + 107.39;
 
 for(int i=2;i<8;i++){
  Serial.println(buf[i]);
  delay(100);
 }
 
 Serial.println(pHVol,2);
 Serial.println("pHVol * -40.05 = " + String(pHVol * -40.05));
 Serial.print("sensor = ");
 Serial.println(phValue,2);
 Serial.println();
 
 delay(2000);
}
