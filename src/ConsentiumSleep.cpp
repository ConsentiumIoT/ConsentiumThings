#include "ConsentiumSleep.h"

#if defined(ESP32)
  #include "esp_sleep.h"
#elif defined(ESP8266)
  extern "C" {
    #include "user_interface.h"
  }
#elif defined(ARDUINO_ARCH_RP2040)
  #include <WiFi.h>
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

    #elif defined(ESP8266)
        if (mode == CONSENTIUM_DEEP_SLEEP) {
            Serial.printf("[ConsentiumSleep] ESP8266 deep sleep for %lu ms\n", interval_ms);
            Serial.flush();
            ESP.deepSleep(interval_ms * 1000ULL);
        } else {
            delay(interval_ms);
        }

    #elif defined(ARDUINO_ARCH_RP2040)
        // Pico W "fake-deep-sleep"
        Serial.printf("[ConsentiumSleep] Pico W simulated sleep for %lu ms\n", interval_ms);
        WiFi.disconnect(true);
        WiFi.mode(WIFI_OFF);
        sleep_ms(interval_ms);
        WiFi.mode(WIFI_STA);

    #else
        Serial.println("[ConsentiumSleep] Unsupported MCU — using delay().");
        delay(interval_ms);
    #endif
}
