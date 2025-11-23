#include "ConsentiumThingsDalton.h"
#include "certs/ServerCertificates.h"
#include "utils/ConsentiumEssentials.h"

#if defined(ESP8266)
  X509List cert(consentium_root_ca);
#endif

void timeSync(){
    configTime(5.5 * 3600, 0, "time.google.com", "time.windows.com");
    Serial.println(F("Waiting for NTP time sync"));
    time_t now = time(nullptr);
    while (now < NTP_SYNC_WAIT) {
      delay(500);
      now = time(nullptr);
    }
    struct tm timeinfo;
    gmtime_r(&now, &timeinfo); 
}

inline void blinkLED() {
    static bool ledState = false;
    digitalWrite(ledPin, ledState);
    ledState = !ledState;
}

char randomChar(uint8_t num) {
  if (num < 26) return 'A' + num;
  if (num < 52) return 'a' + (num - 26);
  return '0' + (num - 52);
}

inline void deviceMAC(char *macAddr) {
  #if defined(ESP32) || defined(ARDUINO_RASPBERRY_PI_PICO_W)
    uint8_t baseMac[6];
    esp_err_t ret = esp_wifi_get_mac(WIFI_IF_STA, baseMac);
    
    if (ret == ESP_OK) {
      sprintf(macAddr, "%02X:%02X:%02X:%02X:%02X:%02X", 
              baseMac[0], baseMac[1], baseMac[2], 
              baseMac[3], baseMac[4], baseMac[5]);
    } else {
      strcpy(macAddr, "00:00:00:00:00:00");  // Error fallback
    }
  #elif defined(ESP8266)
    sprintf(macAddr, WiFi.macAddress().c_str());
  #endif
}

ConsentiumThingsDalton::ConsentiumThingsDalton() : firmwareVersion("0.0") {} // Default constructor without firmware version
ConsentiumThingsDalton::ConsentiumThingsDalton(const char* firmware_version) : firmwareVersion(firmware_version) {} //Constructor when firmware version is passed

void ConsentiumThingsDalton::startSensing(){
  Wire.begin(5, 6);
  
  delay(I2C_DELAY);

  if (!ads_1.begin(currentADCAddr)) {
    Serial.println("Failed to initialize current ADC at 0x48");
  }
  if (!ads_2.begin(voltageADCAddr)) {
    Serial.println("Failed to initialize voltage ADC at 0x49");
  }
}

double ConsentiumThingsDalton::readCurrentBus(int cpin){
  int16_t raw = ads_1.readADC_SingleEnded(cpin);
  return ads_1.computeVolts(raw) * 1000 / 240.0; // Convert to mA assuming 240E resistor
}

double ConsentiumThingsDalton::readVoltageBus(int vpin){
  int16_t raw = ads_2.readADC_SingleEnded(vpin);
  return ads_2.computeVolts(raw) * 2; // Account for voltage divider
}

void ConsentiumThingsDalton::enableBatteryMonitoring(int pin, float refVoltage, float dividerRatio) {
  this->batteryMonitoringEnabled = true;
  this->batteryPin = pin;
  this->refVoltage = refVoltage;
  this->dividerRatio = dividerRatio;
  pinMode(batteryPin, INPUT);
  Serial.println(F("[ConsentiumThings] Battery monitoring enabled."));
}

void ConsentiumThingsDalton::disableBatteryMonitoring() {
  this->batteryMonitoringEnabled = false;
  Serial.println(F("[ConsentiumThings] Battery monitoring disabled."));
}

double ConsentiumThingsDalton::getBatteryVoltage() {
  if (!batteryMonitoringEnabled) return -1.0;

  #if defined(ESP32)
    float raw = 0.0;
    for (int i = 0; i < 10; i++) {
      raw += analogReadMilliVolts(batteryPin);
      delay(2);
    }
    float batteryVoltage = this->dividerRatio * raw / 10 / 1000;  // Average and scale in V
  #else
    double batteryVoltage = 0.0;
  #endif

  return batteryVoltage;
}

void ConsentiumThingsDalton::connectWiFi(const char* ssid, const char* password) {
  WiFi.mode(WIFI_STA);
 
  deviceMAC(macAddr);
  
  Serial.print(F("Attempting to connect SSID: "));
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(WIFI_DELAY);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print(F("Got IP : "));
  Serial.println(WiFi.localIP());
}

void ConsentiumThingsDalton::smartConnect() {
    wm.setDebugOutput(false); // Disable debug output

    char apName[30];
    char apPassword[30];

    sprintf(apName, "ConsentiumIoT_AP_%c%c%c", randomChar(random(62)), randomChar(random(62)), randomChar(random(62)));
    sprintf(apPassword, "consentium%c%c%c", randomChar(random(62)), randomChar(random(62)), randomChar(random(62)));

    Serial.println(F("\n========== WiFi Auto-Connect =========="));
    Serial.println(F("Starting WiFi access point setup..."));
    Serial.println(F("Use the following credentials to connect:"));
    
    Serial.println(F("---------------------------------------"));
    Serial.print(F("SSID:     ")); Serial.println(apName);
    Serial.print(F("Password: ")); Serial.println(apPassword);
    Serial.println(F("---------------------------------------"));

    // Attempt auto-connect with specified AP name and password
    bool res = wm.autoConnect(apName, apPassword);

    deviceMAC(macAddr);

    // Check if connection was successful
    if (!res) {
        Serial.println(F("Failed to connect to WiFi. Restarting..."));
        delay(3000);
        ESP.restart(); // Restart the device to retry
    } else {
        Serial.println(F("Connected to WiFi successfully!"));
        Serial.print(F("Device IP: "));
        Serial.println(WiFi.localIP());
        Serial.println(" ");
    }
}

// Function for sending URL
void ConsentiumThingsDalton::enableSend(const char* key, const char* board_id) {
  pinMode(ledPin, OUTPUT);
    
  #if defined(ESP32) || defined(ARDUINO_RASPBERRY_PI_PICO_W)
    client.setCACert(consentium_root_ca);
  #elif defined(ESP8266)
    timeSync();
    client.setTrustAnchors(&cert);
  #endif
  
  // create the send URL
  sendUrl = String(send_url);
  sendUrl.reserve(ARRAY_RESERVE);
  sendUrl.concat("sendKey=");
  sendUrl.concat(String(key));
  sendUrl.concat("&boardKey=");
  sendUrl.concat(String(board_id));
}

// Function for receiving URL
void ConsentiumThingsDalton::enableReceive(const char* key, const char* board_id) {
  pinMode(ledPin, OUTPUT);
    
  #if defined(ESP32) || defined(ARDUINO_RASPBERRY_PI_PICO_W)
    client.setCACert(consentium_root_ca);
  #elif defined(ESP8266)
    timeSync();
    client.setTrustAnchors(&cert);
  #endif
  
  // create the receive URL
  receiveUrl = String(receive_url);
  receiveUrl.reserve(ARRAY_RESERVE);
  receiveUrl.concat("recents=");
  receiveUrl.concat("true");
  receiveUrl.concat("&receiveKey=");
  receiveUrl.concat(String(key));
  receiveUrl.concat("&boardKey=");
  receiveUrl.concat(String(board_id));
}

// Function for OTA receiving URL
void ConsentiumThingsDalton::enableAirUpdate(const char* key, const char* board_id) {
  pinMode(ledPin, OUTPUT);
    
  #if defined(ESP32) || defined(ARDUINO_RASPBERRY_PI_PICO_W)
    client.setCACert(consentium_root_ca);
  #elif defined(ESP8266)
    timeSync();
    client.setTrustAnchors(&cert);
  #endif

  otaFlag = true;
  
  // create the firmware version URL
  versionUrl = String(versionURL);
  versionUrl.reserve(ARRAY_RESERVE);
  versionUrl.concat("receiveKey=");
  versionUrl.concat(String(key));
  versionUrl.concat("&boardKey=");
  versionUrl.concat(String(board_id));

  // create the firmware download URL
  firmwareUrl = String(firmwareURL);
  firmwareUrl.reserve(ARRAY_RESERVE);
  firmwareUrl.concat("receiveKey=");
  firmwareUrl.concat(String(key));
  firmwareUrl.concat("&boardKey=");
  firmwareUrl.concat(String(board_id));
}

const char* ConsentiumThingsDalton::getRemoteFirmwareVersion() {
  http.begin(client, versionUrl);
  //Serial.println(versionUrl); //Debug
  int httpCode = http.GET();
  static char versionBuffer[128]; // Assuming version won't exceed 128 characters
  if (httpCode == 200) {
    http.getString().toCharArray(versionBuffer, sizeof(versionBuffer));
    http.end(); 
    return versionBuffer;
  } else {
    http.end();
    return nullptr; // Return nullptr on error
  }
}

void ConsentiumThingsDalton::pushData(vector<double> sensor_data, const char* sensor_info[], int precision) {
  int sensor_num = sensor_data.size();

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println(F("WiFi not connected. Cannot send REST request."));
    return;
  }

  long rssi = WiFi.RSSI();
  float batteryVoltage = getBatteryVoltage();

  char name[50];
  char unit[50];

  JsonDocument jsonDocument;

  // Create a JSON array for sensor data 
  JsonArray sensorDataArray = jsonDocument["sensors"].createNestedArray("sensorData");

  // Add sensor data dynamically
  for (int i = 0; i < sensor_num; i++) {
    // Create a JSON object for the current sensor data
    JsonObject sensorData = sensorDataArray.createNestedObject();
    sensorData["info"] = sensor_info[i];
    sensorData["data"] = String(sensor_data[i], precision);
  }
  
  // Create a JSON object for board information
  JsonObject boardInfo = jsonDocument.createNestedObject("boardInfo");
  boardInfo["firmwareVersion"] = firmwareVersion;
  boardInfo["architecture"] = BOARD_TYPE;
  boardInfo["deviceMAC"] = String(macAddr);
  boardInfo["statusOTA"] = otaFlag;
  boardInfo["signalStrength"] = rssi;
  if (batteryMonitoringEnabled){
    boardInfo["batteryStrength"] = batteryVoltage;
  }
  else{
    boardInfo["batteryStrength"] = "NA";
  }
  
  // Serialize the JSON document to a string
  String jsonString;
  serializeJsonPretty(jsonDocument, jsonString);

  // Print the JSON document to the Serial
  // serializeJsonPretty(jsonDocument, Serial);

  http.begin(client, sendUrl);

  // Set the content type header to application/json
  http.addHeader("Content-Type", "application/json");

  // Make the POST request
  int httpCode = http.sendRequest("POST", jsonString);
 
  // Check for errors
  if (httpCode > 0) {
    if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_CREATED) {
      // Print a more human-readable message
      Serial.println(" ");
      Serial.println("Data successfully sent to the server!");
      // Serial.println("Here are the details:");

      Serial.println("Sensor Data:");
      for (int i = 0; i < sensor_num; i++) {
        const char* info = sensor_info[i];    // pick the i-th string
        const char* slash = strchr(info, '/');
        if (slash != nullptr) {
          size_t len = slash - info;
          strncpy(name, info, len);
          name[len] = '\0';
          strcpy(unit, slash + 1);
        } else {
          strcpy(name, info);
          unit[0] = '\0';   // empty unit
        }
        Serial.println(" - " + String(name) + ": " + String(sensor_data[i], precision) + " " + String(unit));
      }

      Serial.println("Board Information:");
      Serial.println(" - Architecture: " + String(BOARD_TYPE));
      Serial.println(" - Device MAC: " + String(macAddr));
      Serial.println(" - OTA enabled: " + String(otaFlag ? "Yes" : "No"));
      Serial.println(" - Signal: " + String(rssi) + " dBm");
      if (batteryMonitoringEnabled){
        Serial.println(" - Battery: " + String(batteryVoltage) + " V");
      }
      
      Serial.println(" ");
      blinkLED();
    }
    else if(httpCode == 422){
      Serial.print("Received 422 response:");
      String response = http.getString();
      // Create a small JSON document
      StaticJsonDocument<128> errorDoc;

      // Deserialize the JSON response
      DeserializationError error = deserializeJson(errorDoc, response);
      const char* message = errorDoc["error"];
      Serial.println(message);

      return ;
    }
  } else if (httpCode == 429) {
      String response = http.getString();
      http.end();
      Serial.print("Response: ");
      StaticJsonDocument<128> errorDoc;
      if (deserializeJson(errorDoc, response) == DeserializationError::Ok) {
          Serial.println(errorDoc["message"].as<const char*>());
      } else {
          Serial.println("Could not parse error message.");
      }
      return;
    } else {
    Serial.println(F("HTTP POST request failed."));

    return ;
  }
  // Close the HTTP connection
  http.end();
}

vector<pair<double, String>> ConsentiumThingsDalton::pullData() {
  vector<pair<double, String>> result;

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println(F("WiFi not connected. Cannot send REST request."));
    return result;
  }

  JsonDocument jsonDocument;
  http.begin(client, receiveUrl);

  int httpCode = http.sendRequest("GET");

  if (httpCode > 0) {
    if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_CREATED) {
      String payload = http.getString();

      DeserializationError error = deserializeJson(jsonDocument, payload);
      if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        http.end();
        return result;
      }

      JsonObject board = jsonDocument["board"];
      JsonArray feeds = jsonDocument["feeds"];

      // Iterate over each feed entry
      for (JsonObject feed : feeds) {
        // Up to 8 possible sensors
        for (int i = 1; i <= 8; i++) {
          String infoKey = "info" + String(i);
          String valueKey = "value" + String(i);

          if (board.containsKey(infoKey) && feed.containsKey(valueKey)) {
            const char* info = board[infoKey];
            double value = feed[valueKey].as<double>();
            result.push_back(std::make_pair(value, String(info)));
          }
        }
      }

      blinkLED();
    }
  } else {
    Serial.println(F("HTTP GET request failed."));
  }

  http.end();
  return result;
}

// void ConsentiumThingsDalton::checkAndPerformUpdate() {
//     if(otaFlag == false) {
//         Serial.println(F("Error: OTA is not enabled, call beginOTA() to enable OTA."));
//         return;
//     }

//     const char* remoteVersion = getRemoteFirmwareVersion();
//     if (!remoteVersion || !firmwareUrl) {
//         Serial.println(F("Error: Firmware version or URL is null."));
//         return;
//     }

//     Serial.println("Firmware Information:");
//     Serial.println(" - Remote version: " + String(remoteVersion));
//     Serial.println(" - Device version: " + String(firmwareVersion));

//     if (strcmp(remoteVersion, firmwareVersion) > 0) {
//         Serial.println(F(" - Update available."));
//         Serial.println(" ");

//         #if defined(ESP32) || defined(ARDUINO_RASPBERRY_PI_PICO_W)
//             httpUpdate.rebootOnUpdate(true);
//             t_httpUpdate_return ret = httpUpdate.update(client, firmwareUrl);
//         #elif defined(ESP8266)
//             ESPhttpUpdate.rebootOnUpdate(true);
//             t_httpUpdate_return ret = ESPhttpUpdate.update(client, firmwareUrl);
//         #endif

//         switch (ret) {
//             case HTTP_UPDATE_FAILED:
//               #if defined(ESP32) || defined(ARDUINO_RASPBERRY_PI_PICO_W)
//                 Serial.printf("HTTP_UPDATE_FAILED Error (%d): %s\n",
//                   httpUpdate.getLastError(),
//                   httpUpdate.getLastErrorString().c_str());
//               #elif defined(ESP8266)
//                 Serial.printf("HTTP_UPDATE_FAILED Error (%d): %s\n",
//                   ESPhttpUpdate.getLastError(),
//                   ESPhttpUpdate.getLastErrorString().c_str());
//               #endif
//                 Serial.println(F("Retry in 10 secs!"));
//                 delay(10000); // Replace with a non-blocking delay
//                 break;
//             case HTTP_UPDATE_OK:
//                 Serial.println(F("Update downloaded."));
//                 delay(1000);
//                 Serial.println(F("Restarting!"));
//                 ESP.restart(); // Ensure restart is intended
//                 break;
//         }
//     } else {
//         Serial.println(" - No update.");
//         Serial.println(" ");
//     }
// }

void ConsentiumThingsDalton::setSyncInterval(int interval) {
  if (interval > 0) {
    airSyncInterval = interval;
    Serial.println("[Consentium IoT] Air sync interval set to every " + String(interval) + " data pushes.");
  } else {
    Serial.println("[Consentium IoT] Error: Air sync interval must be greater than 0.");
  }
}

void ConsentiumThingsDalton::airSync(vector<double> sensor_data, const char* sensor_info[], int precision) {
  // Part 1: Send Sensor Data (runs on every call)
  int sensor_num = sensor_data.size();

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println(F("WiFi not connected. Cannot sync with server."));
    return;
  }

  // --- Construct the JSON payload ---
  long rssi = WiFi.RSSI();
  float batteryVoltage = getBatteryVoltage();

  char name[50];
  char unit[50];

  JsonDocument jsonDocument;
  JsonArray sensorDataArray = jsonDocument["sensors"].createNestedArray("sensorData");

  for (int i = 0; i < sensor_num; i++) {
    JsonObject sensorData = sensorDataArray.createNestedObject();
    sensorData["info"] = sensor_info[i];
    sensorData["data"] = String(sensor_data[i], precision);
  }
  
  JsonObject boardInfo = jsonDocument.createNestedObject("boardInfo");
  boardInfo["firmwareVersion"] = firmwareVersion;
  boardInfo["architecture"] = BOARD_TYPE;
  boardInfo["deviceMAC"] = String(macAddr);
  boardInfo["statusOTA"] = otaFlag;
  boardInfo["signalStrength"] = rssi;
  boardInfo["batteryStrength"] = batteryVoltage;
  
  String jsonString;
  serializeJsonPretty(jsonDocument, jsonString);

  // --- Send the HTTP POST request ---
  http.begin(client, sendUrl);
  http.addHeader("Content-Type", "application/json");
  int httpCode = http.sendRequest("POST", jsonString);
 
  // --- Process the response ---
  if (httpCode > 0) {
    if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_CREATED) {
      // Print a more human-readable message
      Serial.println(" ");
      Serial.println("Data successfully sent to the server!");
      // Serial.println("Here are the details:");
      
      Serial.println("Sensor Data:");
      for (int i = 0; i < sensor_num; i++) {
        const char* info = sensor_info[i];    // pick the i-th string
        const char* slash = strchr(info, '/');
        if (slash != nullptr) {
          size_t len = slash - info;
          strncpy(name, info, len);
          name[len] = '\0';
          strcpy(unit, slash + 1);
        } else {
          strcpy(name, info);
          unit[0] = '\0';   // empty unit
        }
        Serial.println(" - " + String(name) + ": " + String(sensor_data[i], precision) + " " + String(unit));
      }

      Serial.println("Board Information:");
      Serial.println(" - Firmware Version: " + String(firmwareVersion));
      Serial.println(" - Architecture: " + String(BOARD_TYPE));
      Serial.println(" - Device MAC: " + String(macAddr));
      Serial.println(" - OTA enabled: " + String(otaFlag ? "Yes" : "No"));
      Serial.println(" - Signal: " + String(rssi) + " dBm");
      Serial.println(" - Battery: " + String(batteryVoltage) + " V");
      Serial.println(" ");
      
      // Increment counter on successful push and log it
      pushCounter++;
      blinkLED();

    } else if (httpCode == 422) {
      String response = http.getString();
      http.end();
      Serial.print("Response:");
      StaticJsonDocument<128> errorDoc;
      if (deserializeJson(errorDoc, response) == DeserializationError::Ok) {
          Serial.println(errorDoc["message"].as<const char*>());
      } else {
          Serial.println("Could not parse error message.");
      }
      return;
    } else if (httpCode == 429) {
      String response = http.getString();
      http.end();
      Serial.print("Response: ");
      StaticJsonDocument<128> errorDoc;
      if (deserializeJson(errorDoc, response) == DeserializationError::Ok) {
          Serial.println(errorDoc["message"].as<const char*>());
      } else {
          Serial.println("Could not parse error message.");
      }
      return;
    } else {
        http.end();
        Serial.printf("[HTTP] POST... failed, error: %d\n", httpCode);
        return;
    }
  } else {
    http.end();
    Serial.println(F("HTTP POST request failed entirely."));
    return;
  }

  http.end();

  // Part 2: Check for Firmware Update (runs periodically)

  // Only proceed if the push counter has reached the desired interval
  if (pushCounter < airSyncInterval) {
    return; // Not time to check yet, so we're done for this cycle.
  }
  
  // If we've reached here, it's time to check. Reset counter for the next cycle.
  pushCounter = 0;
  
  Serial.println("----------------------------------------");
  Serial.println("Update interval reached. Checking for firmware updates.");

  if(otaFlag == false) {
    Serial.println(F("OTA is not enabled, skipping update check."));
    return;
  }

  const char* remoteVersion = getRemoteFirmwareVersion();
  if (!remoteVersion || !firmwareUrl) {
    Serial.println(F("Error: Firmware version or URL is null. Cannot check for updates."));
    return;
  }

  Serial.println("Firmware Information:");
  Serial.println(" - Remote version: " + String(remoteVersion));
  Serial.println(" - Device version: " + String(firmwareVersion));

  if (strcmp(remoteVersion, firmwareVersion) > 0) {
    Serial.println(F(" - Update available. Starting OTA update..."));
    Serial.println(" ");

    #if defined(ESP32) || defined(ARDUINO_RASPBERRY_PI_PICO_W)
        httpUpdate.rebootOnUpdate(true);
        t_httpUpdate_return ret = httpUpdate.update(client, firmwareUrl);
    #elif defined(ESP8266)
        ESPhttpUpdate.rebootOnUpdate(true);
        t_httpUpdate_return ret = ESPhttpUpdate.update(client, firmwareUrl);
    #endif

    if (ret == HTTP_UPDATE_FAILED) {
      #if defined(ESP32) || defined(ARDUINO_RASPBERRY_PI_PICO_W)
        Serial.printf("HTTP_UPDATE_FAILED Error (%d): %s\n",
          httpUpdate.getLastError(),
          httpUpdate.getLastErrorString().c_str());
      #elif defined(ESP8266)
        Serial.printf("HTTP_UPDATE_FAILED Error (%d): %s\n",
          ESPhttpUpdate.getLastError(),
          ESPhttpUpdate.getLastErrorString().c_str());
      #endif
    }
  } else {
      Serial.println(" - No new update available.");
      Serial.println(" ");
  }
}
