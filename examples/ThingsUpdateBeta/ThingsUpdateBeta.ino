/***************************************************
  Consentium IoT - Edge Beta Board Library
  -------------------------------------------------
  This library is designed for use with Consentium IoT's 
  5V tolerant Edge Beta boards.

  Features:
  - Seamless WiFi connectivity
  - REST-based data transmission to the Consentium IoT Cloud
  - Compatible with 4-20mA and 0-10V sensor interfaces

  Tutorials and Documentation:
  Visit us at: https://docs.consentiumiot.com

  For Support:
  Email: official@consentiumiot.com

  MIT license - Redistribution must include this header.
 ****************************************************/

#include <ConsentiumThings.h>

// Create a ConsentiumThings Beta object
ConsentiumThingsBeta board;

// WiFi credentials
const char *ssid = "YOUR_WIFI_SSID";       // Replace with your WiFi SSID
const char *pass = "YOUR_WIFI_PASSWORD";   // Replace with your WiFi password
const char *SendApiKey = "YOUR_API_KEY";      // API key for sending data
const char *BoardApiKey = "YOUR_BOARD_API_KEY"; // API key for the board

// Data transmission interval
constexpr int interval = 7000; // 7 seconds

void setup() {
  // Start Serial communication for debugging
  Serial.begin(115200);

  // Consentium IoT branding message
  Serial.println("Consentium IoT - Edge Beta Board Library");
  Serial.println("-----------------------------------------");
  Serial.println("Initializing ConsentiumThings Beta Board...");

  // Begin WiFi connection
  board.initWiFi(ssid, pass);

  // Initialize the board for sending data
  board.beginSend(SendApiKey, BoardApiKey);

  Serial.println("ConsentiumThings Beta Board Initialized!");
  Serial.println("-----------------------------------------");
  Serial.println();
}

void loop() {
  // Read 4-20mA sensor data from J1 Port 1
  float data_0 = analogRead(IN_4_20_1) * SCALE; 
  // Read 0-10V sensor data from J2 Port 1
  float data_1 = analogRead(IN_0_10_1) * SCALE; 

  // Prepare sensor data and info arrays
  float sensorValues[] = {data_0, data_1};  
  const char* sensorInfo[] = {"Temperature", "Pressure"}; 

  // Calculate the number of sensors
  int sensorCount = sizeof(sensorValues) / sizeof(sensorValues[0]); 

  // Send sensor data to Consentium IoT Cloud
  board.sendData(sensorValues, sensorInfo, sensorCount, LOW_PRE); 

  // Wait for the next data transmission
  delay(interval);
}
