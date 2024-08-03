#ifndef ConsentiumThingsBeta_h
#define ConsentiumThingsBeta_h

#include <Arduino.h>

#if defined(ARDUINO_AVR_PRO) || defined(ARDUINO_AVR_UNO)

#include <PinDefinitions.h>


bool sendAT(const char*, char[]);

class ConsentiumThingsBeta{
    public:
        ConsentiumThingsBeta();
        void beginSend(const char*, const char*);
        void initWiFi(const char*, const char*);
        void sendData(float[], const char*[], int, int);
    private:
        const char* key;
        const char* board_key;
};
#endif

#endif
