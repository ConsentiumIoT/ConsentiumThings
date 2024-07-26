/***************************************************
  This is Consentium Inc's IoT library
  ----> http://consentiuminc.online/
  Check out the links above for our tutorials and product diagrams.

  This Consentium Inc's IoT library works only for 5v tolerant Edge Beta boards. 
 
  Written by Debjyoti Chowdhury for Consentium Inc.
  MIT license, all text above must be included in any redistribution
 ****************************************************/


#include <ConsentiumThings.h>

ConsentiumThingsBeta board;   // create ConsentiumThing beta object

const char *ssid = "YOUR_WIFI_SSID"; // Add WiFi SSID
const char *pass = "YOUR_WIFI_PASSWORD"; // Add WiFi password
constexpr int interval = 7000; // Wait for 7 seconds
const char *SendApiKey = "YOUR_API_KEY"; // Send API key
const char *BoardApiKey = "YOUR_BOARD_API_KEY"; // Board API key

void setup(){
  board.beginSend(SendApiKey, BoardApiKey);   // init. IoT boad
  board.initWiFi(ssid, pass);  // begin WiFi connection
}

void loop(){
  float data_0 = analogRead(IN_4_20_1)*SCALE; // Read 4-20mA sensor data form J1 port 1
  float data_1 = analogRead(IN_0_10_1)*SCALE; // Read 0-10V sensor data form J2 port 1

  float sensorValues[] = {data_0, data_1};  // sensor data array
  const char* sensorInfo[] = {"Temperature", "Pressure"}; // sensor info. array
  
  int sensorCount = sizeof(sensorValues)/sizeof(sensorValues[0]); // number of sensors connected 
  
  board.sendREST(sensorValues, sensorInfo, sensorCount, LOW_PRE); // send over REST with delay

  delay(interval);
}
