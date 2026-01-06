#include "DeviceStats.h"

DeviceStats::DeviceStats() {
#if defined(CONFIG_IDF_TARGET_ESP32S2) || defined(CONFIG_IDF_TARGET_ESP32C3) || defined(CONFIG_IDF_TARGET_ESP32S3)
    tempSensorEnabled = false;
    temp_sensor = NULL;
#endif
}

void DeviceStats::begin() {
    analogReadResolution(12);
    initTempSensor();
}

void DeviceStats::initTempSensor() {
#if defined(CONFIG_IDF_TARGET_ESP32S2) || defined(CONFIG_IDF_TARGET_ESP32C3) || defined(CONFIG_IDF_TARGET_ESP32S3)
    temperature_sensor_config_t config = TEMPERATURE_SENSOR_CONFIG_DEFAULT(10, 50);
    esp_err_t err = temperature_sensor_install(&config, &temp_sensor);

    if (err == ESP_OK) {
        temperature_sensor_enable(temp_sensor);
        tempSensorEnabled = true;
    } else {
        Serial.println("Temp sensor init failed.");
    }
#endif
}

#if !defined(CONFIG_IDF_TARGET_ESP32S2) && !defined(CONFIG_IDF_TARGET_ESP32C3) && !defined(CONFIG_IDF_TARGET_ESP32S3)
extern "C" { uint8_t temprature_sens_read(); }
#endif

float DeviceStats::getCPUTemperature() {
#if defined(CONFIG_IDF_TARGET_ESP32S2) || defined(CONFIG_IDF_TARGET_ESP32C3) || defined(CONFIG_IDF_TARGET_ESP32S3)
    // Code for S2, C3, S3 using the new driver
    if (!tempSensorEnabled) return -1.0;

    float temp;
    if (temperature_sensor_get_celsius(temp_sensor, &temp) == ESP_OK)
        return temp;
    return -2.0;

#else
    // Code for Original ESP32 (Legacy)
    uint8_t t_f = temprature_sens_read();
    
    if (t_f == 128) return -1.0; // 128 usually indicates an invalid reading

    // Convert F to C: (F - 32) / 1.8
    return (t_f - 32) / 1.8;
#endif
}

uint32_t DeviceStats::getFreeHeap() {
    return ESP.getFreeHeap();
}

unsigned long DeviceStats::getUptimeSeconds() {
    return millis() / 1000;
}

String DeviceStats::getResetReason() {
    esp_reset_reason_t r = esp_reset_reason();
    switch (r) {
        case ESP_RST_POWERON:   return "POWERON";
        case ESP_RST_EXT:       return "EXTERNAL_PIN";
        case ESP_RST_SW:        return "SOFTWARE_RESET";
        case ESP_RST_PANIC:     return "PANIC";
        case ESP_RST_INT_WDT:   return "INT_WDT";
        case ESP_RST_TASK_WDT:  return "TASK_WDT";
        case ESP_RST_WDT:       return "WDT";
        case ESP_RST_DEEPSLEEP: return "DEEPSLEEP";
        case ESP_RST_BROWNOUT:  return "BROWNOUT";
        case ESP_RST_SDIO:      return "SDIO";
        default:                return "FIRST_BOOT";
    }
}

