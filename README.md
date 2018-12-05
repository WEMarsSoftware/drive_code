# Dependencies
 - arduino-esp32 core header files
 - ESPAsyncWebServer library
 - Built-in Arduino headers

 - Note: these should be installed in ~/Document/Arduino/libraries

# src.ino
Main controller code for this project.

# Electrical.hh
Custom API's for setting up and using PWM channels connected to Vex Victor motor controllers.

# CommunicationStuff.hh
Custom HTTP callbacks for interpreting drive commands and paramaters from the control station (see MissionControl repository).
