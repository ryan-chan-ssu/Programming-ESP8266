#ifndef SENSORS_H
#define SENSORS_H

#include <DHT.h>    // DHT library

// Define pin used for button output
#define BUTTON_PIN D1
#define TEMP_HUMIDITY_PIN D2
#define LIGHT_SENSOR_PIN A0

// Define DHT sensor type
#define DHTTYPE DHT11
extern DHT dht; // Initialize the DHT sensor

// Declare functions
float readTemperature();
float readHumidity();
int readLightLevel();
String getCurrentTime(const String& timeZone);

#endif
