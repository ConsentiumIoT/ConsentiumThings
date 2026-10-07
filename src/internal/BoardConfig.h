#ifndef CONSENTIUM_BOARD_CONFIG_H
#define CONSENTIUM_BOARD_CONFIG_H

// Internal board and library settings.
// Only include this from library .cpp files — never from a public header.

#ifdef CONFIG_IDF_TARGET_ESP32C3
    #define CONSENTIUM_LED_PIN 10
    #define CONSENTIUM_BOARD_TYPE "ESP32-C3"
#else
    #define CONSENTIUM_LED_PIN 2
    #define CONSENTIUM_BOARD_TYPE "ESP32"
#endif

// I2C addresses of the onboard ADS1115 ADCs
#define CONSENTIUM_CURRENT_ADC_ADDR 0x48
#define CONSENTIUM_VOLTAGE_ADC_ADDR 0x49

#define CONSENTIUM_WIFI_DELAY 500
#define CONSENTIUM_I2C_DELAY 1000

#define CONSENTIUM_ARRAY_RESERVE 100

#endif // CONSENTIUM_BOARD_CONFIG_H

