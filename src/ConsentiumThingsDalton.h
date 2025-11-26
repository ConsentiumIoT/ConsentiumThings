#ifndef ConsentiumThingsDalton_h
#define ConsentiumThingsDalton_h

#include <PinDefinitions.h>
#include "ConsentiumSleep.h"

using namespace std;

class ConsentiumThingsDalton{
    public:
        // Main objects
        ConsentiumThingsDalton();
        ConsentiumThingsDalton(const char*);

        // Air Sync and OTA
        void setSyncInterval(int);
        void airSync(vector<double>, const char* [], int);
        void enableAirUpdate(const char*, const char*);
        
        // Data Send and Receive
        void enableSend(const char*, const char*);
        void enableReceive(const char*, const char*);
        vector<pair<double, String>> pullData();
        void pushData(vector<double>, const char* [], int);
        
        // WiFi Management
        void connectWiFi(const char*, const char*);
        void smartConnect();

        
        // Current and Voltage Sensing
        void startSensing();
        double readCurrentBus(int);
        double readVoltageBus(int);
        

        // Battery Monitoring
        void enableBatteryMonitoring(int pin, float refVoltage = 3.3, float dividerRatio = 2.0);
        void disableBatteryMonitoring();

        // Sleep
        void sleep(unsigned long interval_ms, ConsentiumSleepMode mode = CONSENTIUM_DEEP_SLEEP);
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
        
        bool batteryMonitoringEnabled = false;
        int batteryPin;
        float refVoltage;
        float dividerRatio;
        double getBatteryVoltage();
};

#endif

