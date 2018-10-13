/**
 * src.ino -> main setup and loop
 * 
 * Thank you to:
 * me-no-dev:   https://github.com/me-no-dev/ESPAsyncWebServer
 */
 
#include <WiFi.h>
#include "ESPAsyncWebServer.h"
#include "CommunicationStuff.hh"

/**
 * MOTOR LAYOUT [1 -> 1 mapping of array indices]
 * 0 -- 3
 *   ||
 * 1 -- 4
 *   ||
 * 2 -- 5
 */

void setup()
{
  Serial.begin(115200);

  // is inline
  connectToWiFi();
}


void loop() { /* Intentionally empty. Required for Asynchronous Web Server. */ }
