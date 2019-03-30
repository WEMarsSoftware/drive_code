// SensorController.hh

#ifndef SENSOR_CONTROLLER
#define SENSOR_CONTROLLER


#include "Arduino.h"
#include "RotaryEncoder.h"

// Note: different than Arduino
const int MAX_ANALOG_IN = 4095;
const int MIN_ANALOG_IN = 0;

// CURRENT SENSORS
// varies linearly with DC in
const int NUM_CHASSIS_MOTORS = 6;

const int MAX_CURRENT_IN = 33;

// code running on Core #0
class SensorController
{
public:
	// let WiFi callback access these directly
	static int currentValues[NUM_CHASSIS_MOTORS];
	static int speedValues[NUM_CHASSIS_MOTORS];

	// RotaryEncoder library will count number of ticks in specified time length
	// specified in constructor (last parameter in microseconds)
	static RotaryEncoder* encoders[NUM_CHASSIS_MOTORS];
	static int deltaTicks[NUM_CHASSIS_MOTORS];

	// pin assignments
	static int A_PINS[NUM_CHASSIS_MOTORS];
	static int B_PINS[NUM_CHASSIS_MOTORS];
	static int CURRENT_IN[NUM_CHASSIS_MOTORS];

	// constants
  	static const int CORE_LOOP_DELAY;
  	static const int ENCODER_TIME;
  //	static const int NUM_CHASSIS_MOTORS = 6;

	// main loop to update data points
	// infinite -> process ends with a power off of rover
	static void sensorsCoreLoop();

	// attach encoders to pins
	static void setupSensors(void* args);
};

// link statics
int SensorController::currentValues[NUM_CHASSIS_MOTORS] = {};
int SensorController::speedValues[NUM_CHASSIS_MOTORS] = {};
RotaryEncoder* SensorController::encoders[NUM_CHASSIS_MOTORS];

// pin assignments temporary
int SensorController::A_PINS[NUM_CHASSIS_MOTORS] = {13, 12, 14, 27, 26, 25};
int SensorController::B_PINS[NUM_CHASSIS_MOTORS] = {37, 39, 36, 34, 23, 22};
int SensorController::CURRENT_IN[NUM_CHASSIS_MOTORS] = {9, 10, 11, 19, 18, 5};
int SensorController::deltaTicks[NUM_CHASSIS_MOTORS] = {};

// constants
const int SensorController::CORE_LOOP_DELAY = 10;
const int SensorController::ENCODER_TIME = 1000;
//const int SensorController::NUM_CHASSIS_MOTORS = 6;

// temp array for reading encoder ticks
int deltaTicks[NUM_CHASSIS_MOTORS] = {};
const int CORE_LOOP_DELAY = 10;
const int ENCODER_TIME = 1000;

void SensorController::sensorsCoreLoop()
{
	// volatile so this isn't optimized away
	volatile unsigned int a = 1;

	while (a)
	{
		// update encoder and current sensor data
		for (int i = 0; i < NUM_CHASSIS_MOTORS; i++)
		{
			deltaTicks[i] = encoders[i]->readEncoder();
			speedValues[i] = (double)(deltaTicks[i])/ENCODER_TIME;

			currentValues[i] = analogRead(CURRENT_IN[i]);
		}
		delay(CORE_LOOP_DELAY);
  	} 
}

void SensorController::setupSensors(void* args)
{
	// create RotaryEncoder objects
	for (int i = 0; i < NUM_CHASSIS_MOTORS; i++)
	{
		encoders[i] = new RotaryEncoder(A_PINS[i], B_PINS[i], 1, 1, ENCODER_TIME);
	}

	// don't let this task end
	sensorsCoreLoop();
}

#endif
