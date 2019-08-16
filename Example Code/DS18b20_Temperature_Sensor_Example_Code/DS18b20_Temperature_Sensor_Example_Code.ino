#include <OneWire.h>
#include <DallasTemperature.h>

// Assign to pin 10 of your Arduino to the DS18B20
#define ONE_WIRE_BUS 7

// Create a onewire instanace
OneWire oneWire(ONE_WIRE_BUS);

// Declare a DS18B20 Instance and assing the OneWire reference to it.
DallasTemperature sensors(&oneWire);

void setup(void)
{
  // Start serial port
  Serial.begin(9600);
  //Start the DallasTemperature Library
    sensors.begin();
}

void loop(void)
{ 
  sensors.requestTemperatures(); // Tell the DS18B20 to get make a measurement
  Serial.println(sensors.getTempCByIndex(0),4); // Get that temperature and print it.
  Serial.println(); 
  delay(1000); 
}
