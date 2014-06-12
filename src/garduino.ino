#include "DHT.h"

DHT dht;

// -- Pins
const int MOISTURE_PIN = A0;
const int LDR_PIN = A1;
const int TEMPERATURE_PIN = 3;
const int PUMP_PIN = 4;
const int PH_PIN = 5;
const int PH_STATUS_PIN = 6;

// -- Parts status
bool pump = false;
bool light = false; // Not supported for the moment
bool badpHFlag = false; // Will be set if there is a bad pH detected
bool badTemperatureFlag = false; // Will be set if the temperature is to hot or cold for the herbs

void setup() {
	Serial.begin(9600);
	dht.setup(TEMPERATURE_PIN);
	pinMode(PUMP_PIN, OUTPUT); 
}


void loop() {

	delay(dht.getMinimumSamplingPeriod());

	// Handle REST info call
	handleREST();

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
	manageGarden();

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

void handleSensors() {

	// Read sensor values
	int moisture = getMoistureValue();
	float temperature = getTemperatureValue();
	float humidity = getHumidityValue();
	int light = getLightValue();
	int pH = 6;

	// Water control
	if(moisture < 700) {
		pump = true;
	}
	if(moisture > 900) {
		pump = false;
	}

	// pH
	if(pH > 5 && pH < 7) {
		badpHFlag = false;
	} else {
		badpHFlag = true;
	}

	// Temperature
	if(temperature > 20 && temperature < 40) {
		badTemperatureFlag = false;
	} else {
		badTemperatureFlag = true;
	}

	log(temperature, humidity, moisture, light, pH);

}

/**
 * Based on https://github.com/jjg/RESTduino/blob/master/RESTduino.ino
 */
void handleREST() {

	// Read sensor values
	int moisture = getMoistureValue();
	float temperature = getTemperatureValue();
	float humidity = getHumidityValue();
	int light = getLightValue();
	int pH = 6;

	String jsonOut = String();

	jsonOut += "{\"";
	jsonOut += "\"moisture\"";
	jsonOut += "\":\"";
	jsonOut += String(moisture);
	jsonOut += "\"}";

	//  return value with wildcarded Cross-origin policy
	/*client.println("HTTP/1.1 200 OK");
	client.println("Content-Type: text/html");
	client.println("Access-Control-Allow-Origin: *");
	client.println();
	client.println(jsonOut);*/

}

void manageGarden() {

	if(pump) {
		digitalWrite(PUMP_PIN, HIGH);
	} else {
		digitalWrite(PUMP_PIN, LOW);
	}

	if(badpHFlag) {
		digitalWrite(PH_STATUS_PIN, HIGH);
	} else {
		digitalWrite(PH_STATUS_PIN, LOW);
	}

}

void log(float temperature, float humidity, int moisture, int light, int pH) {
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
	Serial.print(light);
	Serial.print("\t");  
	Serial.print("pH: ");
	Serial.print(pH);
	Serial.print("\n");
}
