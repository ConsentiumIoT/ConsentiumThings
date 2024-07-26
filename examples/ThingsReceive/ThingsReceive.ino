/***************************************************
  This is Consentium's IoT library
  ----> https://consentiuminc.online/
  Check out the links above for our tutorials and product diagrams.

  This Consentium's IoT library works only for ESP8266/ESP32/Raspberry Pi Pico W compatible Edge boards. 
  
  Connect a LED to GPIO 16 for ESP 8266, GPIO 23 for ESP 32 and GPIO 25 for Raspberry Pi Pico W to indicate the REST events. 
  
  Written by Debjyoti Chowdhury for Consentium.
  MIT license, all text above must be included in any redistribution
 ****************************************************/


#include <ConsentiumThings.h>

ConsentiumThings board; // Create ConsentiumThings object

const char *ssid = "YOUR_WIFI_SSID"; // Add WiFi SSID
const char *pass = "YOUR_WIFI_PASSWORD"; // Add WiFi password
constexpr int interval = 7000; // Wait for 7 seconds
const char *ReceiveApiKey = "YOUR_API_KEY"; // Receive API key
const char *BoardApiKey = "YOUR_BOARD_API_KEY"; // Board API key

void setup() {
  board.beginReceive(ReceiveApiKey, BoardApiKey); // Initialize IoT board to receive data
  board.initWiFi(ssid, pass); // Begin WiFi connection
}

void loop(){
  // get sensor data from server
  auto dataPairs = board.receiveREST(); 
  
  //print sensor data from server
  for(size_t i=0; i<dataPairs.size(); i++){
    double data = dataPairs[i].first; 
    String info = dataPairs[i].second.c_str();

    Serial.print("Data: ");
    Serial.print(data);
    Serial.print("          \t");
    Serial.print("Info: ");
    Serial.println(info);
  }
  Serial.println();

  delay(interval);
}
