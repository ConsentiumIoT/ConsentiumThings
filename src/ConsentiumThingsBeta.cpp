#include "ConsentiumThingsBeta.h"

#if defined(ARDUINO_AVR_PRO) || defined(ARDUINO_AVR_UNO)

ConsentiumThingsBeta::ConsentiumThingsBeta() {}

bool sendAT(const char* command, char response[], long wdelay) {
    Serial.println(command);
    delay(wdelay);
    return Serial.find(response);
}

void ConsentiumThingsBeta::beginSend(const char* key, const char* board_key) {
    this->key = key;
    this->board_key = board_key;
    Serial.begin(COMM_BAUD);
}

void ConsentiumThingsBeta::initWiFi(const char* ssid, const char* password) {
    int attempts = 0;
    bool connected = false;
    
    while (!connected && attempts < MAX_WIFI_ATTEMPTS) {
        sendAT("AT", (char*) "OK", 5000);
        sendAT("AT+CWMODE=1", (char*) "OK", 5000);

        char cmd[50];
        sprintf(cmd, "AT+CWJAP=\"%s\",\"%s\"", ssid, password);
        if (sendAT(cmd, (char*) "OK", 5000)) {
            connected = true;
            Serial.println(F("WiFi connected"));
        } else {
            Serial.println(F("WiFi not connected, retrying..."));
            attempts++;
            delay(WIFI_RETRY_DELAY);
        }
    }

    if (!connected) {
        Serial.println(F("Failed to connect to WiFi"));
    }
}


void ConsentiumThingsBeta::sendREST(float data[], const char* info[], int sensor_num, int pre) {
    const char* ip = "consentiuminc.online";
    const char* port = "443";

    // Establish TCP connection
    String tcp_str = "AT+CIPSTART=\"TCP\",\"" + String(ip) + "\"," + String(port);
    bool tcp_resp = sendAT(tcp_str.c_str(), (char*) "OK", 100);

    if (tcp_resp) {
        delay(2000);

        // Construct HTTP request body
        String requestBody = "key=" + String(key) + "&boardkey=" + String(board_key) + "&firmwareVersion=" + String(0.0) + "&architecture=ESP-01";
        for (int i = 0; i < sensor_num; ++i) {
            requestBody += "&sensor" + String(i + 1) + "=" + String(data[i], pre) + "&info" + String(i + 1) + "=" + info[i];
        }

        // Construct HTTP request header
        String requestHeader = "POST /api/board/update/ HTTP/1.1\r\n";
        requestHeader += "Host: " + String(ip) + "\r\n";
        requestHeader += "Content-Type: application/x-www-form-urlencoded\r\n";
        requestHeader += "Content-Length: " + String(requestBody.length()) + "\r\n\r\n";

        // Send HTTP request
        String httpRequest = requestHeader + requestBody;
        String cip_send_cmd = "AT+CIPSEND=" + String(httpRequest.length());
        bool rstatus = sendAT(cip_send_cmd.c_str(), (char*) ">", 10);
        if (rstatus) {
            delay(100); // Wait a short moment before sending the request
            if (sendAT(httpRequest.c_str(), (char*) "OK", 5000)) {
                sendAT("AT+CIPCLOSE", (char*) "OK", 5000);
            } else {
                Serial.println("No response from server");
            }
        } else {
            Serial.println("AT+CIPSEND error!");
        }
    } else {
        Serial.println("TCP error!");
    }
}



#endif
