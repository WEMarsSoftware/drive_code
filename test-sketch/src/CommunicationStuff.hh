// CommunicationStuff.hh

#ifndef COMMUNICATION_STUFF
#define COMMUNICATION_STUFF

#include "Arduino.h"
#include <WiFi.h>
#include "ESPAsyncWebServer.h"
#include "Electrical.hh"
#include "SensorController.hh"

// INFO FOR LOCAL ROUTER
char* ssid = "HUAWEI P20 Pro";
const char* password = "WeMars123";

// COMMUNICATION CONSTANTS
// Testing with router settings:
//    Ethernet
//    Manually
//    192.168.1.13
//    255.255.0.0
//    192.168.0.50
AsyncWebServer server(80);
IPAddress staticIP(192,168,1,16);
IPAddress gateway(10,10,10,1);
IPAddress subnet(255,255,255,0);

int motorShutdown = 0;

// POST PARAMETERS
const char* motor1 = "MOTOR1";
const char* motor2 = "MOTOR2";
const int NUM_PARAMS = 2;

//HTTP GET PARAMS
const String motorParams[] = {"left-side", "right-side"};

// incremented on each http connection
int numPings = 0;

void inline connectToWiFi()
{
    // Set WiFi to station mode and disconnect from an AP if it was previously connected
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);

    // ensure our IP is 10.10.10.10
    WiFi.config(staticIP, gateway, subnet);
    
    delay(100);
  
    // WiFi.scanNetworks will return the number of networks found
    int n = WiFi.scanNetworks();
  

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
#ifdef DEBUG
      Serial.println("Connecting to WiFi..");
#endif
    }

#ifdef DEBUG
    Serial.println("CONNECTED TO " + String(ssid));
    Serial.println(WiFi.localIP());
    Serial.println(WiFi.macAddress());
#endif
}

// Note: will be running on Core #1 (the default core)
void inline setupESPServer(void * args)
{    
  /**
   * HTTP callback with paramaters
   * Will be expecting <IP>/?left-side=#&right-side=#
   * Thanks to:
   * https://techtutorialsx.com/2017/12/17/esp32-arduino-http-server-getting-query-parameters/
   */
   server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
      numPings++;
      
      // parse parameters into left and right
      int numParams = request->params();
      
      if (numParams != NUM_PARAMS) {
        request->send(200, "text/plain", "Error: number of paramaters not as expected.");
      }

      String left, right;
      for ( int i = 0; i < numParams; i++ ) {
        AsyncWebParameter* p = request->getParam(i);
        String name = p->name();
        if (name == motorParams[0]) {
            left = p->value();
        } else if (name == motorParams[1]) {
            right = p->value();
        }
      }
       
       // if all went well, we now have the two power percentage values
       // set motor output signals
       moveMotors(left.toInt(), right.toInt());
       
       
       // send response back with motor current vals
       // and rotary encoder positions in JSON
       // are both size 6 arrays
       int* currentSensors = SensorController::currentValues;
       int* speedVals = SensorController::speedValues;

       String response = String("{") 
                + "\"Current-Left-0\":\"" + String(currentSensors[0]) 
                + "\",\"Current-Left-1\":\"" + String(currentSensors[1])
                + "\",\"Current-Left-2\":\"" + String(currentSensors[2])
                + "\",\"Current-Right-0\":\"" + String(currentSensors[3])
                + "\",\"Current-Right-1\":\"" + String(currentSensors[4])
                + "\",\"Current-Right-2\":\"" + String(currentSensors[5])
                + "\",\"Speed-Left-0\":\"" + String(speedVals[0])
                + "\",\"Speed-Left-1\":\"" + String(speedVals[1])
                + "\",\"Speed-Left-2\":\"" + String(speedVals[2])
                + "\",\"Speed-Right-0\":\"" + String(speedVals[3])
                + "\",\"Speed-Right-1\":\"" + String(speedVals[4])
                + "\",\"Speed-Right-2\":\"" + String(speedVals[5]) 
                + "\"}";
        request->send(200, "text/plain", response);
       
       // send success
       // request->send(200, "text/plain", "SUCCESS");
   });

  /* Test callback -> test connection without affecting motors */
  server.on("/hello", HTTP_GET, [](AsyncWebServerRequest *request){ 
       // send success
       request->send(200, "text/plain", "Hello!");
   });
   
  server.begin();
}

#endif
