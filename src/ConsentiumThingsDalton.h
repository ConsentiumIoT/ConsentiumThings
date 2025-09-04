#ifndef ConsentiumThingsDalton_h
#define ConsentiumThingsDalton_h

#include <PinDefinitions.h>

using namespace std;

class ConsentiumThingsDalton{
    public:
        ConsentiumThingsDalton();
        ConsentiumThingsDalton(const char*);
        void startSensing();
        void enableAirUpdate(const char*, const char*);
        void enableSend(const char*, const char*);
        void enableReceive(const char*, const char*);
        void connectWiFi(const char*, const char*);
        void smartConnect();
        void pushData(vector<double>, const char* [], int);
        double readCurrentBus(int);
        double readVoltageBus(int);
        vector<pair<double, String>> pullData();
        void setSyncInterval(int);
        void airSync(vector<double>, const char* [], int);
    private:
        WiFiClientSecure client;
        WiFiManager wm;
        HTTPClient http;
        Adafruit_ADS1115 ads_1;
        Adafruit_ADS1115 ads_2;

        String sendUrl;
        String receiveUrl; 
        String versionUrl;
        String firmwareUrl;
        const char* firmwareVersion;
        const char* getRemoteFirmwareVersion();
        int airSyncInterval;
        int pushCounter;     
};

#endif

