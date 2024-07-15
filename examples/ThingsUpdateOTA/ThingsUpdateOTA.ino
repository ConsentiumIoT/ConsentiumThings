/***************************************************
  This is Consentium's IoT library
  ----> https://consentiuminc.online/
  Check out the links above for our tutorials and product diagrams.

  This Consentium's IoT library works only for ESP8266/ESP32/Raspberry Pi Pico W compatible Edge boards. 
  
  Connect a LED to GPIO 16 for ESP 8266, GPIO 23 for ESP 32 and GPIO 25 for Raspberry Pi Pico W to indicate the REST events. 
  
  Connect analog devices to A0 for ESP 8266, GPIO 34 for ESP 32 and GPIO 26 for Raspberry Pi Pico W using alias ADC_IN. 

  Written by Debjyoti Chowdhury for Consentium.
  MIT license, all text above must be included in any redistribution
 ****************************************************/


#include <ConsentiumThingsDalton.h>

#define FIRMWARE_VERSION "0.1"

ConsentiumThings board(FIRMWARE_VERSION); // Create ConsentiumThings object with firmware version

const char *ssid = "YOUR_WIFI_SSID"; // Add WiFi SSID
const char *pass = "YOUR_WIFI_PASSWORD"; // Add WiFi password
constexpr int interval = 7000; // Wait for 7 seconds
const char *SendApiKey = "YOUR_API_KEY"; // Send API key
const char *ReceiveApiKey = "YOUR_API_KEY"; // Receive API key
const char *BoardApiKey = "YOUR_BOARD_API_KEY"; // Board API key
const int updateInterval = 100; // Check for update after 100 cycles

int loopCounter = 0; // Counter to keep track of loop cycles

void setup() {
  board.beginSend(SendApiKey, BoardApiKey); // Initialize IoT board
  board.beginOTA(ReceiveApiKey, BoardApiKey); // Enable OTA updates
  board.initWiFi(ssid, pass); // Begin WiFi connection
}

void loop(){
  double data_0 = board.busRead(0);  // read voltage data
  
  double sensorValues[] = {data_0};  // sensor data array
  const char* sensorInfo[] = {"Temperature"}; // sensor info. array
  
  int sensorCount = sizeof(sensorValues)/sizeof(sensorValues[0]); // number of sensors connected 
  
  board.sendREST(sensorValues, sensorInfo, sensorCount, LOW_PRE); // send over REST with delay with desired prescision

  loopCounter++;
  if (loopCounter >= updateInterval) {
    board.checkAndPerformUpdate();
    loopCounter = 0; // Reset the counter after checking for an update
  }

  delay(interval);
}
