/**
 * src.ino -> main setup and loop
 * 
 * Thank you to:
 * me-no-dev:   https://github.com/me-no-dev/ESPAsyncWebServer
 */
 
#include <WiFi.h>
#include "ESPAsyncWebServer.h"
#include "CommunicationStuff.hh"
#include "Electrical.hh"
#include "esp32-hal-ledc.h"


/**
 * MOTOR LAYOUT [1 -> 1 mapping of array indices]
 * 0 -- 3
 *   ||
 * 1 -- 4
 *   ||
 * 2 -- 5
 */
const int LEFT_DRIVE_CHANNELS[] = {1, 2, 3};
const int RIGHT_DRIVE_CHANNELS[] = {4, 5, 6};

const int LEFT_DRIVE_PINS[] = {20, 21, 22};
const int RIGHT_DRIVE_PINS[] = {24, 25, 26};
const int NUM_MOTORS_PER_SIDE = 3;

void setup()
{
  Serial.begin(115200);

  // setup electrical stuff
  for (int i = 0; i < NUM_MOTORS_PER_SIDE; i++) {
    setup(LEFT_DRIVE_PINS[i], LEFT_DRIVE_CHANNELS[i]);
    setup(RIGHT_DRIVE_PINS[i], RIGHT_DRIVE_CHANNELS[i]);

    setDriveChannel(1, LEFT_DRIVE_CHANNELS[i]);
    setDriveChannel(0, RIGHT_DRIVE_CHANNELS[i]);
  }

  // setup networking stuff
  connectToWiFi();
  setupESPServer();
}

void loop() { /* Intentionally empty. Required for Asynchronous Web Server. */
   for (int dutyCycle = 0; dutyCycle <= 255; dutyCycle++) {
    ledcWrite(ledChannel, dutyCycle);
    delay(7);
  }
 
  for (int dutyCycle = 255; dutyCycle >= 0; dutyCycle--) {
    ledcWrite(ledChannel, dutyCycle);
    delay(7);
  }
}
