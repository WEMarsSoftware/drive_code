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
 * ESP-32 Pinout:
 * See drive_code/images
 */
const int LEFT_DRIVE_CHANNELS[] = {1, 2, 3};
const int RIGHT_DRIVE_CHANNELS[] = {4, 5, 6};

// on left hand side of board
const int LEFT_DRIVE_PINS[] = {32, 33, 25};

// on right hand side of board
const int RIGHT_DRIVE_PINS[] = {4, 2, 15};

const int NUM_MOTORS_PER_SIDE = 3;

void setup()
{
  Serial.begin(115200);

  // setup electrical stuff
  for (int i = 0; i < NUM_MOTORS_PER_SIDE; i++) {
    setup(LEFT_DRIVE_PINS[i], LEFT_DRIVE_CHANNELS[i]);
    setup(RIGHT_DRIVE_PINS[i], RIGHT_DRIVE_CHANNELS[i]);

    setDriveChannel(i, LEFT_DRIVE_CHANNELS[i]);
    setDriveChannel(i+3, RIGHT_DRIVE_CHANNELS[i]);
  }

  // setup networking stuff
  // TEST with router 
  // Ethernet
  // Manually
  // 192.168.1.13
  // 255.255.0.0
  // 192.168.0.50
  connectToWiFi();
  setupESPServer();
}

void loop() { /* Intentionally empty. Required for Asynchronous Web Server. */ }
