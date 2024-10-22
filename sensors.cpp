/*
 * -------------------------------------------
 * Project/Program Name : Programming ESP8266
 * File Name            : sensor.cpp
 * Author               : Ryan Chan
 * Date                 : 15/10/2024
 * Version              : 1.0
 * 
 * Purpose:
 *    This file contains functions to read data from temperature, humidity, and light sensors, 
 *    and a function to fetch the current time from TimeAPI.io 
 * 
 * Inputs:
 *    - Temperature and humidity from the DHT sensor
 *    - Light level from the LDR sensor
 *    - Time zone input from user
 * 
 * Outputs:
 *    - Prints temperature, humidity, and light level to the serial monitor
 *    - Sends a request to TimeAPI.io to get the current date and time
 * 
 * Dependencies:
 *    - External libraries: ArduinoJson.h, ESP8266WiFi.h, ESP8266HTTPClient.h
 *    - Hardware: ESP8266, DHT11, HW-486 LDR
 * ---------------------------------------------------------------------------
 */

#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>     // WiFi library for ESP8266
#include <ESP8266HTTPClient.h> // HTTP client for ESP8266
#include <sensors.h>

// Initialize the DHT sensor on the pin
DHT dht(TEMP_HUMIDITY_PIN, DHTTYPE); 

// Read temperature from HW-507
float readTemperature() {
    float temperature = dht.readTemperature(); // Get temperature in Celsius
    if (isnan(temperature)) {
        Serial.println("Failed to read temperature");
        return -1;
    }
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" °C");
    return temperature;
}

// Read humidity from HW-507
float readHumidity() {
    float humidity = dht.readHumidity(); // Get humidity
    if (isnan(humidity)) {
        Serial.println("Failed to read humidity");
        return -1;
    }
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.println(" %");
    return humidity;
}

// Read light level from HW-486
int readLightLevel() {
    int lightLevel = analogRead(LIGHT_SENSOR_PIN); // Read analog light level
    if (isnan(lightLevel)) {
        Serial.println("Failed to read light level");
        return -1;
    }
    Serial.print("Light Level: ");
    Serial.print(lightLevel);
    Serial.println(" V");
    return lightLevel;
}

// Get current time from TimeAPI.io
String getCurrentTime(const String& timeZone) {
    WiFiClientSecure client;
    HTTPClient http;
    client.setInsecure(); // Bypass SSL certificate validation
    String url = "https://timeapi.io/api/Time/current/zone?timeZone=" + timeZone;
    http.begin(client, url);
    int httpResponseCode = http.GET();
    if (httpResponseCode > 0) {
        String payload = http.getString();

        // Parse the JSON response
        DynamicJsonDocument jsonDoc(400);
        DeserializationError error = deserializeJson(jsonDoc, payload); // Handle error
        if (error) {
            Serial.print("JSON parsing error: ");
            Serial.println(error.c_str());
            return "";
        }
        String dateTime = jsonDoc["dateTime"];
        dateTime.replace("T", " ");
        int dotIndex = dateTime.indexOf('.');
        if (dotIndex != -1) {
            dateTime = dateTime.substring(0, dotIndex);  // Keep only up to seconds
        }
        Serial.println("Current Time: " + dateTime);
        return dateTime;
    } else {
        Serial.print("Error retrieving time"); // Handle failed requests
        return "";
    }
    http.end();
}