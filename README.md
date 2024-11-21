## ConsentiumThingsDalton

**Version:** v2.1.0

**Description:**

This project provides a simple example of how to use the Consentium Things IoT board to read sensor data and send it over HTTPS (only ESP32/ESP 8266/Raspberry Pi Pico W supported) to the ConsentiumThings cloud.

**How to use:**

1. Clone this repository to your local machine.
2. Install the Arduino IDE.
3. Connect the ConsentiumThingsDalton board to your computer using a USB cable.
4. Open the Arduino IDE and open the `ThingsUpdate.ino` file.
5. Enter your WiFi SSID and password in the `ssid` and `pass` variables, respectively.
6. Enter your ConsentiumThings API key in the `ApiKey` variable.
7. Enter your ConsentiumThings board API key in the `BoardApiKey` variable.
8. Upload the code to the ConsentiumThingsDalton board.
9. The board will start reading sensor data and sending it over REST to the ConsentiumThings cloud every 7 seconds.

**Notes:**

* You can change the sampling interval by changing the `interval` variable.
* You can change the precision of the data sent over REST by changing the `pre` variable.
* You can add additional sensors to the board by connecting them to the appropriate pins and updating the `sensorCount` variable and the `sensorValues` and `sensorInfo` arrays.
* You can connect an LED to GPIO 16 for ESP 8266, GPIO 23 for ESP 32 and GPIO 25 for Raspberry Pi Pico W to indicate the REST events.
**License:**

This project is licensed under the MIT License.

**Badges:**

[![MIT License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)

**Contributing:**

Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

# Licensing and Credits
* The [Arduino IDE and ArduinoCore-API](https://arduino.cc) are developed and maintained by the Arduino team. The IDE is licensed under GPL.
* The [RP2040 GCC-based toolchain](https://github.com/earlephilhower/pico-quick-toolchain) is licensed under under the GPL.
* The [Pico-SDK](https://github.com/raspberrypi/pico-sdk) is by Raspberry Pi (Trading) Ltd and licensed under the BSD 3-Clause license.
* The [ESP8266 Libraries] modified from ESP8266 Core Development Team: [GitHub](https://github.com/esp8266/Arduino)
* The [ESP32 Libraries are part of Espressif Systems]: [Website](https://www.espressif.com/) ESP32 Arduino Core Development Team: [GitHub](https://github.com/espressif/arduino-esp32)
  
