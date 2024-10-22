/*
 * -------------------------------------------
 * Project/Program Name : Programming ESP8266
 * File Name            : sendRequest.cpp
 * Author               : Ryan Chan
 * Date                 : 15/10/2024
 * Version              : 1.0
 * 
 * Purpose:
 *    This file contains functions that connect the ESP8266 to a WiFi network, 
 *    retrieve connection details, and send sensor data to a server using HTTPS.
 * 
 * Inputs:
 *    - WiFi credentials (SSID and password)
 *    - Sensor data (temperature, humidity, light level, timestamp)
 * 
 * Outputs:
 *    - Sends an HTTP GET request to a remote server to transmit sensor data
 *    - Prints connection details and server response
 * 
 * Dependencies:
 *    - External libraries: ESP8266WiFi.h, ESP8266HTTPClient.h
 *    - Hardware: ESP8266
 * ------------------------------------------------------------
 */

#include <Arduino.h>
#include <ESP8266WiFi.h>     // WiFi library for ESP8266
#include <ESP8266HTTPClient.h> // HTTP client for ESP8266
#include <sendRequest.h>

// Connect to WiFi
void connectToWiFi(const char* ssid, const char* password) {
    Serial.println("Connecting to WiFi");
    WiFi.begin(ssid, password); // Connection to WiFi Starts until status()=WL_CONNECTED

    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print("."); // waiting to get connected
    }
    // Details of the connection
    connectionDetails();
}

// Print details of the connection
void connectionDetails()
{
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());  // Tell us what network we're connected to
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());   // Send the IP address of the ESP8266 to the computer
  Serial.print("Channel:\t");
  Serial.println(WiFi.channel());          
  Serial.print("RSSI:\t");
  Serial.println(WiFi.RSSI());   
  Serial.print("DNS IP:\t");
  Serial.println(WiFi.dnsIP(1));    // get the DNS IP Address    
  Serial.print("Gateway IP:\t");
  Serial.println(WiFi.gatewayIP()); 
  Serial.println("---------------------");    
}

// Function to transmit data to server
void transmitData(float temperature, float humidity, int lightLevel, const String& timestamp) {
    WiFiClientSecure client;
    HTTPClient http;
    client.setInsecure(); // Bypass SSL certificate validation

    // Encode timestamp
    String encodedTime = timestamp;
    encodedTime.replace(" ", "%20");
    encodedTime.replace(":", "%3A");

    // Construct the URL
    String url = "https://rckh.xyz/dbinsert.php";
    url += "?node=node_6";
    url += "&temperature=" + String(temperature);
    url += "&humidity=" + String(humidity);
    url += "&light_level=" + String(lightLevel);
    url += "&time_received=" + encodedTime;

    Serial.println("URL: " + String(url));

    // GET request
    http.begin(client, url);
    int httpResponseCode = http.GET();

    // print response code: e.g.,:200
    Serial.println("Response code <-- " + String(httpResponseCode));
    if (httpResponseCode > 0) {
        String response = http.getString();
        Serial.print("Server response: ");
        Serial.println(response);
    } else {
        Serial.println("Failed to send data");
    }
    // Close the connection
    http.end(); 
}