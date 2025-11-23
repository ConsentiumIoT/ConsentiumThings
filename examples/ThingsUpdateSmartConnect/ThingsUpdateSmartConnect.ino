/***************************************************
  Consentium IoT - Edge Board Library
  -------------------------------------------------
  This library is designed for use with Consentium IoT's 
  ESP8266, ESP32, and Raspberry Pi Pico W-compatible edge boards.

  Features:
  - Seamless WiFi connectivity
  - REST-based data transmission to the Consentium IoT Cloud

  Hardware Setup:
  - Connect an LED to GPIO 16 (ESP8266), GPIO 23 (ESP32), or GPIO 25 (Raspberry Pi Pico W) 
    to indicate REST events.

  Tutorials and Documentation:
  Visit us at: http://docs.consentiumiot.com/code-usage/wifi-auto-connect-api

  For Support:
  Email: official@consentiumiot.com

  MIT license - Redistribution must include this header.
 ****************************************************/

#include <ConsentiumThings.h>

// Create ConsentiumThings object
ConsentiumThingsDalton board;

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

  // Initialize WiFi with auto-connect feature
  board.smartConnect();

  // Initialize the board for sending data
  board.enableSend(SendApiKey, BoardApiKey);

  // Enable battery monitoring on ADC_IN pin (optional)
  // board.enableBatteryMonitoring(ADC_IN);

  // Enable current and voltage industrial monitoring (optional)
  // board.startSensing();
  

  Serial.println("ConsentiumThings Board Initialized!");
  Serial.println("------------------------------------");
  Serial.println();
}

void loop() {
  // Prepare sample sensor data
  double data_0 = 1.0;  // Sample temperature data

  // Get data from 4-20mA current sensor, returns as mA (optional)
  // double cin_1_current = board.readCurrentBus(CIN_1);
  // double cin_2_current = board.readCurrentBus(CIN_2);
  // double cin_3_current = board.readCurrentBus(CIN_3);
  // double cin_4_current = board.readCurrentBus(CIN_4);

  // Get data from 0-10v voltage sensor, returns as volts (optional)
  // double vin_1_current = board.readVoltageBus(VIN_1);
  // double vin_2_current = board.readVoltageBus(VIN_2);
  // double vin_3_current = board.readVoltageBus(VIN_3);
  // double vin_4_current = board.readVoltageBus(VIN_4);

  vector<double> sensorValues = {data_0};  // Sensor data vector
  
  // Sensor info without units can be defined as
  const char* sensorInfo[] = {"Temperature"}; 

  // Sensor info with units can be defined as
  // const char* sensorInfo[] = {"Temperature/C"};

  board.pushData(sensorValues, sensorInfo, LOW_PRE);  // Send over REST with low precision

  // Wait before sending the next batch of data
  delay(interval);
}
