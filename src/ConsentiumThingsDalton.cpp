#include "ConsentiumThingsDalton.h"

#if defined(ESP32) || defined(ARDUINO_RASPBERRY_PI_PICO_W) || defined(ESP8266)

#include "certs/ServerCertificates.h"
#include "utils/ConsentiumEssentials.h"

#if defined(ESP8266)
  X509List cert(consentium_root_ca);
#endif

bool otaFlag = false;
char macAddr[18];

void syncTime(){
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

void toggleLED() {
    static bool ledState = false;
    digitalWrite(ledPin, ledState);
    ledState = !ledState;
}

char random_char(uint8_t num) {
  if (num < 26){
    return 'A' + num;        // A-Z
  } 
  else if (num < 52) {
    return 'a' + (num - 26); // a-z
  }
  else {
    return '0' + (num - 52); // 0-9
  }
}

void readMacAddress(char *macAddr) {
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

ConsentiumThingsDalton::ConsentiumThingsDalton() : firmwareVersion("0.0") {Serial.begin(ESPBAUD);} // Default constructor without firmware version
ConsentiumThingsDalton::ConsentiumThingsDalton(const char* firmware_version) : firmwareVersion(firmware_version) {Serial.begin(ESPBAUD);} //Constructor when firmware version is passed

void ConsentiumThingsDalton::beginSense(){
  delay(I2C_DELAY);

  if (!ads_1.begin(currentADCAddr)) {
    Serial.println("Failed to initialize current ADC at 0x48");
  }
  if (!ads_2.begin(voltageADCAddr)) {
    Serial.println("Failed to initialize voltage ADC at 0x49");
  }
}

double ConsentiumThingsDalton::readCurrentBus(int cpin){
  return ads_1.readADC_SingleEnded(cpin)*multiplier;
}

double ConsentiumThingsDalton::readVoltageBus(int vpin){
  return ads_2.readADC_SingleEnded(vpin)*multiplier;
}

void ConsentiumThingsDalton::initWiFi(const char* ssid, const char* password) {
  WiFi.mode(WIFI_STA);
 
  readMacAddress(macAddr);
  
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

void ConsentiumThingsDalton::initWiFiAutoConnect() {
    wm.setDebugOutput(false); // Disable debug output

    char apName[30];
    char apPassword[30];

    sprintf(apName, "ConsentiumIoT_AP_%c%c%c", random_char(random(62)), random_char(random(62)), random_char(random(62)));
    sprintf(apPassword, "consentium%c%c%c", random_char(random(62)), random_char(random(62)), random_char(random(62)));

    Serial.println(F("\n========== WiFi Auto-Connect =========="));
    Serial.println(F("Starting WiFi access point setup..."));
    Serial.println(F("Use the following credentials to connect:"));
    
    Serial.println(F("---------------------------------------"));
    Serial.print(F("SSID:     ")); Serial.println(apName);
    Serial.print(F("Password: ")); Serial.println(apPassword);
    Serial.println(F("---------------------------------------"));

    // Attempt auto-connect with specified AP name and password
    bool res = wm.autoConnect(apName, apPassword);

    readMacAddress(macAddr);

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
void ConsentiumThingsDalton::beginSend(const char* key, const char* board_id) {
  pinMode(ledPin, OUTPUT);
    
  #if defined(ESP32) || defined(ARDUINO_RASPBERRY_PI_PICO_W)
    client.setCACert(consentium_root_ca);
  #elif defined(ESP8266)
    syncTime();
    client.setTrustAnchors(&cert);
  #endif
  
  // create the send URL
  sendUrl = String(send_url);
  sendUrl.reserve(ARRAY_RESERVE);
  sendUrl.concat("key=");
  sendUrl.concat(String(key));
  sendUrl.concat("&boardkey=");
  sendUrl.concat(String(board_id));
}

// Function for receiving URL
void ConsentiumThingsDalton::beginReceive(const char* key, const char* board_id) {
  pinMode(ledPin, OUTPUT);
    
  #if defined(ESP32) || defined(ARDUINO_RASPBERRY_PI_PICO_W)
    client.setCACert(consentium_root_ca);
  #elif defined(ESP8266)
    syncTime();
    client.setTrustAnchors(&cert);
  #endif
  
  // create the receive URL
  receiveUrl = String(receive_url);
  receiveUrl.reserve(ARRAY_RESERVE);
  receiveUrl.concat("receivekey=");
  receiveUrl.concat(String(key));
  receiveUrl.concat("&boardkey=");
  receiveUrl.concat(String(board_id));
}

// Function for OTA receiving URL
void ConsentiumThingsDalton::beginOTA(const char* key, const char* board_id) {
  pinMode(ledPin, OUTPUT);
    
  #if defined(ESP32) || defined(ARDUINO_RASPBERRY_PI_PICO_W)
    client.setCACert(consentium_root_ca);
  #elif defined(ESP8266)
    syncTime();
    client.setTrustAnchors(&cert);
  #endif

  otaFlag = true;
  
  // create the firmware version URL
  versionUrl = String(versionURL);
  versionUrl.reserve(ARRAY_RESERVE);
  versionUrl.concat("receivekey=");
  versionUrl.concat(String(key));
  versionUrl.concat("&boardkey=");
  versionUrl.concat(String(board_id));

  // create the firmware download URL
  firmwareUrl = String(firmwareURL);
  firmwareUrl.reserve(ARRAY_RESERVE);
  firmwareUrl.concat("receivekey=");
  firmwareUrl.concat(String(key));
  firmwareUrl.concat("&boardkey=");
  firmwareUrl.concat(String(board_id));
}

const char* ConsentiumThingsDalton::getRemoteFirmwareVersion() {
  http.begin(client, versionUrl);
  //Serial.println(versionUrl); //Debug
  int httpCode = http.GET();
  static char versionBuffer[128]; // Assuming version won't exceed 128 characters
  if (httpCode == 200) {
    http.getString().toCharArray(versionBuffer, sizeof(versionBuffer));
    return versionBuffer;
  } else {
    return nullptr; // Return nullptr on error
  }
}

void ConsentiumThingsDalton::sendData(vector<double> sensor_data, const char* sensor_info[], int precision) {
  int sensor_num = sensor_data.size();

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println(F("WiFi not connected. Cannot send REST request."));
    return;
  }

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
      String response = http.getString();

      // Print a more human-readable message
      Serial.println(" ");
      Serial.println("Data successfully sent to the server!");
      // Serial.println("Here are the details:");
      
      Serial.println("Sensor Data:");
      for (int i = 0; i < sensor_num; i++) {
        Serial.println(" - " + String(sensor_info[i]) + ": " + String(sensor_data[i], precision));
      }

      Serial.println("Board Information:");
      Serial.println(" - Firmware Version: " + String(firmwareVersion));
      Serial.println(" - Architecture: " + String(BOARD_TYPE));
      Serial.println(" - Device MAC: " + String(macAddr));
      Serial.println(" - OTA enabled: " + String(otaFlag ? "True" : "False"));
      Serial.println(" ");
      toggleLED();
    }
  } else {
    Serial.println(F("HTTP POST request failed."));
  }
  // Close the HTTP connection
  http.end();
}

vector<pair<double, String>> ConsentiumThingsDalton::receiveData() {
  vector<pair<double, String>> result;

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println(F("WiFi not connected. Cannot send REST request."));
    return result;
  }
  
  JsonDocument jsonDocument;

  http.begin(client, receiveUrl);

  // Set the content type header to application/json
  //http.addHeader("Content-Type", "application/json");

  // Make the GET request
  int httpCode = http.sendRequest("GET");
  
  // Check for errors
  if (httpCode > 0) {
    if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_CREATED) {
      String payload = http.getString();

      DeserializationError error = deserializeJson(jsonDocument, payload);
      // Extract info and data list
      JsonArray sensorData = jsonDocument["sensors"][0]["sensorData"];
    
      for (JsonObject obj : sensorData){
        double data = obj["data"].as<double>();
        const char* info = obj["info"];
        result.push_back(std::make_pair(data, String(info)));
      }
    toggleLED();
    }
  } 
  else {
    Serial.println(F("HTTP GET request failed."));
  }
  // Close the HTTP connection
  http.end();
  
  return result;
}

void ConsentiumThingsDalton::checkAndPerformUpdate() {
    if(otaFlag == false) {
        Serial.println(F("Error: OTA is not enabled, call beginOTA() to enable OTA."));
        return;
    }

    const char* remoteVersion = getRemoteFirmwareVersion();
    if (!remoteVersion || !firmwareUrl) {
        Serial.println(F("Error: Firmware version or URL is null."));
        return;
    }

    Serial.println("Firmware Information:");
    Serial.println(" - Remote version: " + String(remoteVersion));
    Serial.println(" - Device version: " + String(firmwareVersion));

    if (strcmp(remoteVersion, firmwareVersion) > 0) {
        Serial.println(F(" - Update available."));
        Serial.println(" ");

        #if defined(ESP32) || defined(ARDUINO_RASPBERRY_PI_PICO_W)
            httpUpdate.rebootOnUpdate(true);
            t_httpUpdate_return ret = httpUpdate.update(client, firmwareUrl);
        #elif defined(ESP8266)
            ESPhttpUpdate.rebootOnUpdate(true);
            t_httpUpdate_return ret = ESPhttpUpdate.update(client, firmwareUrl);
        #endif

        switch (ret) {
            case HTTP_UPDATE_FAILED:
              #if defined(ESP32) || defined(ARDUINO_RASPBERRY_PI_PICO_W)
                Serial.printf("HTTP_UPDATE_FAILED Error (%d): %s\n",
                  httpUpdate.getLastError(),
                  httpUpdate.getLastErrorString().c_str());
              #elif defined(ESP8266)
                Serial.printf("HTTP_UPDATE_FAILED Error (%d): %s\n",
                  ESPhttpUpdate.getLastError(),
                  ESPhttpUpdate.getLastErrorString().c_str());
              #endif
                Serial.println(F("Retry in 10 secs!"));
                delay(10000); // Replace with a non-blocking delay
                break;
            case HTTP_UPDATE_OK:
                Serial.println(F("Update downloaded."));
                delay(1000);
                Serial.println(F("Restarting!"));
                ESP.restart(); // Ensure restart is intended
                break;
        }
    } else {
        Serial.println(" - No update.");
        Serial.println(" ");
    }
}


#endif
