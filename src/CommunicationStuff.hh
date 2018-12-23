// CommunicationStuff.hh

#ifndef COMMUNICATION_SETUP
#define COMMUNICATION_SETUP

#include "Arduino.h"
#include <WiFi.h>
#include "ESPAsyncWebServer.h"
#include "Electrical.hh"
#include "ReadSensors.hh"

// INFO FOR LOCAL ROUTER
char* ssid = "WE MARS Rover";
const char* password = "westillfirst";

// COMMUNICATION CONSTANTS
AsyncWebServer server(80);
IPAddress staticIP(192,168,1,16);
IPAddress gateway(10,10,10,1);
IPAddress subnet(255,255,255,0);

int motorShutdown = 0;

// POST PARAMETERS
const char* motor1 = "MOTOR1";
const char* motor2 = "MOTOR2";

//HTTP GET PARAMS
const String motorParams[] = {"left-side", "right-side"};

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
      Serial.println("Connecting to WiFi..");
    }

    Serial.println("CONNECTED TO " + String(ssid));
    Serial.println(WiFi.localIP());
    Serial.println(WiFi.macAddress());
}

void inline setupESPServer()
{    
  /**
   * HTTP callback with paramaters
   * Will be expecting <IP>/?left-side=#&right-side=#
   * Thanks to:
   * https://techtutorialsx.com/2017/12/17/esp32-arduino-http-server-getting-query-parameters/
   */
   server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
      
      int numParams = request->params();
      String left, right;
      // Serial.println("PARAM CALLBACK");
      for ( int i = 0; i < numParams; i++ ) {
        AsyncWebParameter* p = request->getParam(i);
        String name = p->name();
        // Serial.println("Param " + name);
        if (name == motorParams[0]) {
            left = p->value();
        } else if (name == motorParams[1]) {
            right = p->value();
        }
      }
       
       // if all went well, we now have the two power percentage values
       // use our Electrical API
      //  Serial.print("Calling move motors with params " + left);
       // Serial.println(" " + right);
       moveMotors(left.toInt(), right.toInt());
       
       
       // send response back with motor current vals
       // and rotary encoder positions in JSON
       // TODO: add getRotaryPositions() into ReadSensors.hh
       // TODO: add results into JSON here
       int* currentSensors = getCurrentValues(); // is size 6 array
       int* posVals = getRotaryPositions();
       String response = "{" 
                + "\"Current-Left-0\":\"" + String(currentSensors[0]) 
                + "\",\"Current-Left-1\":\"" + String(currentSensors[1])
                + "\",\"Current-Left-2\":\"" + String(currentSensors[2])
                + "\",\"Current-Right-0\":\"" + String(currentSensors[3])
                + "\",\"Current-Right-1\":\"" + String(currentSensors[4])
                + "\",\"Current-Right-2\":\"" + String(currentSensors[5])
                + "\",\"Pos-Left-1\":\"" + String(posVals[0])
                + "\",\"Pos-Left-1\":\"" + String(posVals[1])
                + "\",\"Pos-Left-2\":\"" + String(posVals[2])
                + "\",\"Pos-Right-0\":\"" + String(posVals[3])
                + "\",\"Pos-Right-1\":\"" + String(posVals[4])
                + "\",\"Pos-Right-2\":\"" + String(posVals[5]) 
                + "\"}";
        request->send(200, "text/plain", response);
       
       // send success
       // request->send(200, "text/plain", "SUCCESS");
   });

  /* Test callback -> test connection without affecting motors */
  server.on("/hello", HTTP_GET, [](AsyncWebServerRequest *request){ 
       // send success
       request->send(200, "text/plain", "SUCCESS");
   });
   
  server.begin();
}

#endif
