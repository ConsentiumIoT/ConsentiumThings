# ConsentiumThings

**Description:**  
The **ConsentiumThingsDalton** project demonstrates how to use the ConsentiumThings IoT board to read sensor data and transmit it over HTTPS to the ConsentiumThings cloud. This library supports **ESP32**, **ESP8266**, and **Raspberry Pi Pico W** edge devices.

---

## How to Use

1. **Clone the Repository**: Clone this repository to your local machine.
2. **Set Up Arduino IDE**:
   - Install the [Arduino IDE](https://www.arduino.cc/en/software).
   - Install the necessary board support packages for ESP32, ESP8266, or Raspberry Pi Pico W.
3. **Connect the Board**: Connect your **ConsentiumThingsDalton** board to your computer via USB.
4. **Load the Code**:
   - Open the Arduino IDE and load the `ThingsUpdate.ino` file.
   - Enter the following in the provided variables:
     - **WiFi SSID** and **password** in the `ssid` and `pass` variables.
     - **ConsentiumThings API key** in the `ApiKey` variable.
     - **ConsentiumThings board API key** in the `BoardApiKey` variable.
5. **Upload the Code**: Compile and upload the code to the **ConsentiumThingsDalton** board.
6. **Monitor Activity**:
   - The board will begin reading sensor data and transmitting it over REST to the **ConsentiumThings cloud** every 7 seconds by default.

---

## Features

- **Customizable Sampling Interval**:  
   Adjust the sampling interval by modifying the `interval` variable.
   
- **Data Precision Control**:  
   Set the precision of REST data transmission by updating the `pre` variable.

- **Add Sensors**:  
   Connect additional sensors to supported pins. Update:
   - `sensorValues` array with new sensor readings.
   - `sensorInfo` array with new sensor descriptions.
   - `sensorCount` variable to match the number of connected sensors.

- **REST Event LED Indication**:  
   Connect an LED to the following pins for visual REST event feedback:
   - **GPIO 16** for ESP8266  
   - **GPIO 23** for ESP32  
   - **GPIO 25** for Raspberry Pi Pico W  

---

## Notes

- Ensure secure transmission by using HTTPS endpoints for ConsentiumThings cloud communication.
- You can view the logs and debug information using the Serial Monitor in the Arduino IDE (set baud rate to **115200**).

---

## License

This project is licensed under the [MIT License](LICENSE).  

[![MIT License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)

---

## Contributing

Contributions are welcome!  
To contribute:
- Fork the repository.
- Submit a pull request with a clear description of changes.  
For significant changes, open an issue first to discuss your ideas.  

---

## Licensing and Credits

This project leverages open-source tools and libraries:
- **Arduino IDE and ArduinoCore-API**: Developed by the Arduino team ([GPL License](https://arduino.cc)).
- **RP2040 GCC-based Toolchain**: By Earle Philhower ([GPL License](https://github.com/earlephilhower/pico-quick-toolchain)).
- **Pico-SDK**: By Raspberry Pi (Trading) Ltd ([BSD 3-Clause License](https://github.com/raspberrypi/pico-sdk)).
- **ESP8266 Libraries**: Modified from ESP8266 Core Development Team ([GitHub](https://github.com/esp8266/Arduino)).
- **ESP32 Libraries**: Developed by Espressif Systems ([GitHub](https://github.com/espressif/arduino-esp32)).
