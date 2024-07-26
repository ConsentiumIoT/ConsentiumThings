#ifndef ConsentiumThingsDalton_h
#define ConsentiumThingsDalton_h

#if defined(ESP32) || defined(ARDUINO_RASPBERRY_PI_PICO_W) || defined(ESP8266)

#include <ArduinoJson.h>
#include <PinDefinitions.h>

class ConsentiumThingsDalton{
    public:
        ConsentiumThingsDalton();
        ConsentiumThingsDalton(const char*);
        void beginOTA(const char*, const char*);
        void beginSend(const char*, const char*);
        void beginReceive(const char*, const char*);
        void initWiFi(const char*, const char*);
        void sendREST(double [], const char* [], int, int); 
        std::vector<std::pair<double, String>> receiveREST();
        void checkAndPerformUpdate();
        float busRead(int); 
    private:
        WiFiClientSecure client;
        HTTPClient http;
        String sendUrl;
        String receiveUrl; 
        String versionUrl;
        String firmwareUrl;
        const char* firmwareVersion;
        const char* getRemoteFirmwareVersion();     
};

#endif

#endif
