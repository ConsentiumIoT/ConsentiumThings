#ifndef CONSENTIUM_SLEEP_H
#define CONSENTIUM_SLEEP_H

#include <Arduino.h>

// Sleep modes supported
enum ConsentiumSleepMode {
    CONSENTIUM_DEEP_SLEEP,
    CONSENTIUM_LIGHT_SLEEP
};

class ConsentiumSleep {
public:
    static void sleep(unsigned long interval_ms, ConsentiumSleepMode mode = CONSENTIUM_DEEP_SLEEP);
};

#endif
