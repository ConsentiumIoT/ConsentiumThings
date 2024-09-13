#if defined(ESP32)
    #include <WiFi.h>
    #include <Wire.h>
    #include <vector>
    #include <HTTPClient.h>
    #include <HTTPUpdate.h> 
    #include <ArduinoJson.h> 
    #include <WiFiClientSecure.h>
    #include <Adafruit_ADS1X15.h>
    
    #ifdef CONFIG_IDF_TARGET_ESP32C3
        #define ledPin LED_BUILTIN 
        #define BOARD_TYPE "ESP32-C3"
    #else
        #define ledPin 23
        #define BOARD_TYPE "ESP32"
    #endif
 
    #define ADC_IN 34 // ADC1_CH6
    #define ADC_VREF_mV    3300.0 // in millivolt
    #define ADC_RESOLUTION 4096.0

    const float multiplier = 0.1875F;

    #define currentADCAddr 0x48
    #define voltageADCAddr 0x49
#elif defined(ESP8266)
    #include <Wire.h>
    #include <vector>
    #include <ESP8266WiFi.h>
    #include <ArduinoJson.h>
    #include <ESP8266HTTPClient.h>
    #include <ESP8266httpUpdate.h>
    #include <WiFiClientSecure.h>
    #include <Adafruit_ADS1X15.h>

    #define ledPin 16
    #define BOARD_TYPE "ESP8266"
            
    #define ADC_IN A0 // A0
    #define ADC_VREF_mV    3300.0 // in millivolt
    #define ADC_RESOLUTION 1024.0

    const float multiplier = 0.1875F;
    #define currentADCAddr 0x48
    #define voltageADCAddr 0x49
#elif defined(ARDUINO_RASPBERRY_PI_PICO_W)
    #include <Wire.h>
    #include <WiFi.h>
    #include <vector>
    #include <ArduinoJson.h>
    #include <HTTPClient.h>
    #include <HTTPUpdate.h>
    #include <WiFiClientSecure.h>
    #include <Adafruit_ADS1X15.h>
    
    #define ledPin LED_BUILTIN
    #define BOARD_TYPE "RASPBERRY-PI-PICO-W"

    #define ADC_IN 26 // ADC0
    #define ADC_VREF_mV    3300.0 // in millivolt

    const float multiplier = 0.1875F;
    #define currentADCAddr 0x48
    #define voltageADCAddr 0x49
#elif defined(ARDUINO_AVR_PRO) || defined(ARDUINO_AVR_UNO)
    // Internal definitions
    #define COMM_BAUD 115200
    #define INTERVAL_MUL 1000
    #define MAX_WIFI_ATTEMPTS 200
    #define WIFI_RETRY_DELAY 100

    // IO definitions
    #define SCALE 0.488
    #define IN_4_20_1 A0
    #define IN_4_20_2 A1
    #define IN_4_20_3 A2 
    #define IN_0_10_1 A3
    #define IN_0_10_2 A4
    #define IN_0_10_3 A5

    // Precision value
    #define LOW_PRE 2
    #define MID_PRE 4
    #define HIGH_PRE 6
#endif


#define WIFI_DELAY 500
#define I2C_DELAY 1000

#define NTP_SYNC_WAIT 8 * 3600 * 2

#define MAX_JSON_SIZE 200
#define MAX_JSON_SENSOR_DATA_SIZE 50

#define ARRAY_RESERVE 100

#define ESPBAUD 115200

#define LOW_PRE 2
#define MID_PRE 4
#define HIGH_PRE 7