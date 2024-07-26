#ifndef ConsentiumThings_h
#define ConsentiumThings_h

#if defined(ESP32) || defined(ARDUINO_RASPBERRY_PI_PICO_W) || defined(ESP8266)
    #include "ConsentiumThingsDalton.h"   
#elif defined(ARDUINO_AVR_PRO) || defined(ARDUINO_AVR_UNO)
    #include "ConsentiumThingsBeta.h"
#endif

#endif

