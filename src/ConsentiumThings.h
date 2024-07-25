#ifndef ConsentiumThings_h
#define ConsentiumThings_h

#include <ArduinoJson.h>
#include <PinDefinitions.h>

class ConsentiumThings{
    public:
        ConsentiumThings();
        ConsentiumThings(const char*);
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
