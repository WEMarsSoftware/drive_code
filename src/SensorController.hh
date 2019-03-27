// SensorController.hh

#ifndef SENSOR_CONTROLLER
#define SENSOR_CONTROLLER


#include "Arduino.h"
#include "RotaryEncoder.h"
#include "SPI.h"

// Note: different than Arduino
const int MAX_ANALOG_IN = 4095;
const int MIN_ANALOG_IN = 0;

// CURRENT SENSORS
// varies linearly with DC in
const int NUM_CHASSIS_MOTORS = 6;

const int MAX_CURRENT_IN = 33;

static const int spiClk = 1000000; // 1 MHz

//uninitalised pointers to SPI objects
SPIClass * vspi = NULL;
SPIClass * hspi = NULL;

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

  // SPI constants
  const int HSPI_CLK = 14;
  const int HSPI_MISO = 12;
  const int HSPI_MOSI = 13;
  const int HSPI_CS_CURR = 1;
  const int HSPI_CS_IO = 3;
  
  const int VSPI_CLK = 18;
  const int VSPI_MISO = 19;
  const int VSPI_MOSI = 23;
  const int VSPI_CS_POT = 26;
  const int VSPI_EOC_POT = 27;

  const int CAN_R = 22;
  const int CAN_D = 21;
  
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
int SensorController::A_PINS[NUM_CHASSIS_MOTORS] = {36, 34, 32, 25, 26, 19};
int SensorController::B_PINS[NUM_CHASSIS_MOTORS] = {39, 35, 33, 23, 27, 18};
//int SensorController::CURRENT_IN[NUM_CHASSIS_MOTORS] = {9, 10, 11, 19, 18, 5};
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
	/*volatile unsigned int a = 1;

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
  	} */
}

void SensorController::setupSensors(void* args)
{
	// create RotaryEncoder objects
	/*for (int i = 0; i < NUM_CHASSIS_MOTORS; i++)
	{
		//encoders[i] = new RotaryEncoder(A_PINS[i], B_PINS[i], 1, 1, ENCODER_TIME);
	}*/

   //initialise two instances of the SPIClass attached to VSPI and HSPI respectively
  vspi = new SPIClass(VSPI);
  hspi = new SPIClass(HSPI);
  
  //clock miso mosi ss

  //initialise vspi with default pins
  //SCLK = 18, MISO = 19, MOSI = 23, SS = 5
  vspi->begin();
  //alternatively route through GPIO pins of your choice
  //hspi->begin(0, 2, 4, 33); //SCLK, MISO, MOSI, SS
  
  //initialise hspi with default pins
  //SCLK = 14, MISO = 12, MOSI = 13, SS = 15
  hspi->begin(); 
  //alternatively route through GPIO pins
  //hspi->begin(25, 26, 27, 32); //SCLK, MISO, MOSI, SS

  //set up slave select pins as outputs as the Arduino API
  //doesn't handle automatically pulling SS low
  pinMode(5, OUTPUT); //VSPI SS
  pinMode(15, OUTPUT); //HSPI SS
  
	// don't let this task end
	sensorsCoreLoop();
}


// the loop function runs over and over again until power down or reset
void loop() {
  //use the SPI buses
  vspiCommand();
  hspiCommand();
  delay(100);
}

void vspiCommand() {
  byte data = 0b01010101; // junk data to illustrate usage

  //use it as you would the regular arduino SPI API
  vspi->beginTransaction(SPISettings(spiClk, MSBFIRST, SPI_MODE0));
  digitalWrite(5, LOW); //pull SS slow to prep other end for transfer
  vspi->transfer(data);  
  digitalWrite(5, HIGH); //pull ss high to signify end of data transfer
  vspi->endTransaction();
}

void hspiCommand() {
  byte stuff = 0b11001100;
  
  hspi->beginTransaction(SPISettings(spiClk, MSBFIRST, SPI_MODE0));
  digitalWrite(15, LOW);
  hspi->transfer(stuff);
  digitalWrite(15, HIGH);
  hspi->endTransaction();
}

#endif
