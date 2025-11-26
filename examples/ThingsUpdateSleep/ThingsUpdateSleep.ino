/***************************************************
  Consentium IoT - Edge Board Library
  -------------------------------------------------
  This library is designed for use with Consentium IoT's 
  ESP8266, ESP32, and Raspberry Pi Pico W-compatible edge boards.

  Features:
  - Seamless WiFi connectivity
  - REST-based data transmission to the Consentium IoT Cloud
  - Deep sleep mode for power savings

  Hardware Setup:
  - Connect an LED to GPIO 16 (ESP8266), GPIO 23 (ESP32), or GPIO 25 (Raspberry Pi Pico W) 
    to indicate REST events.

  Tutorials and Documentation:
  Visit us at: https://docs.consentiumiot.com/code-usage/consentiumthings-arduino-api

  For Support:
  Email: official@consentiumiot.com

  MIT license - Redistribution must include this header.
 ****************************************************/


#include <ConsentiumThings.h>

// Initialize the board instance with firmware version
ConsentiumThingsDalton board;

// Define WiFi credentials
const char *ssid = "YOUR_WIFI_SSID";      // WiFi SSID
const char *pass = "YOUR_WIFI_PASSWORD";  // WiFi password

// Define API keys
const char *SendApiKey = "YOUR_API_KEY";      // API key for sending data
const char *BoardApiKey = "YOUR_BOARD_API_KEY"; // API key for the board

// Define the interval for data sending
constexpr int interval = 5000;  // Data transmission interval (5 seconds for free tier)

void setup() {
  // Start serial communication for debugging
  Serial.begin(115200);

  // Consentium IoT branding message
  Serial.println("Consentium IoT - Edge Board Library");
  Serial.println("------------------------------------");
  Serial.println("Initializing ConsentiumThings Board...");

  // Connect to WiFi
  board.connectWiFi(ssid, pass);
  
  // Enable secure telemetry transmission
  board.enableSend(SendApiKey, BoardApiKey);

  Serial.println("ConsentiumThings Board Initialized!");
  Serial.println("------------------------------------");
  Serial.println();

  // Prepare sample sensor data
  double data_0 = 1.0;  // Sample temperature data
  
  vector<double> sensorValues = {data_0};
  const char* sensorInfo[] = {"Temperature"};

  // Sensor info with units can be defined as
  // const char* sensorInfo[] = {"Temperature/C"};

  // Send data to cloud
  board.pushData(sensorValues, sensorInfo, LOW_PRE);

  // Put device into deep sleep for 60 seconds
  board.sleep(60000);
}

void loop() {
  // Device will not reach here during deep sleep
}
