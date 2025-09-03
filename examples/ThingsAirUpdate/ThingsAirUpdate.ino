/***************************************************
  Consentium IoT - Edge Board Library
  -------------------------------------------------
  This library is designed for use with Consentium IoT's 
  ESP8266, ESP32, and Raspberry Pi Pico W-compatible edge boards.

  Features:
  - Seamless WiFi connectivity
  - REST-based data transmission to Consentium IoT Cloud
  - Over-The-Air (OTA) updates for firmware
  - Support for analog devices using ADC_IN alias

  Hardware Setup:
  - Connect an LED to GPIO 16 (ESP8266), GPIO 23 (ESP32), or GPIO 25 (Raspberry Pi Pico W) 
    to indicate REST events.
  - Connect analog devices to ADC_IN:
      * A0 (ESP8266)
      * GPIO 34 (ESP32)
      * GPIO 26 (Raspberry Pi Pico W)

  Tutorials and Documentation:
  Visit us at: https://docs.consentiumiot.com/code-usage/consentiumthings-arduino-api-with-ota-updates

  For Support:
  Email: official@consentiumiot.com

  MIT license - Redistribution must include this header.
 ****************************************************/

#include <ConsentiumThings.h>

// Define firmware version
#define FIRMWARE_VERSION "0.0"

// Create ConsentiumThings object with firmware version
ConsentiumThingsDalton board(FIRMWARE_VERSION);

// WiFi credentials
const char *ssid = "YOUR_WIFI_SSID";       // Replace with your WiFi SSID
const char *pass = "YOUR_WIFI_PASSWORD";   // Replace with your WiFi password

// API keys
const char *SendApiKey = "YOUR_API_KEY";       // API key for sending data
const char *ReceiveApiKey = "YOUR_API_KEY";    // API key for receiving OTA updates
const char *BoardApiKey = "YOUR_BOARD_API_KEY"; // API key for the board

// Data transmission and update intervals
constexpr int interval = 7000;        // Data transmission interval (7 seconds)
const int updateInterval = 100;       // Check for OTA updates every 100 cycles

// Loop cycle counter
int loopCounter = 0;

void setup() {
  // Consentium IoT branding message
  Serial.println("Consentium IoT - Edge Board Library");
  Serial.println("-----------------------------------------");
  Serial.println("Initializing ConsentiumThings Board...");

  // Begin WiFi connection
  board.initWiFi(ssid, pass);

  // Initialize the board for sending data
  board.beginSend(SendApiKey, BoardApiKey);

  // Enable OTA updates
  board.beginOTA(ReceiveApiKey, BoardApiKey);

  Serial.println("ConsentiumThings Board Initialized!");
  Serial.println("-----------------------------------------");
  Serial.println();
}

void loop() {
  // Prepare sample sensor data
  double data_0 = 1.0; // Sample temperature data
  vector<double> sensorValues = {data_0}; // Sensor data vector
  const char* sensorInfo[] = {"Temperature"}; // Sensor information array


  board.sendData(sensorValues, sensorInfo, LOW_PRE); // Send with low precision

  // Increment the loop counter
  loopCounter++;

  // Check for OTA updates periodically
  if (loopCounter >= updateInterval) {
    Serial.println("[Consentium IoT] Checking for OTA updates...");
    board.checkAndPerformUpdate(); // Check and perform OTA updates
    loopCounter = 0; // Reset the counter after checking for updates
  }

  // Wait before the next data transmission
  delay(interval);
}
