#ifndef ConsentiumThings_h
#define ConsentiumThings_h

#if defined(ESP32)
    #include "ConsentiumThingsDalton.h"   
#else
    #error "ConsentiumThings supports ESP32-class boards only. ESP8266 and Raspberry Pi Pico W are no longer supported."
#endif

#endif

