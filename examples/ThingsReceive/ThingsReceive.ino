/***************************************************
  Consentium IoT - Edge Board Library
  -------------------------------------------------
  This library is designed for use with Consentium IoT's 
  ESP8266, ESP32, and Raspberry Pi Pico W-compatible edge boards.

  Features:
  - Seamless WiFi connectivity
  - REST-based data reception and integration with Consentium IoT Cloud

  Hardware Setup:
  - Connect an LED to GPIO 16 (ESP8266), GPIO 23 (ESP32), or GPIO 25 (Raspberry Pi Pico W) 
    to indicate REST events.

  Tutorials and Documentation:
  Visit us at: https://docs.consentiumiot.com/code-usage/consentiumthings-arduino-data-receiver-api

  For Support:
  Email: official@consentiumiot.com

  MIT license - Redistribution must include this header.
 ****************************************************/

#include <ConsentiumThings.h>

ConsentiumThingsDalton board; // Create ConsentiumThings object

const char *ssid = "YOUR_WIFI_SSID";       // WiFi SSID
const char *pass = "YOUR_WIFI_PASSWORD";   // WiFi password
constexpr int interval = 7000;             // Data retrieval interval (7 seconds)
const char *ReceiveApiKey = "YOUR_API_KEY";   // API key for receiving data
const char *BoardApiKey = "YOUR_BOARD_API_KEY"; // API key for the edge board

void setup() {
  // Consentium IoT branding message
  Serial.println("Consentium IoT - Edge Board Library");
  Serial.println("------------------------------------");
  Serial.println("Initializing ConsentiumThings Board...");

  // Begin WiFi connection
  board.initWiFi(ssid, pass);

  // Initialize IoT board to receive data
  board.beginReceive(ReceiveApiKey, BoardApiKey);

  Serial.println("ConsentiumThings Board Initialized!");
  Serial.println("------------------------------------");
  Serial.println();
}

void loop() {
  // Get sensor data from the Consentium IoT Cloud
  auto dataPairs = board.receiveData();

  // Print received sensor data
  Serial.println("[Consentium IoT] Received Data:");
  for (size_t i = 0; i < dataPairs.size(); i++) {
    double data = dataPairs[i].first; 
    String info = dataPairs[i].second.c_str();

    Serial.print("  Data: ");
    Serial.print(data);
    Serial.print(" \t");
    Serial.print("Info: ");
    Serial.println(info);
  }
  Serial.println();

  // Wait before the next request
  delay(interval);
}
