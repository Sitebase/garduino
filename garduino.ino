#include "DHT.h"

DHT dht;
int MOISTURE_PIN = A0;
int LDR_PIN = A1;
int TEMPERATURE_PIN = 3;
int PUMP_PIN = 4;

void setup() {
  Serial.begin(9600);
  dht.setup(TEMPERATURE_PIN);
  pinMode(PUMP_PIN, OUTPUT); 
}


void loop() {

  delay(dht.getMinimumSamplingPeriod());

  int moisture = getMoistureValue();

  Serial.print("Temperature: ");
  Serial.print(getTemperatureValue(), 1);
  Serial.print("\t");  
  Serial.print("Humidity: ");
  Serial.print(getHumidityValue(), 1);
  Serial.print("\t");  
  Serial.print("Moisture: ");
  Serial.print(moisture);
  Serial.print("\t");  
  Serial.print("Light: ");
  Serial.print(getLightValue());
  
  if(moisture < 700) {
    digitalWrite(PUMP_PIN, LOW);
  } else {
    digitalWrite(PUMP_PIN, HIGH);
  }
  
  Serial.print("\n");
}

int getLightValue() {
   int value = analogRead(LDR_PIN); 
   return value;
}

int getMoistureValue() {
   int value = analogRead(MOISTURE_PIN); 
   return value;
}

float getTemperatureValue() {
  // dht.toFahrenheit(temperature) // for temperature in Fahrenheit
  float value = dht.getTemperature();
  return value;   
}

float getHumidityValue() {
  float value = dht.getHumidity();
  return value; 
}
