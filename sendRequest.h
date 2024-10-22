#ifndef SENDREQUEST_H
#define SENDREQUEST_H

// Function to connect to WiFi
void connectToWiFi(const char* ssid, const char* password);

// Function to print WiFi connection details
void connectionDetails();

// Function to transmit sensor data to server
void transmitData(float temperature, float humidity, int lightLevel, const String& timestamp);

#endif