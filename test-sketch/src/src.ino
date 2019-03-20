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
#include "PreprocessorOptions.hh"
#include "esp32-hal-ledc.h"

// required for hal-ledc
const int LEFT_DRIVE_CHANNELS[] = {1, 2, 3};
const int RIGHT_DRIVE_CHANNELS[] = {4, 5, 6};

const int LEFT_DRIVE_PINS[] = {32, 33, 25};
const int RIGHT_DRIVE_PINS[] = {4, 2, 15};

const int NUM_MOTORS_PER_SIDE = 3;

// required for AsynchronousWebServer to run on alternate core
TaskHandle_t Task1;

// for timer interrupts
hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
int lastPingVal = 0;

// Stops motors if we lost connection
void IRAM_ATTR onTimer() {
  portENTER_CRITICAL_ISR(&timerMux);
  
  if (lastPingVal == numPings) {
    moveMotors(0, 0); // TURN MOTORS OFF -> WE LOST CONNECTION
  }
  lastPingVal = numPings;
  
  portEXIT_CRITICAL_ISR(&timerMux);
}

void setup()
{
#ifdef DEBUG
  Serial.begin(115200);
#endif

  // setup electrical stuff
  for (int i = 0; i < NUM_MOTORS_PER_SIDE; i++) {
    setup(LEFT_DRIVE_PINS[i], LEFT_DRIVE_CHANNELS[i]);
    setup(RIGHT_DRIVE_PINS[i], RIGHT_DRIVE_CHANNELS[i]);

    setDriveChannel(i, LEFT_DRIVE_CHANNELS[i]);
    setDriveChannel(i+3, RIGHT_DRIVE_CHANNELS[i]);
  }

  // setup networking stuff
  connectToWiFi();


  // run WiFi server and control motor PWM outputs (CORE 0 - secondary core)
  /*
   * 
   xTaskCreatePinnedToCore(
            setupESPServer, /* Function 
            "ServerTask",   /* Name 
            10000,          /* Stack size 
            NULL,           /* Parameter of function 
            0,              /* Priority - 0 since this should override timer and sensor ISR's 
            &Task1,         /* Task handle 
            0               /* Core 
            );  **/
            
  setupESPServer(NULL);
  
  // timer interrupt to check for connection loss
  // runs once per second
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 1000000, true);
  timerAlarmEnable(timer);

  // run sensor ISR on main core
  SensorController::setupSensors(nullptr);
}

void loop() { /* Intentionally empty. Required for Asynchronous Web Server. */ }
