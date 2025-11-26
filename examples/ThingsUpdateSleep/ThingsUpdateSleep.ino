/**
 * -------------------------------------------------------------
 *     ConsentiumThings™ — Smarter IoT Starts at the Edge
 * -------------------------------------------------------------
 *  Example: Using Deep Sleep with the Dalton (CT-Dalton) Library
 *
 *  This example demonstrates:
 *   ✓ WiFi connection using ConsentiumThingsDalton
 *   ✓ Enabling secure cloud sync
 *   ✓ Sending sensor data using airSync()
 *   ✓ Putting the device into deep sleep using board.sleep()
 *
 *  Learn more: https://consentiuminc.com/iot
 * -------------------------------------------------------------
 */

#include <ConsentiumThings.h>

// Initialize the board instance with firmware version
ConsentiumThingsDalton board;

void setup() {
  // Connect to WiFi
  board.connectWiFi("SSID", "PASS");

  // Enable secure telemetry transmission
  board.enableSend("SEND_KEY", "BOARD_KEY");

  // Example sensor data
  vector<double> data = {25.4};
  const char* info[] = {"Temperature"};

  // Send data to cloud
  board.pushData(data, info, LOW_PRE);

  // Put device into deep sleep for 60 seconds
  board.sleep(60000);
}

void loop() {
  // Device will not reach here during deep sleep
}
