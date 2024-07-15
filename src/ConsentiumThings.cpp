#include <ConsentiumThings.h>
#include <certs/ServerCertificates.h>
#include <utils/ConsentiumEssentials.h>

#if defined(ESP8266)
  X509List cert(consentium_root_ca);
#endif

const int kselect_lines[SELECT_LINES] = {S_0, S_1, S_2, S_3}; // MUX select lines

const int kMUXtable[MUX_IN_LINES][SELECT_LINES] = {
  {0, 0, 0, 0}, {1, 0, 0, 0}, {0, 1, 0, 0}, {1, 1, 0, 0},
  {0, 0, 1, 0}, {1, 0, 1, 0}, {0, 1, 1, 0}, {1, 1, 1, 0},
  {0, 0, 0, 1}, {1, 0, 0, 1}, {0, 1, 0, 1}, {1, 1, 0, 1},
  {0, 0, 1, 1}, {1, 0, 1, 1}, {0, 1, 1, 1}, {1, 1, 1, 1}
};

void syncTime(){
    configTime(5.5 * 3600, 0, "time.google.com", "time.windows.com");
    Serial.println(F("Waiting for NTP time sync: "));
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

ConsentiumThingsDalton::ConsentiumThingsDalton() : firmwareVersion("0.0") {} // Default constructor without firmware version
ConsentiumThingsDalton::ConsentiumThingsDalton(const char* firmware_version) : firmwareVersion(firmware_version) {} //Constructor when firmware version is passed

// Function for sending URL
void ConsentiumThingsDalton::beginSend(const char* key, const char* board_id) {
  Serial.begin(ESPBAUD);
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

  for (int i = 0; i < SELECT_LINES; i++) {
      pinMode(kselect_lines[i], OUTPUT);
  }
}

// Function for receiving URL
void ConsentiumThingsDalton::beginReceive(const char* key, const char* board_id) {
  Serial.begin(ESPBAUD);
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
  Serial.begin(ESPBAUD);
  pinMode(ledPin, OUTPUT);
    
  #if defined(ESP32) || defined(ARDUINO_RASPBERRY_PI_PICO_W)
    client.setCACert(consentium_root_ca);
  #elif defined(ESP8266)
    syncTime();
    client.setTrustAnchors(&cert);
  #endif
  
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


void ConsentiumThingsDalton::initWiFi(const char* ssid, const char* password) {
  WiFi.mode(WIFI_STA);
  
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

float ConsentiumThingsDalton::busRead(int j) {
  for (int i = 0; i < SELECT_LINES; i++) {
    digitalWrite(kselect_lines[i], kMUXtable[j][i]);
  }
  return analogRead(ADC_IN);
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

void ConsentiumThingsDalton::sendREST(double sensor_data[], const char* sensor_info[], int sensor_num, int precision) {
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
  
  // Serialize the JSON document to a string
  String jsonString;
  serializeJsonPretty(jsonDocument, jsonString);

  http.begin(client, sendUrl);

  // Set the content type header to application/json
  http.addHeader("Content-Type", "application/json");

  // Make the POST request
  int httpCode = http.sendRequest("POST", jsonString);
  
  // Check for errors
  if (httpCode > 0) {
    if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_CREATED) {
      String response = http.getString();
      String combinedOutput = "Server response: " + response + " with code: " + String(httpCode) + "\nData packet: " + jsonString + "\n";
      Serial.println(combinedOutput);
      toggleLED();
    }
  } 
  else {
    Serial.println(F("HTTP POST request failed."));
  }
  // Close the HTTP connection
  http.end();
}

std::vector<std::pair<double, String>> ConsentiumThingsDalton::receiveREST() {
  std::vector<std::pair<double, String>> result;

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
  const char* remoteVersion = getRemoteFirmwareVersion();

  Serial.print(F("Remote version: "));
  Serial.print(remoteVersion);
  Serial.print(F(" On-device version: "));
  Serial.print(firmwareVersion);

  if (strcmp(remoteVersion, firmwareVersion) > 0) {

    Serial.println(F("-> Update available."));

    #if defined(ESP32) || defined(ARDUINO_RASPBERRY_PI_PICO_W)
      httpUpdate.rebootOnUpdate(true); // Remove automatic update
      t_httpUpdate_return ret = httpUpdate.update(client, firmwareUrl);
      switch (ret) {
        case HTTP_UPDATE_FAILED:
          Serial.printf("HTTP_UPDATE_FAILED Error (%d): %s\n", httpUpdate.getLastError(), httpUpdate.getLastErrorString().c_str());
          Serial.println(F("Retry in 10 secs!"));
          delay(10000); // Wait 10 secs before retrying
          break;
        case HTTP_UPDATE_OK:
          Serial.println(F("Updated downloaded"));
          delay(1000); // Wait a second and restart
          Serial.println(F("Restarting!"));
          //ESP.restart();
          break;
    #elif defined(ESP8266)
      ESPhttpUpdate.rebootOnUpdate(true); // Remove automatic update
      t_httpUpdate_return ret = ESPhttpUpdate.update(client, firmwareUrl);
      switch (ret) {
        case HTTP_UPDATE_FAILED:
          Serial.printf("HTTP_UPDATE_FAILED Error (%d): %s\n", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
          Serial.println(F("Retry in 10 secs!"));
          delay(10000); // Wait 10 secs before retrying
          break;
        case HTTP_UPDATE_OK:
          Serial.println(F("Updated downloaded"));
          delay(1000); // Wait a second and restart
          Serial.println(F("Restarting!"));
          //ESP.restart();
          break;
    #endif


    }
  } else {
    Serial.println("-> No update.");
  }
}

