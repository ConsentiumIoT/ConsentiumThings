#ifndef ConsentiumThingsDalton_h
#define ConsentiumThingsDalton_h

#if defined(ESP32) || defined(ARDUINO_RASPBERRY_PI_PICO_W) || defined(ESP8266)


#include <PinDefinitions.h>

using namespace std;

class ConsentiumThingsDalton{
    public:
        ConsentiumThingsDalton();
        ConsentiumThingsDalton(const char*);
        void beginOTA(const char*, const char*);
        void beginSend(const char*, const char*);
        void beginReceive(const char*, const char*);
        void initWiFi(const char*, const char*);
        void initWiFiAutoConnect(const char* apName = "ConsentiumIoT_AP", const char* apPassword = "Consentium2024");
        void sendData(vector<double>, const char* [], int);
        double readCurrentBus(int);
        double readVoltageBus(int);
        vector<pair<double, String>> receiveData();
        void checkAndPerformUpdate();
    private:
        WiFiClientSecure client;
        HTTPClient http;
        Adafruit_ADS1115 ads_1;
        Adafruit_ADS1115 ads_2;

        String sendUrl;
        String receiveUrl; 
        String versionUrl;
        String firmwareUrl;
        const char* firmwareVersion;
        const char* getRemoteFirmwareVersion();     
};

#endif

#endif
