#include "ConsentiumSleep.h"

#if defined(ESP32)
  #include "esp_sleep.h"
#endif

void ConsentiumSleep::sleep(unsigned long interval_ms, ConsentiumSleepMode mode){
    Serial.println("[ConsentiumSleep] Entering sleep...");

    #if defined(ESP32)
        if (mode == CONSENTIUM_DEEP_SLEEP) {
            Serial.printf("[ConsentiumSleep] ESP32 deep sleep for %lu ms\n", interval_ms);
            Serial.flush();
            esp_sleep_enable_timer_wakeup(interval_ms * 1000ULL);
            esp_deep_sleep_start();
        } else {
            Serial.println("[ConsentiumSleep] Light sleep not implemented; using delay().");
            delay(interval_ms);
        }

    #else
        Serial.println("[ConsentiumSleep] Unsupported MCU — using delay().");
        delay(interval_ms);
    #endif
}
