#if defined(ESP32)
    #include <WiFi.h>
    #include <HTTPClient.h>
    #include <HTTPUpdate.h>  
    #include <WiFiClientSecure.h>  
    
    #define ledPin 23
    #define BOARD_TYPE "ESP32"

    #define S_0 21
    #define S_1 19
    #define S_2 18
    #define S_3 5    
    #define ADC_IN 34 // ADC1_CH6

    #define ADC_VREF_mV    3300.0 // in millivolt
    #define ADC_RESOLUTION 4096.0
#elif defined(ESP8266)
    #include <ESP8266WiFi.h>
    #include <ESP8266HTTPClient.h>
    #include <ESP8266httpUpdate.h>
    #include <WiFiClientSecure.h>

    #define ledPin 16
    #define BOARD_TYPE "ESP8266"
            
    #define S_0 14 
    #define S_1 12
    #define S_2 13
    #define S_3 15  
    #define ADC_IN A0 // A0

    #define ADC_VREF_mV    3300.0 // in millivolt
    #define ADC_RESOLUTION 1024.0
#elif defined(ARDUINO_RASPBERRY_PI_PICO_W)
    #include <WiFi.h>
    #include <HTTPClient.h>
    #include <HTTPUpdate.h>
    #include <WiFiClientSecure.h>
    
    #define ledPin LED_BUILTIN
    #define BOARD_TYPE "RASPBERRY-PI-PICO-W"

    #define S_0 18 
    #define S_1 19
    #define S_2 20
    #define S_3 21

    #define ADC_IN 26 // ADC0
    #define ADC_VREF_mV    3300.0 // in millivolt
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

#define SELECT_LINES 4
#define MUX_IN_LINES 16

#define WIFI_DELAY 500

#define NTP_SYNC_WAIT 8 * 3600 * 2

#define MAX_JSON_SIZE 200
#define MAX_JSON_SENSOR_DATA_SIZE 50

#define ARRAY_RESERVE 100

#define ESPBAUD 115200

#define LOW_PRE 2
#define MID_PRE 4
#define HIGH_PRE 7