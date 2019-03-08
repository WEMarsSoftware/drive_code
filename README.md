# Running
- IP of the ESP-32 running this is set constant at 192.168.1.16
- HTTP GET to http://192.168.1.16/?left-side=<###>&right-side=<###>
	- the numbers in parameters correspond to "percentage powers" of chassis motors from -100 to +100	

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
Code for sending back JSON with sensor data values.

# ReadSensors.hh
Functions for reading data from current sensors and rotary encoders.
