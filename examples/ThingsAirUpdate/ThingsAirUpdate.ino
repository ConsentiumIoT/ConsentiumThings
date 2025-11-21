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
constexpr int interval = 5000;        // Data transmission interval (5 seconds for free tier)
const int updateInterval = 100;       // Check for OTA updates every 100 cycles

void setup() {
  // Start serial communication for debugging
  Serial.begin(115200);
  
  // Consentium IoT branding message
  Serial.println("Consentium IoT - Edge Board Library");
  Serial.println("-----------------------------------------");
  Serial.println("Initializing ConsentiumThings Board...");

  // Begin WiFi connection
  board.connectWiFi(ssid, pass);

  // Initialize the board for sending data
  board.enableSend(SendApiKey, BoardApiKey);

  // Enable battery monitoring on ADC_IN pin (optional)
  // board.enableBatteryMonitoring(ADC_IN);
  
  // Enable OTA updates
  board.enableAirUpdate(ReceiveApiKey, BoardApiKey);

  // Set the interval for checking OTA updates
  board.setSyncInterval(updateInterval);

  Serial.println("ConsentiumThings Board Initialized!");
  Serial.println("-----------------------------------------");
  Serial.println();
}

void loop() {
  // Prepare sample sensor data
  double data_0 = 1.0; // Sample temperature data
  vector<double> sensorValues = {data_0}; // Sensor data vector

  // Sensor info with units can be defined as
  // const char* sensorInfo[] = {"Temperature/C"};

  // Sensor information array with no units
  const char* sensorInfo[] = {"Temperature"}; 

  board.airSync(sensorValues, sensorInfo, LOW_PRE); // Send with low precision

  // Wait before the next data transmission
  delay(interval);
}
