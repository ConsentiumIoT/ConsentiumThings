#ifndef DEVICE_STATS_H
#define DEVICE_STATS_H

#include <Arduino.h>
#include "esp_system.h"

#if defined(CONFIG_IDF_TARGET_ESP32S2) || defined(CONFIG_IDF_TARGET_ESP32C3) || defined(CONFIG_IDF_TARGET_ESP32S3)
  #include "driver/temperature_sensor.h"
#endif

class DeviceStats {
public:
    DeviceStats();
    void begin();

    float getCPUTemperature();
    uint32_t getFreeHeap();
    unsigned long getUptimeSeconds();
    String getResetReason();

private:
    void initTempSensor();

#if defined(CONFIG_IDF_TARGET_ESP32S2) || defined(CONFIG_IDF_TARGET_ESP32C3) || defined(CONFIG_IDF_TARGET_ESP32S3)
    temperature_sensor_handle_t temp_sensor;
    bool tempSensorEnabled;
#endif
};

#endif
