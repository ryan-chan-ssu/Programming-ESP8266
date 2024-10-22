/*
 * -------------------------------------------
 * Project/Program Name : Programming ESP8266
 * File Name            : main.cpp
 * Author               : Ryan Chan
 * Date                 : 15/10/2024
 * Version              : 1.0
 * 
 * Purpose:
 *    This program reads data from two sensors (temperature and humidity from DHT11, and light level from LDR), 
 *    retrieves the current time from an API, and sends this data to the rckh.xyz server using an ESP8266.
 * 
 * Inputs:
 *    - Temperature and humidity from the DHT11 sensor
 *    - Light level from the HW-486 LDR
 *    - Time from the timeapi.io API (or system time if the API is unavailable)
 *    - A push button to trigger data transmission
 * 
 * Outputs:
 *    - Data (node, temperature, humidity, light level, timestamp) is sent to the rckh.xyz server
 * 
 * Example Application:
 *    This program can be used to record temperature, humidity, light levels and upload the data
 *    to a server when the button is pressed.
 * 
 * Dependencies:
 *    - External libraries: ESP8266WiFi.h, HTTPClient.h, DHT.h
 *    - Hardware: ESP8266, DHT11, HW-486 LDR, pushbutton
 * 
 * Usage Notes:
 *    - Change the Wi-Fi credentials in the code according to the network used.
 *    - The program will only send data when the pushbutton is pressed.
 * ---------------------------------------------------------------------------
 */

#include <Arduino.h>
#include <sendRequest.h>
#include <sensors.h>

// WiFi credentials
const char* ssid     = "Loading...";
const char* password = "Finexes0621";

// Time zone options
const char* timeZones[] = {
  "America/New_York",   // Eastern
  "America/Chicago",    // Central
  "America/Denver",     // Mountain
  "America/Los_Angeles",// Pacific
  "America/Anchorage",  // Alaska
  "Pacific/Honolulu",   // Hawaii-Aleutian
  "America/Puerto_Rico" // Atlantic
};
// Default to PST if no input
String timeZone = "America/Los_Angeles";

void setup() {
  Serial.begin(9600);
  delay(400); // just a short delay
  Serial.println("");

  // Connect to WiFi
  connectToWiFi(ssid, password);
  // Initialize the DHT sensor
  dht.begin();
  // Set the button pin mode
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // Display time zone options
  Serial.println("Select Your Time Zone (The default time is PT):");
  Serial.println("1) Eastern Time Zone (ET) - New York City, NY");
  Serial.println("2) Central Time Zone (CT) - Chicago, IL");
  Serial.println("3) Mountain Time Zone (MT) - Denver, CO");
  Serial.println("4) Pacific Time Zone (PT) - Los Angeles, CA");
  Serial.println("5) Alaska Time Zone (AKT) - Anchorage, AK");
  Serial.println("6) Hawaii-Aleutian Time Zone (HAT) - Honolulu, HI");
  Serial.println("7) Atlantic Time Zone (AT) - San Juan, PR");
  Serial.print("-> Enter a number between 1-7 to select your time zone: ");

  while (!Serial.available()) {
    delay(200);
  }

  // Read the input
  int selection = Serial.parseInt();
  // Display the input
  Serial.println(selection);

  // Check if input is valid
  if (selection >= 1 && selection <= 7) {
    // Set timeZone based on user input 
    timeZone = timeZones[selection - 1];
    Serial.print("Time zone set to: ");
    Serial.println(timeZone);
  } else {
    // Set timezone to default if input is invalid
    Serial.println("Invalid selection. Using default time zone (Pacific Time).");
  }
}

void loop() {
  // Check if the button is pressed
  if (digitalRead(BUTTON_PIN) == LOW) {
    Serial.println("Button pressed.");
    delay(200);  // Small debounce delay

    // Read the temperature, humidity, and light level
    float temperature = readTemperature();
    float humidity = readHumidity();
    int lightLevel = readLightLevel();

    if (isnan(temperature) || isnan(humidity) || isnan(lightLevel)) {
      Serial.println("Failed to read sensor data.");
    } else {
      // Get current time
      String currentTime = getCurrentTime(timeZone);
      // Transmit data
      transmitData(temperature, humidity, lightLevel, currentTime);
      Serial.println();
    }

    // Wait for button release
    while (digitalRead(BUTTON_PIN) == LOW) {
      yield(); // avoid watchdog timer resetting the processor
    }
  }
  delay(400); // Short delay to debounce input reading
}