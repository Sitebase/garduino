/*
	Garduino

	Garden control system that will monitor some key values and makes adjustments based on that.
	For example detecting low moisture level will turn on a pump to give the plant more water.

	The circuit:
	* list the components attached to each input
	* list the components attached to each output

	Created 11 06 2014
	By Wim Mostmans (Sitebase)

	https://github.com/sitebase/garduino

*/

#include "DHT.h"
#include <avr/sleep.h>
#include <Narcoleptic.h>
#include <Scheduler.h>
#include <Queue.h>


Scheduler scheduler = Scheduler(); 
Queue myQueue;
DHT dht;

// -- Pins
const int MOISTURE_PIN = A0;
const int LDR_PIN = A1;
const int TEMPERATURE_PIN = 3;
const int PUMP_PIN = 4;
const int PH_PIN = 5;
const int PH_STATUS_PIN = 6;

const int MAX_WATER_CYCLES = 1;
const int NOT_WATERING_CYCLES = 2;

// -- Parts status
bool pump = false;
bool light = false; // Not supported for the moment
bool badpHFlag = false; // Will be set if there is a bad pH detected
bool badTemperatureFlag = false; // Will be set if the temperature is to hot or cold for the herbs

int current_water_cycles = 0;
int current_off_water_cycles = 0;

bool status = false;

void setup() {

	Serial.begin(9600);
	dht.setup(TEMPERATURE_PIN);
	pinMode(PUMP_PIN, OUTPUT); 
	pinMode(13, OUTPUT);
	pinMode(10, INPUT);  
	

    myQueue.scheduleFunction(handleSensors, "Test", 1000, 5000);
    //myQueue.scheduleFunction(testFunction2, "Test", 1000, 5000);

    while(1) {
        myQueue.Run(millis());
        delay(2001);
    }
}


void loop() {

	// Better sleep method
	//delay(dht.getMinimumSamplingPeriod());
	/*Narcoleptic.delay(1000);

	//myQueue.Run(millis());
	scheduler.update();
	if (Serial.available()){            //if we have recieved anything on the Serial
    	scheduler.schedule(setHigh,5000);  //schedule a setHigh call in 500 milliseconds
    	Serial.flush();                   //flush Serial so we do not schedule multiple setHigh calls
  	}


	//digitalWrite(13, HIGH);

	// Check for user input
	// Button clicks
	bool manual = handleInputs();

	// when use enables the pump manually the
	// sensors should not overwrite the pump for example
	// Read sensors and do calculations
	if(manual == false) {
		handleSensors();
	}
	
	// Manage garden
	manageGarden();*/

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

/**
 * Return true if there was a button action
 * if that happens we know we don't need to handle the garden based on the sensors input
 * @return bool
 */
bool handleInputs() {
	return false;
}

int handleSensors(unsigned long now) {

	// Read sensor values
	int moisture = getMoistureValue();
	float temperature = getTemperatureValue();
	float humidity = getHumidityValue();
	int light = getLightValue();
	int pH = 6;

	// Water control
	if(
		moisture > 400 && 
		current_water_cycles < MAX_WATER_CYCLES &&
		light > 100 // Make sure it's not night time
	) {
		pump = true;
		current_water_cycles = current_water_cycles + 1;
		log("Enable pump");
	} else {

		if(pump == true) {
			log("Disable pump");
		}

		pump = false;
		current_off_water_cycles = current_off_water_cycles + 1;

		// When the pump is of for 8 cycles and can be enabled again
		// when dry soil is detected
		// This is done to give the water the time to get in the soil
		if(current_off_water_cycles == NOT_WATERING_CYCLES) {
			current_off_water_cycles = 0;
			current_water_cycles = 0;
		}
	}

	// Temperature
	if(temperature > 20 && temperature < 40) {
		badTemperatureFlag = false;
	} else {
		badTemperatureFlag = true;
	}

	logData(temperature, humidity, moisture, light, pH);

	manageGarden();

}

void manageGarden() {

	if(pump) {
		digitalWrite(PUMP_PIN, HIGH);
	} else {
		digitalWrite(PUMP_PIN, LOW);
	}

	/*if(badpHFlag) {
		digitalWrite(PH_STATUS_PIN, HIGH);
	} else {
		digitalWrite(PH_STATUS_PIN, LOW);
	}*/

}

void log(char *str) {
	Serial.print("-> ");
	Serial.println(str);
}

void logData(float temperature, float humidity, int moisture, int light, int pH) {
	Serial.print("Temperature: ");
	Serial.print(temperature, 1);
	Serial.print("\t");  
	Serial.print("Humidity: ");
	Serial.print(humidity, 1);
	Serial.print("\t");  
	Serial.print("Moisture: ");
	Serial.print(moisture);
	Serial.print("\t");  
	Serial.print("Light: ");
	Serial.println(light); 
}

void setHigh(){
  Serial.println("This is the scheduled task *******");
}

int testFunction(unsigned long now)
{
    Serial.print("Hello: ");
    Serial.println(now);
}

int testFunction2(unsigned long now)
{
    Serial.print("Hello world: ");
    Serial.println(now);
}